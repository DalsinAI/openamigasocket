#ifndef ARPA_INET_H
#define ARPA_INET_H
/*
 * OpenSocket SDK: converting Internet addresses to and from text.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef NETINET_IN_H
#include <netinet/in.h>
#endif
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK         0x7F000001
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NO_NET_API
in_addr_t (inet_addr)(const char *cp);
int (inet_aton)(const char *cp, struct in_addr *pin);
in_addr_t (inet_lnaof)(struct in_addr in);
struct in_addr (inet_makeaddr)(in_addr_t net, in_addr_t lna);
in_addr_t (inet_netof)(struct in_addr in);
in_addr_t (inet_network)(const char *cp);
char *(inet_ntoa)(struct in_addr in);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ARPA_INET_H */
