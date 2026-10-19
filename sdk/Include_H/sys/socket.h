#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H
/*
 * OpenSocket SDK: sockets, their options, addresses and messages.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef SYS_UIO_H
#include <sys/uio.h>
#endif
#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif
/* the types the socket headers share: the C library's where it has
 * them, else these, behind the same guards */
#if defined(__has_include)
#if __has_include(<sys/types.h>)
#include <sys/types.h>
#endif
#endif
#ifndef _SIZE_T_DEFINED_OPENSOCKET
#define _SIZE_T_DEFINED_OPENSOCKET
#include <stddef.h>
#endif
#ifndef _SSIZE_T_DECLARED
#define _SSIZE_T_DECLARED
typedef int ssize_t;
#endif
#ifndef _IN_ADDR_T_DECLARED
#define _IN_ADDR_T_DECLARED
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ in_addr_t;
#else
typedef ULONG in_addr_t;
#endif
#endif
#ifndef _IN_PORT_T_DECLARED
#define _IN_PORT_T_DECLARED
typedef UWORD in_port_t;
#endif
#ifndef _SA_FAMILY_T_DECLARED
#define _SA_FAMILY_T_DECLARED
typedef UBYTE sa_family_t;
#endif
#ifndef _SOCKLEN_T_DECLARED
#define _SOCKLEN_T_DECLARED
typedef unsigned long socklen_t;
#endif

/* socket types */
#define SOCK_STREAM             1
#define SOCK_DGRAM              2
#define SOCK_RAW                3
#define SOCK_RDM                4
#define SOCK_SEQPACKET          5

/* options, at level SOL_SOCKET */
#define SO_DEBUG                0x1
#define SO_ACCEPTCONN           0x2
#define SO_REUSEADDR            0x4
#define SO_KEEPALIVE            0x8
#define SO_DONTROUTE            0x10
#define SO_BROADCAST            0x20
#define SO_USELOOPBACK          0x40
#define SO_LINGER               0x80
#define SO_OOBINLINE            0x100
#define SO_REUSEPORT            0x200
#define SO_SNDBUF               0x1001
#define SO_RCVBUF               0x1002
#define SO_SNDLOWAT             0x1003
#define SO_RCVLOWAT             0x1004
#define SO_SNDTIMEO             0x1005
#define SO_RCVTIMEO             0x1006
#define SO_ERROR                0x1007
#define SO_TYPE                 0x1008
#define SO_EVENTMASK            0x2001
#define SOL_SOCKET              0xFFFF

/* SO_LINGER's argument */
struct linger
{
    LONG l_onoff;
    LONG l_linger;			/* in seconds */
};

/* address families; the protocol families have the same numbers */
#define AF_UNSPEC               0
#define AF_LOCAL                1
#define AF_UNIX                 1
#define AF_INET                 2
#define AF_IMPLINK              3
#define AF_PUP                  4
#define AF_CHAOS                5
#define AF_NS                   6
#define AF_ISO                  7
#define AF_OSI                  7
#define AF_ECMA                 8
#define AF_DATAKIT              9
#define AF_CCITT                10
#define AF_SNA                  11
#define AF_DECnet               12
#define AF_DLI                  13
#define AF_LAT                  14
#define AF_HYLINK               15
#define AF_APPLETALK            16
#define AF_ROUTE                17
#define AF_LINK                 18
#define pseudo_AF_XTP           19
#define AF_COIP                 20
#define AF_CNT                  21
#define pseudo_AF_RTIP          22
#define AF_IPX                  23
#define AF_INET6                23
#define AF_SIP                  24
#define pseudo_AF_PIP           25
#define AF_MAX                  26

#define PF_UNSPEC		AF_UNSPEC
#define PF_LOCAL		AF_LOCAL
#define PF_UNIX			AF_UNIX
#define PF_INET			AF_INET
#define PF_IMPLINK		AF_IMPLINK
#define PF_PUP			AF_PUP
#define PF_CHAOS		AF_CHAOS
#define PF_NS			AF_NS
#define PF_ISO			AF_ISO
#define PF_OSI			AF_OSI
#define PF_ECMA			AF_ECMA
#define PF_DATAKIT		AF_DATAKIT
#define PF_CCITT		AF_CCITT
#define PF_SNA			AF_SNA
#define PF_DECnet		AF_DECnet
#define PF_DLI			AF_DLI
#define PF_LAT			AF_LAT
#define PF_HYLINK		AF_HYLINK
#define PF_APPLETALK		AF_APPLETALK
#define PF_ROUTE		AF_ROUTE
#define PF_LINK			AF_LINK
#define PF_XTP			pseudo_AF_XTP
#define PF_COIP			AF_COIP
#define PF_CNT			AF_CNT
#define PF_SIP			AF_SIP
#define PF_IPX			AF_IPX
#define PF_RTIP			pseudo_AF_RTIP
#define PF_PIP			pseudo_AF_PIP
#define PF_MAX			AF_MAX

/* an address: its length, its family, then the family's bytes */
struct sockaddr
{
    UBYTE sa_len;
    sa_family_t sa_family;
    char sa_data[14];
};

/* the protocol a raw socket uses */
struct sockproto
{
    UWORD sp_family;
    UWORD sp_protocol;
};

/* the routing table's sysctl names */
#define NET_MAXID               26
#define NET_RT_DUMP             1
#define NET_RT_FLAGS            2
#define NET_RT_IFLIST           3
#define NET_RT_MAXID            4

/* the longest queue of pending connections listen() accepts */
#define SOMAXCONN               5

/* a message for sendmsg() and recvmsg() */
struct msghdr
{
    APTR msg_name;			/* the address, or NULL */
    socklen_t msg_namelen;
    struct iovec *msg_iov;		/* the scattered data */
    ULONG msg_iovlen;
    APTR msg_control;		/* ancillary data: cmsghdr entries */
    socklen_t msg_controllen;
    LONG msg_flags;
};

#define MSG_OOB                 0x1
#define MSG_PEEK                0x2
#define MSG_DONTROUTE           0x4
#define MSG_EOR                 0x8
#define MSG_TRUNC               0x10
#define MSG_CTRUNC              0x20
#define MSG_WAITALL             0x40
#define MSG_DONTWAIT            0x80

/* one entry of ancillary data; the data follows it */
struct cmsghdr
{
    socklen_t cmsg_len;		/* the entry's length, data included */
    LONG cmsg_level;
    LONG cmsg_type;
};

/* the entries are 4-byte aligned */
#define __OPENSOCKET_CMSG_ALIGN(n)	(((ULONG)(n) + 3) & ~(ULONG)3)
#define CMSG_DATA(cmsg)		((UBYTE *)((cmsg) + 1))
#define CMSG_FIRSTHDR(mhdr)	((struct cmsghdr *)(mhdr)->msg_control)
#define CMSG_NXTHDR(mhdr, cmsg)	\
    (((UBYTE *)(cmsg) + __OPENSOCKET_CMSG_ALIGN((cmsg)->cmsg_len) + __OPENSOCKET_CMSG_ALIGN(sizeof(struct cmsghdr)) > \
      (UBYTE *)(mhdr)->msg_control + (mhdr)->msg_controllen) ? (struct cmsghdr *)NULL : \
     (struct cmsghdr *)((UBYTE *)(cmsg) + __OPENSOCKET_CMSG_ALIGN((cmsg)->cmsg_len)))
#define SCM_RIGHTS              1

/* shutdown()'s how */
#define SHUT_RD                 0
#define SHUT_WR                 1
#define SHUT_RDWR               2

/* the sets WaitSelect() takes, as the C library's sys/select.h has them */
#ifndef _SYS_TYPES_FD_SET
#define _SYS_TYPES_FD_SET
#ifndef FD_SETSIZE
#define FD_SETSIZE              64
#endif
#ifndef NBBY
#define NBBY			8
#endif
#define NFDBITS                 32
#ifndef howmany
#define howmany(x, y)		(((x) + ((y) - 1)) / (y))
#endif
typedef ULONG fd_mask;
typedef struct fd_set
{
    fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;
#define FD_SET(n, p)		((p)->fds_bits[(n) / NFDBITS] |= (1UL << ((n) % NFDBITS)))
#define FD_CLR(n, p)		((p)->fds_bits[(n) / NFDBITS] &= ~(1UL << ((n) % NFDBITS)))
#define FD_ISSET(n, p)		((p)->fds_bits[(n) / NFDBITS] & (1UL << ((n) % NFDBITS)))
#define FD_COPY(f, t)		(*(t) = *(f))
#define FD_ZERO(p)		do { ULONG __fdz; for (__fdz = 0; __fdz < howmany(FD_SETSIZE, NFDBITS); __fdz++) (p)->fds_bits[__fdz] = 0; } while (0)
#else
#ifndef howmany
#define howmany(x, y)		(((x) + ((y) - 1)) / (y))
#endif
#ifndef FD_COPY
#define FD_COPY(f, t)		(*(t) = *(f))
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NO_NET_API
/* the BSD calls; the names in parentheses so that the inline calls of
 * proto/bsdsocket.h, when they come first, are not expanded here */
int (accept)(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
int (bind)(int socket, const struct sockaddr *address, socklen_t address_len);
int (connect)(int socket, const struct sockaddr *address, socklen_t address_len);
int (getpeername)(int socket, struct sockaddr *address, socklen_t *address_len);
int (getsockname)(int socket, struct sockaddr *address, socklen_t *address_len);
int (getsockopt)(int socket, int level, int option_name, void *option_value, socklen_t *option_len);
int (listen)(int socket, int backlog);
ssize_t (recv)(int socket, void *buffer, size_t length, int flags);
ssize_t (recvfrom)(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len);
ssize_t (recvmsg)(int socket, struct msghdr *message, int flags);
ssize_t (send)(int socket, const void *buffer, size_t length, int flags);
ssize_t (sendmsg)(int socket, const struct msghdr *message, int flags);
ssize_t (sendto)(int socket, const void *message, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
int (setsockopt)(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int (shutdown)(int socket, int how);
int (socket)(int domain, int type, int protocol);
#endif

#ifdef __cplusplus
}
#endif

#endif /* SYS_SOCKET_H */
