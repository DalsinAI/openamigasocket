#ifndef SYS_IOCTL_H
#define SYS_IOCTL_H
/*
 * OpenSocket SDK: ioctl() and the commands sockets answer.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef SYS_IOCCOM_H
#include <sys/ioccom.h>
#endif
#ifndef SYS_FILIO_H
#include <sys/filio.h>
#endif
#ifndef SYS_SOCKIO_H
#include <sys/sockio.h>
#endif

/* a terminal's size in characters */
struct ttysize
{
    UWORD ts_lines;
    UWORD ts_cols;
    UWORD ts_xxx;
    UWORD ts_yyy;
};
#define TIOCGSIZE		_IOR('t', 104, struct ttysize)
#define TIOCSSIZE		_IOW('t', 103, struct ttysize)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NO_NET_API
int (ioctl)(int fd, unsigned long request, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif /* SYS_IOCTL_H */
