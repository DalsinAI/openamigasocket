/* OpenSocketDirect: lwIP's compiler and platform layer for the 68k (big
 * endian), built bare with the os32-gcc16 stove (no C library). MIT. */
#ifndef OPENSOCKET_ARCH_CC_H
#define OPENSOCKET_ARCH_CC_H

#define BYTE_ORDER BIG_ENDIAN
#define LWIP_NO_INTTYPES_H 1
#define LWIP_NO_CTYPE_H    1
#define X8_F  "02x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define SZT_F "lu"

#define LWIP_PLATFORM_DIAG(x)   do { } while (0)
#define LWIP_PLATFORM_ASSERT(x) do { } while (0)
#define LWIP_NO_UNISTD_H 1
#define SSIZE_MAX 0x7fffffffL
typedef long ssize_t;

#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#endif
