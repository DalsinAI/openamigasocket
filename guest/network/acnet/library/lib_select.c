/* ACNet bsdsocket.library: provider readiness, timeouts and WaitSelect. MIT. */
#include <exec/types.h>
#include <proto/exec.h>
#include <devices/timer.h>
#include <sys/socket.h>
#include "lib_internal.h"

static LONG timer_open(struct SocketBase *sb){if(sb->tdev_open)return 0;sb->tport=CreateMsgPort();if(!sb->tport)return fail(sb,AE_NOMEM);sb->treq=(struct timerequest*)CreateIORequest(sb->tport,sizeof(*sb->treq));if(!sb->treq){DeleteMsgPort(sb->tport);sb->tport=NULL;return fail(sb,AE_NOMEM);}if(OpenDevice(TIMERNAME,UNIT_MICROHZ,(struct IORequest*)sb->treq,0)){DeleteIORequest((struct IORequest*)sb->treq);DeleteMsgPort(sb->tport);sb->treq=NULL;sb->tport=NULL;return fail(sb,AE_NOMEM);}sb->tdev_open=1;return 0;}
void timer_close(struct SocketBase *sb){if(!sb->tdev_open)return;if(!CheckIO((struct IORequest*)sb->treq)){AbortIO((struct IORequest*)sb->treq);WaitIO((struct IORequest*)sb->treq);SetSignal(0,1UL<<sb->tport->mp_SigBit);}CloseDevice((struct IORequest*)sb->treq);DeleteIORequest((struct IORequest*)sb->treq);DeleteMsgPort(sb->tport);sb->treq=NULL;sb->tport=NULL;sb->tdev_open=0;}
/* The timer port's signal bit is not a reliable "timer fired" flag: AbortIO()
 * replies the request at once, which signals the port, and WaitIO() then finds
 * it already replied and leaves the bit set. Clear it before every request and
 * after every cancel, and judge expiry by io_Error, not by the signal. */
static ULONG timer_start(struct SocketBase *sb,const struct timeval *tv){ULONG m;if(!tv)return 0;if(timer_open(sb)<0)return 0;m=1UL<<sb->tport->mp_SigBit;SetSignal(0,m);sb->treq->tr_node.io_Command=TR_ADDREQUEST;sb->treq->tr_time=*tv;SendIO((struct IORequest*)sb->treq);return m;}
/* 1 if the request ran its full time, 0 if it was aborted. */
static LONG timer_cancel(struct SocketBase *sb){if(!sb->tdev_open)return 0;if(!CheckIO((struct IORequest*)sb->treq))AbortIO((struct IORequest*)sb->treq);WaitIO((struct IORequest*)sb->treq);SetSignal(0,1UL<<sb->tport->mp_SigBit);return sb->treq->tr_node.io_Error==0;}

LONG wait_items(struct SocketBase *sb,ULONG *items,ULONG *revents,LONG n,const struct timeval *tv,ULONG extra,ULONG *got_extra){ULONG rxlen=0,tmask,sigs;LONG ready,expired;if(got_extra)*got_extra=0;for(;;){prov_arm(sb);ready=prov_call(sb,ACHS_CMD_POLL,n,0,0,0,items,n*8,revents,n*4,&rxlen);if(ready<0){prov_disarm(sb);return fail_provider(sb);}if(ready>0){prov_disarm(sb);return ready;}if(tv&&tv->tv_secs==0&&tv->tv_micro==0){prov_disarm(sb);return 0;}tmask=timer_start(sb,tv);if(tv&&!tmask){prov_disarm(sb);return -1;}sigs=Wait(sb->provider_sigmask|tmask|extra|sb->sigintr);prov_disarm(sb);expired=tmask?timer_cancel(sb):0;if(sigs&sb->sigintr){SetSignal(sigs&sb->sigintr,sigs&sb->sigintr);return fail(sb,AE_INTR);}if(sigs&extra){if(got_extra)*got_extra=sigs&extra;return 0;}if(expired)return 0;if(!n&&(sigs&sb->provider_sigmask))return 0;/* a pure event wait (wait_event): ACNet's signal ends it, the caller asks again */}}
LONG wait_ready(struct SocketBase *sb,LONG handle,ULONG events,const struct timeval *tv){ULONG items[2]={handle,events},rev=0;LONG r=wait_items(sb,items,&rev,1,(tv&&(tv->tv_secs||tv->tv_micro))?tv:NULL,0,NULL);if(r<=0)return r;return (LONG)rev;}
LONG wait_event(struct SocketBase *sb){ULONG items[2]={0,0},rev=0;LONG r=wait_items(sb,items,&rev,0,NULL,0,NULL);return r<0?-1:0;}

/* A break signal (SBTC_BREAKMASK, Ctrl-C unless the program sets it) ends a wait with EINTR and is set again
 * after Wait() took it, as Roadshow and AmiTCP leave it: the program checks it after EINTR (ping's
 * CheckSignal() of its timer and of Ctrl-C). 4.3: before, the signal was eaten, so ping sent one packet and
 * then waited for ever, deaf to Ctrl-C too. */
/* Wait after the caller has already armed the provider, preserving the classic
 * arm -> poll -> sleep pattern without losing an event between poll and Wait().
 * 1=provider event, 0=timeout, 2=extra signal, -1=interrupt/error. */
LONG wait_armed_event(struct SocketBase *sb,const struct timeval *tv,ULONG extra,ULONG *got_extra)
{
 ULONG tmask=0,sigs;LONG expired;if(got_extra)*got_extra=0;
 if(tv){tmask=timer_start(sb,tv);if(!tmask){prov_disarm(sb);return -1;}}
 sigs=Wait(sb->provider_sigmask|tmask|extra|sb->sigintr);prov_disarm(sb);
 expired=tmask?timer_cancel(sb):0;
 if(sigs&sb->sigintr){SetSignal(sigs&sb->sigintr,sigs&sb->sigintr);return fail(sb,AE_INTR);}
 if(sigs&extra){if(got_extra)*got_extra=sigs&extra;return 2;}
 if(expired)return 0;
 return (sigs&sb->provider_sigmask)?1:0;
}

LONG bsd_WaitSelect(struct SocketBase *sb,LONG nfds,APTR rf,APTR wf,APTR ef,struct timeval *tv,ULONG *signals)
{
 fd_set *r=(fd_set*)rf,*w=(fd_set*)wf,*e=(fd_set*)ef;ULONG items[FD_SETSIZE*2],rev[FD_SETSIZE],extra=signals?*signals:0,got=0;LONG map[FD_SETSIZE];UBYTE want[FD_SETSIZE];LONG n=0,fd,rc,ready=0;
 if(nfds<0)return fail(sb,AE_INVAL);
 if(nfds>FD_SETSIZE)nfds=FD_SETSIZE;
 for(fd=0;fd<nfds;fd++){ULONG ev=0;if(r&&FD_ISSET(fd,r))ev|=ACHS_POLLIN;if(w&&FD_ISSET(fd,w))ev|=ACHS_POLLOUT;if(e&&FD_ISSET(fd,e))ev|=ACHS_POLLPRI;if(!ev)continue;struct FD*f=fd_get(sb,fd);if(!f)return -1;map[n]=fd;want[n]=(UBYTE)ev;items[n*2]=f->handle;items[n*2+1]=ev;rev[n]=0;n++;}
 if(r)FD_ZERO(r);
 if(w)FD_ZERO(w);
 if(e)FD_ZERO(e);
 rc=wait_items(sb,items,rev,n,tv,extra,&got);if(signals)*signals=got;if(rc<=0)return rc;
 for(fd=0;fd<n;fd++){int any=0;if(r&&(want[fd]&ACHS_POLLIN)&&(rev[fd]&(ACHS_POLLIN|ACHS_POLLHUP|ACHS_POLLERR))){FD_SET(map[fd],r);any=1;}if(w&&(want[fd]&ACHS_POLLOUT)&&(rev[fd]&(ACHS_POLLOUT|ACHS_POLLERR))){FD_SET(map[fd],w);any=1;}if(e&&(want[fd]&ACHS_POLLPRI)&&(rev[fd]&(ACHS_POLLPRI|ACHS_POLLERR))){FD_SET(map[fd],e);any=1;}if(any)ready++;}
 return ready;
}
