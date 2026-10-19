#ifndef NETINET_TCP_H
#define NETINET_TCP_H
/*
 * OpenSocket SDK: TCP's header flags, options and socket options.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#define TH_FIN                  0x1
#define TH_SYN                  0x2
#define TH_RST                  0x4
#define TH_PUSH                 0x8
#define TH_ACK                  0x10
#define TH_URG                  0x20

#define TCPOPT_EOL              0
#define TCPOPT_NOP              1
#define TCPOPT_MAXSEG           2
#define TCPOLEN_MAXSEG          4
#define TCPOPT_WINDOW           3
#define TCPOLEN_WINDOW          3
#define TCPOPT_SACK_PERMITTED   4
#define TCPOLEN_SACK_PERMITTED  2
#define TCPOPT_SACK             5
#define TCPOPT_TIMESTAMP        8
#define TCPOLEN_TIMESTAMP       10
#define TCPOLEN_TSTAMP_APPA     12
/* NOP, NOP, then the timestamp option's kind and length */
#define TCPOPT_TSTAMP_HDR       0x101080A

#define TCP_MSS                 512
#define TCP_MAXWIN              65535
#define TCP_MAX_WINSHIFT        14

/* options, at level IPPROTO_TCP */
#define TCP_NODELAY             0x1
#define TCP_MAXSEG              0x2

#endif /* NETINET_TCP_H */
