#ifndef NETDB_H
#define NETDB_H
/*
 * OpenSocket SDK: host, network, service and protocol names, and address lookups.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifndef NETINET_IN_H
#include <netinet/in.h>
#endif
#define MAXHOSTNAMELEN          256

/* a host's names and addresses */
struct hostent
{
    char *h_name;			/* its official name */
    char **h_aliases;		/* other names, NULL at the end */
    LONG h_addrtype;		/* AF_INET */
    LONG h_length;			/* the length of each address */
    char **h_addr_list;		/* its addresses, NULL at the end */
};
#define h_addr			h_addr_list[0]

/* a network's names and number */
struct netent
{
    char *n_name;
    char **n_aliases;
    LONG n_addrtype;
    in_addr_t n_net;
};

/* a service's names, port and protocol */
struct servent
{
    char *s_name;
    char **s_aliases;
    LONG s_port;			/* network byte order */
    char *s_proto;
};

/* a protocol's names and number */
struct protoent
{
    char *p_name;
    char **p_aliases;
    LONG p_proto;
};

/* getaddrinfo()'s hints and answers */
struct addrinfo
{
    int ai_flags;			/* AI_ */
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
};

/* h_errno's values */
#define NETDB_INTERNAL          (-1)
#define NETDB_SUCCESS           0
#define HOST_NOT_FOUND          1
#define TRY_AGAIN               2
#define NO_RECOVERY             3
#define NO_DATA                 4
#define NO_ADDRESS              4

#define AI_PASSIVE              0x1
#define AI_CANONNAME            0x2
#define AI_NUMERICHOST          0x4
#define AI_EXT                  0x8
#define AI_NUMERICSERV          0x10
#define AI_MASK                 0x17

#define NI_NUMERICHOST          0x1
#define NI_NUMERICSERV          0x2
#define NI_NOFQDN               0x4
#define NI_NAMEREQD             0x8
#define NI_DGRAM                0x10
#define NI_WITHSCOPEID          0x20
#define NI_MAXHOST              256
#define NI_MAXSERV              32

/* getaddrinfo()'s and getnameinfo()'s errors */
#define EAI_BADFLAGS            (-1)
#define EAI_NONAME              (-2)
#define EAI_AGAIN               (-3)
#define EAI_FAIL                (-4)
#define EAI_NODATA              (-5)
#define EAI_FAMILY              (-6)
#define EAI_SOCKTYPE            (-7)
#define EAI_SERVICE             (-8)
#define EAI_ADDRFAMILY          (-9)
#define EAI_MEMORY              (-10)
#define EAI_SYSTEM              (-11)
#define EAI_BADHINTS            (-12)
#define EAI_PROTOCOL            (-13)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NO_NET_API
struct hostent *(gethostbyaddr)(const void *addr, socklen_t len, int type);
struct hostent *(gethostbyname)(const char *name);
struct netent *(getnetbyaddr)(in_addr_t net, int type);
struct netent *(getnetbyname)(const char *name);
struct protoent *(getprotobyname)(const char *name);
struct protoent *(getprotobynumber)(int proto);
struct servent *(getservbyname)(const char *name, const char *proto);
struct servent *(getservbyport)(int port, const char *proto);
const char *(hstrerror)(int err);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NETDB_H */
