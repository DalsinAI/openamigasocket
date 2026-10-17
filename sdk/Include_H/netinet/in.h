#ifndef NETINET_IN_H
#define NETINET_IN_H
/*
 * OpenSocket SDK: the Internet protocol family: addresses, ports, options.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef SYS_SOCKET_H
#include <sys/socket.h>
#endif

#define IPPROTO_IP              0
#define IPPROTO_ICMP            1
#define IPPROTO_IGMP            2
#define IPPROTO_GGP             3
#define IPPROTO_TCP             6
#define IPPROTO_EGP             8
#define IPPROTO_PUP             12
#define IPPROTO_UDP             17
#define IPPROTO_IDP             22
#define IPPROTO_TP              29
#define IPPROTO_EON             80
#define IPPROTO_ENCAP           98
#define IPPROTO_RAW             255
#define IPPROTO_MAX             256

/* ports below IPPORT_RESERVED are the system's */
#define IPPORT_RESERVED         1024
#define IPPORT_USERRESERVED     5000

/* an IPv4 address, in network byte order */
struct in_addr
{
    in_addr_t s_addr;
};

/* the old classful split of an address */
#define IN_CLASSA(i)		(((ULONG)(i) & 0x80000000UL) == 0)
#define IN_CLASSA_NET           0xFF000000UL
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0xFFFFFF
#define IN_CLASSA_MAX           128
#define IN_CLASSB(i)		(((ULONG)(i) & 0xC0000000UL) == 0x80000000UL)
#define IN_CLASSB_NET           0xFFFF0000UL
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          65535
#define IN_CLASSB_MAX           0x10000
#define IN_CLASSC(i)		(((ULONG)(i) & 0xE0000000UL) == 0xC0000000UL)
#define IN_CLASSC_NET           0xFFFFFF00UL
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          255
#define IN_CLASSD(i)		(((ULONG)(i) & 0xF0000000UL) == 0xE0000000UL)
#define IN_CLASSD_NET           0xF0000000UL
#define IN_CLASSD_NSHIFT        28
#define IN_CLASSD_HOST          0xFFFFFFF
#define IN_MULTICAST(i)		IN_CLASSD(i)
#define IN_EXPERIMENTAL(i)	(((ULONG)(i) & 0xF0000000UL) == 0xF0000000UL)
#define IN_BADCLASS(i)		IN_EXPERIMENTAL(i)

#define INADDR_ANY              0
#define INADDR_BROADCAST        0xFFFFFFFFUL
#define INADDR_NONE             0xFFFFFFFFUL
#define INADDR_UNSPEC_GROUP     0xE0000000UL
#define INADDR_ALLHOSTS_GROUP   0xE0000001UL
#define INADDR_MAX_LOCAL_GROUP  0xE00000FFUL
#define IN_LOOPBACKNET          127

/* an IPv4 socket address */
struct sockaddr_in
{
    UBYTE sin_len;
    sa_family_t sin_family;
    in_port_t sin_port;		/* network byte order */
    struct in_addr sin_addr;
    UBYTE sin_zero[8];
};

/* an IPv6 address and socket address */
struct in6_addr
{
    unsigned char s6_addr[16];
};

struct sockaddr_in6
{
    sa_family_t sin6_family;
    in_port_t sin6_port;
    ULONG sin6_flowinfo;
    struct in6_addr sin6_addr;
    ULONG sin6_scope_id;
};

/* options, at level IPPROTO_IP */
#define IP_OPTIONS              1
#define IP_HDRINCL              2
#define IP_TOS                  3
#define IP_TTL                  4
#define IP_RECVOPTS             5
#define IP_RECVRETOPTS          6
#define IP_RECVDSTADDR          7
#define IP_RETOPTS              8
#define IP_MULTICAST_IF         9
#define IP_MULTICAST_TTL        10
#define IP_MULTICAST_LOOP       11
#define IP_ADD_MEMBERSHIP       12
#define IP_DROP_MEMBERSHIP      13
#define IP_DEFAULT_MULTICAST_TTL 1
#define IP_DEFAULT_MULTICAST_LOOP 1
#define IP_MAX_MEMBERSHIPS      20

/* IP_ADD_MEMBERSHIP's and IP_DROP_MEMBERSHIP's argument */
struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

/* the sysctl names */
#define IPPROTO_MAXID           23
#define IPCTL_FORWARDING        1
#define IPCTL_SENDREDIRECTS     2
#define IPCTL_DEFTTL            3
#define IPCTL_MAXID             5

/* the 68k is big-endian: network byte order is its own */
#define ntohl(x)		(x)
#define ntohs(x)		(x)
#define htonl(x)		(x)
#define htons(x)		(x)
#define NTOHL(x)		(x)
#define NTOHS(x)		(x)
#define HTONL(x)		(x)
#define HTONS(x)		(x)

#endif /* NETINET_IN_H */
