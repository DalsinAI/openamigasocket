/* The few exec types card_parse.c uses, for building it on the build
 * machine (tests/host). Not the NDK's header. MIT. */
#ifndef EXEC_TYPES_H
#define EXEC_TYPES_H
#include <stdint.h>
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef int16_t WORD;
typedef uint16_t UWORD;
typedef uint8_t UBYTE;
typedef int8_t BYTE;
typedef void *APTR;
#endif
