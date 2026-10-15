/* bsdsocket.library's Direct backend: OpenSocketDirect, OpenSocket's own
 * TCP/IP stack over the SANA-II device the card layer chose (card.c). Not
 * built yet: until it is, the library still opens, and every call answers
 * ENOSYS ("Function not implemented") at once, so a program reports it and
 * nothing waits. MIT. */
#include <exec/types.h>
#include <proto/exec.h>

#include "provider.h"

#define ENOSYS 78

LONG direct_open(struct SocketBase *sb) { (void)sb; return 0; }

void direct_close(struct SocketBase *sb) { (void)sb; }

LONG direct_call(struct SocketBase *sb, ULONG cmd, ULONG a0, ULONG a1, ULONG a2, ULONG a3,
                 const void *tx, ULONG txlen, void *rx, ULONG rxmax, ULONG *rxlen)
{
    (void)cmd; (void)a0; (void)a1; (void)a2; (void)a3; (void)tx; (void)txlen; (void)rx; (void)rxmax;
    sb->last_err = ENOSYS;
    if (rxlen) *rxlen = 0;
    return -1;
}

void direct_arm(struct SocketBase *sb) { (void)sb; }

void direct_disarm(struct SocketBase *sb) { (void)sb; }
