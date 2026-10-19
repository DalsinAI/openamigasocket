#ifndef SYS_FILIO_H
#define SYS_FILIO_H
/*
 * OpenSocket SDK: the ioctl commands every descriptor answers.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef SYS_IOCCOM_H
#include <sys/ioccom.h>
#endif

#define FIOCLEX			_IO('f', 1)
#define FIONCLEX		_IO('f', 2)
#define FIONREAD		_IOR('f', 127, LONG)	/* bytes waiting */
#define FIONBIO			_IOW('f', 126, LONG)	/* non-blocking on or off */
#define FIOASYNC		_IOW('f', 125, LONG)
#define FIOSETOWN		_IOW('f', 124, LONG)
#define FIOGETOWN		_IOR('f', 123, LONG)

#endif /* SYS_FILIO_H */
