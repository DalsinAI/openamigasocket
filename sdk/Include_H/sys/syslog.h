#ifndef SYS_SYSLOG_H
#define SYS_SYSLOG_H
/*
 * OpenSocket SDK: system log priorities, facilities and options.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef _OPENSOCKET_STDARG
#define _OPENSOCKET_STDARG
#include <stdarg.h>
#endif

/* priorities, most urgent first */
#define LOG_EMERG               0
#define LOG_ALERT               1
#define LOG_CRIT                2
#define LOG_ERR                 3
#define LOG_WARNING             4
#define LOG_NOTICE              5
#define LOG_INFO                6
#define LOG_DEBUG               7
#define LOG_PRIMASK             7

#define LOG_PRI(p)		((p) & LOG_PRIMASK)
#define LOG_MAKEPRI(fac, pri)	(((fac) << 3) | (pri))

/* facilities, in the bits above the priority */
#define LOG_KERN                0
#define LOG_USER                8
#define LOG_MAIL                16
#define LOG_DAEMON              24
#define LOG_AUTH                32
#define LOG_SYSLOG              40
#define LOG_LPR                 48
#define LOG_NEWS                56
#define LOG_UUCP                64
#define LOG_CRON                72
#define LOG_AUTHPRIV            80
#define LOG_FTP                 88
#define LOG_LOCAL0              128
#define LOG_LOCAL1              136
#define LOG_LOCAL2              144
#define LOG_LOCAL3              152
#define LOG_LOCAL4              160
#define LOG_LOCAL5              168
#define LOG_LOCAL6              176
#define LOG_LOCAL7              184
#define LOG_NFACILITIES         24
#define LOG_FACMASK             1016

#define LOG_FAC(p)		(((p) & LOG_FACMASK) >> 3)

/* setlogmask()'s masks */
#define LOG_MASK(pri)		(1 << (pri))
#define LOG_UPTO(pri)		((1 << ((pri) + 1)) - 1)

/* openlog()'s options */
#define LOG_PID                 1
#define LOG_CONS                2
#define LOG_ODELAY              4
#define LOG_NDELAY              8
#define LOG_NOWAIT              16
#define LOG_PERROR              32

#ifdef __cplusplus
extern "C" {
#endif

void (closelog)(void);
void (openlog)(const char *ident, int option, int facility);
int (setlogmask)(int mask);
void (syslog)(int priority, const char *message, ...);
void (vsyslog)(int priority, const char *message, va_list args);

#ifdef __cplusplus
}
#endif

#endif /* SYS_SYSLOG_H */
