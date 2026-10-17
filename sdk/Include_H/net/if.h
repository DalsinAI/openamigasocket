#ifndef NET_IF_H
#define NET_IF_H
/*
 * OpenSocket SDK: network interfaces: their flags and the ioctl requests about them.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef SYS_SOCKET_H
#include <sys/socket.h>
#endif

#define IFF_UP                  0x1
#define IFF_BROADCAST           0x2
#define IFF_DEBUG               0x4
#define IFF_LOOPBACK            0x8
#define IFF_POINTOPOINT         0x10
#define IFF_NOTRAILERS          0x20
#define IFF_RUNNING             0x40
#define IFF_NOARP               0x80
#define IFF_PROMISC             0x100
#define IFF_ALLMULTI            0x200
#define IFF_OACTIVE             0x400
#define IFF_SIMPLEX             0x800
#define IFF_LINK0               0x1000
#define IFF_LINK1               0x2000
#define IFF_LINK2               0x4000
#define IFF_MULTICAST           0x8000
#define IFF_CANTCHANGE          0x8E52

#define IFNAMSIZ                16

/* one interface request: its name, then what it asks about */
struct ifreq
{
    char ifr_name[IFNAMSIZ];
    union
    {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        WORD ifru_flags;
        LONG ifru_metric;
        APTR ifru_data;
    } ifr_ifru;
};
#define ifr_addr		ifr_ifru.ifru_addr
#define ifr_dstaddr		ifr_ifru.ifru_dstaddr
#define ifr_broadaddr		ifr_ifru.ifru_broadaddr
#define ifr_flags		ifr_ifru.ifru_flags
#define ifr_metric		ifr_ifru.ifru_metric
#define ifr_data		ifr_ifru.ifru_data

/* SIOCAIFADDR's argument: an address, its broadcast address and mask */
struct ifaliasreq
{
    char ifra_name[IFNAMSIZ];
    struct sockaddr ifra_addr;
    struct sockaddr ifra_broadaddr;
    struct sockaddr ifra_mask;
};

/* SIOCGIFCONF's argument: a buffer the interfaces' requests go into */
struct ifconf
{
    LONG ifc_len;
    union
    {
        APTR ifcu_buf;
        struct ifreq *ifcu_req;
    } ifc_ifcu;
};
#define ifc_buf			ifc_ifcu.ifcu_buf
#define ifc_req			ifc_ifcu.ifcu_req

#endif /* NET_IF_H */
