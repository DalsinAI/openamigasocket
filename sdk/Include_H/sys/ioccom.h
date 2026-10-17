#ifndef SYS_IOCCOM_H
#define SYS_IOCCOM_H
/*
 * OpenSocket SDK: how an ioctl command is made: direction, size, group, number.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define IOCPARM_MASK		0x1fff		/* the argument's size, up to 8 KB */
#define IOCPARM_LEN(x)		(((x) >> 16) & IOCPARM_MASK)
#define IOCBASECMD(x)		((x) & ~(IOCPARM_MASK << 16))
#define IOCGROUP(x)		(((x) >> 8) & 0xff)
#define IOCPARM_MAX		4096
#define IOC_VOID		0x20000000UL	/* no argument */
#define IOC_OUT			0x40000000UL	/* an argument coming back */
#define IOC_IN			0x80000000UL	/* an argument going in */
#define IOC_INOUT		(IOC_IN | IOC_OUT)
#define IOC_DIRMASK		0xe0000000UL
#define _IOC(inout, group, num, len) \
    ((ULONG)((inout) | (((ULONG)(len) & IOCPARM_MASK) << 16) | ((group) << 8) | (num)))
#define _IO(g, n)		_IOC(IOC_VOID, (g), (n), 0)
#define _IOR(g, n, t)		_IOC(IOC_OUT, (g), (n), sizeof(t))
#define _IOW(g, n, t)		_IOC(IOC_IN, (g), (n), sizeof(t))
#define _IOWR(g, n, t)		_IOC(IOC_INOUT, (g), (n), sizeof(t))

#endif /* SYS_IOCCOM_H */
