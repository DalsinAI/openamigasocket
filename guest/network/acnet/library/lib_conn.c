/* ACNet bsdsocket.library: socket lifecycle and connection calls. MIT. */
#include <exec/types.h>
#include <proto/exec.h>
#include <sys/socket.h>
#include "lib_internal.h"

static LONG sockaddr_call(struct SocketBase *sb, LONG s, ULONG cmd, struct sockaddr *sa, socklen_t len, LONG a1)
{
    struct FD *f = fd_get(sb, s); if (!f) return -1;
    if (!sa || len < 8 || len > 255) return fail(sb, AE_INVAL);
    LONG r = prov_call(sb, cmd, (ULONG)f->handle, (ULONG)a1, 0, 0, sa, (ULONG)len, NULL, 0, NULL);
    if (r >= 0) return r;
    if (sb->last_err != AE_INPROGRESS || f->nonblock || cmd != ACHS_CMD_CONNECT) return fail_provider(sb);
    r = wait_ready(sb, f->handle, ACHS_POLLOUT, &f->sndtimeo);
    if (r <= 0) return r == 0 ? fail(sb, AE_TIMEDOUT) : -1;
    ULONG e = 0, n = 0;
    r = prov_call(sb, ACHS_CMD_GETSOCKOPT, (ULONG)f->handle, A_SOL_SOCKET, 0x1007, 4,
                  NULL, 0, &e, sizeof(e), &n);
    if (r < 0) return fail_provider(sb);
    if (e) return fail(sb, (LONG)e);
    return 0;
}

LONG bsd_socket(struct SocketBase *sb, LONG domain, LONG type, LONG protocol)
{
    LONG h = prov_call(sb, ACHS_CMD_SOCKET, domain, type, protocol, 0, NULL, 0, NULL, 0, NULL);
    if (h < 0) return fail_provider(sb);
    LONG s = fd_alloc(sb, h, (UBYTE)type, -1);
    if (s < 0) { prov_call(sb, ACHS_CMD_CLOSE, h, 0, 0, 0, NULL, 0, NULL, 0, NULL); return -1; }
    handle_ref(sb, h); return s;
}

LONG bsd_bind(struct SocketBase *sb, LONG s, struct sockaddr *name, socklen_t len) { return sockaddr_call(sb,s,ACHS_CMD_BIND,name,len,0); }
LONG bsd_connect(struct SocketBase *sb, LONG s, struct sockaddr *name, socklen_t len) { return sockaddr_call(sb,s,ACHS_CMD_CONNECT,name,len,0); }

LONG bsd_listen(struct SocketBase *sb, LONG s, LONG backlog)
{
    struct FD *f=fd_get(sb,s); if(!f) return -1;
    LONG r=prov_call(sb,ACHS_CMD_LISTEN,f->handle,backlog,0,0,NULL,0,NULL,0,NULL);
    return r<0?fail_provider(sb):r;
}

LONG bsd_accept(struct SocketBase *sb, LONG s, struct sockaddr *addr, socklen_t *addrlen)
{
    struct FD *f=fd_get(sb,s); UBYTE rx[16]; ULONG n=0; LONG h;
    if(!f) return -1;
    for (;;) {
        h=prov_call(sb,ACHS_CMD_ACCEPT,f->handle,0,0,0,NULL,0,rx,sizeof(rx),&n);
        if(h>=0) break;
        if(sb->last_err!=AE_AGAIN || f->nonblock) return fail_provider(sb);
        LONG w=wait_ready(sb,f->handle,ACHS_POLLIN,&f->rcvtimeo);
        if(w<=0) return w==0?fail(sb,AE_TIMEDOUT):-1;
    }
    LONG ns=fd_alloc(sb,h,f->type,-1); if(ns<0){prov_call(sb,ACHS_CMD_CLOSE,h,0,0,0,NULL,0,NULL,0,NULL);return -1;}
    handle_ref(sb, h);
    if(addr && addrlen){ULONG m=*addrlen<n?*addrlen:n; CopyMem(rx,addr,m); *addrlen=(socklen_t)n;}
    return ns;
}

LONG bsd_shutdown(struct SocketBase *sb, LONG s, LONG how)
{
    struct FD *f=fd_get(sb,s); if(!f)return -1;
    LONG r=prov_call(sb,ACHS_CMD_SHUTDOWN,f->handle,how,0,0,NULL,0,NULL,0,NULL);
    return r<0?fail_provider(sb):r;
}
