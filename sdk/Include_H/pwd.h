#ifndef PWD_H
#define PWD_H
/*
 * OpenSocket SDK: users, as the user database describes them.
 * Copyright (c) 2026 DalsinAI. MIT licence (see LICENSE).
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#if defined(__has_include)
#if __has_include(<sys/types.h>)
#include <sys/types.h>
#endif
#endif
#ifndef _UID_T_DECLARED
#define _UID_T_DECLARED
typedef int uid_t;
#endif
#ifndef _TIME_T_DECLARED
#ifndef __time_t_defined
#define _TIME_T_DECLARED
typedef long time_t;
#endif
#endif

struct passwd
{
    char *pw_name;
    char *pw_passwd;		/* encrypted */
    int pw_uid;
    int pw_gid;
    time_t pw_change;		/* when the password must change */
    char *pw_class;
    char *pw_gecos;			/* the user's full name */
    char *pw_dir;			/* the home directory */
    char *pw_shell;
    time_t pw_expire;		/* when the account expires */
};

#ifdef __cplusplus
extern "C" {
#endif

struct passwd *(getpwuid)(uid_t uid);
struct passwd *(getpwnam)(const char *name);
struct passwd *(getpwent)(void);
int (setpassent)(int stayopen);
int (setpwent)(void);
void (endpwent)(void);

#ifdef __cplusplus
}
#endif

#endif /* PWD_H */
