#ifndef SYS_UIO_H
#define SYS_UIO_H
/*
 * OpenSocket SDK: a piece of a scattered buffer.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef _OPENSOCKET_STDDEF
#define _OPENSOCKET_STDDEF
#include <stddef.h>
#endif

struct iovec
{
    APTR iov_base;
    size_t iov_len;
};

#endif /* SYS_UIO_H */
