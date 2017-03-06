#ifndef __BS_COMMON_H_
#define __BS_COMMON_H_

#ifdef __LINUX__
#include <iconv.h>
#endif
#include "bs_type.h"
#include "bs_error.h"

#define bs_max(a, b) ((a)>=(b) ? (a):(b))
#define bs_min(a, b) ((a) < (b) ? (a):(b))
#define bs_subtract_abs(a, b)   ((a)>=(b) ? ((a)-(b)):((b)-(a)))

#define BS_SET_TIMEVAL(tm, usec)   do{ \
(tm)->tv_sec = usec/1000000;        \
(tm)->tv_usec = usec%1000000;       \
}while(0)

#define BS_USEC_INTERVAL(tm1, tm2)      (((tm1)->tv_sec-(tm2)->tv_sec)*1000000+((tm1)->tv_usec-(tm2)->tv_usec))

#define bs_is_num(code)         ('0'<=(code) && (code)<='9')
#define bs_is_lowletter(code)   ('a'<=(code) && (code)<='z')
#define bs_is_highletter(code)  ('A'<=(code) && (code)<='Z')
#define bs_is_letter(code)      (bs_is_lowletter(code)||bs_is_highletter(code))
#define bs_is_char(code)        (bs_is_num(code) || bs_is_letter(code))
#define bs_is_space(code)       ((code)==' ' || (code)=='\t' || (code)=='\n')

// 判断是否为2的幂
#define bs_is_powerof2(i)       (((i)&((i)-1))==0)

/// flag
typedef uint32_t				flag_t;
#define	BS_SCLb(k)					(31 & (k))
#define	BS_BTST(x, b)				(((x) >> (b)) & 1)
#define	BS_BSETMASK(b)				((b) < 32 ? (1u << BS_SCLb(b)) : 0)
#define	BS_BSET(x, b)				do{	(x) |= BS_BSETMASK(b); }while(0)
#define	BS_BCLRMASK(b)				(~(BS_BSETMASK(b)))
#define	BS_BCLR(x, b)				do{	(x) &= BS_BCLRMASK(b); }while(0)
#define	BS_NSETMASK(msb, lsb)		(BS_BSETMASK((msb) + 1) - BS_BSETMASK(lsb))
#define	BS_NCLRMASK(msb, lsb)		(~(BS_NSETMASK(msb, lsb)))

typedef uint32_t				flag_t;
#define FLAG_MAXNUMBER			0xffffffff

#define FLAG_MAXBIT				32
typedef	uint32_t 				flags_t[(FLAG_MAXBIT+sizeof(uint32_t)-1)/sizeof(uint32_t)];
#define FLAGS_BITSIZE			FLAG_MAXBIT
#define FLAGS_UINTSIZE(f)		(sizeof(f)/sizeof(uint32_t))
#define FLAGS_BYTESIZE(f)		sizeof(f)

typedef uint32_t	flag_t;
#define FLAG_CLR(f)				(f=0)
#define FLAG_SET(f, i)			(f |= (1<<(i)))
#define FLAG_UNSET(f, i)		(f &= (~(1<<(i))))
#define FLAG_MASK(i)			(0xffffffff>>(32-(i)-1))
#define FLAG_CHK(f, i)			((f)&FLAG_MASK(i) == FLAG_MASK(i))


#ifdef __cplusplus
extern "C"{
#endif

uint64_t atou64(const char* str);
uint32_t bs_strcpy(void* src, uint32_t src_size, const void* dst);
uint32_t bs_memcpy(void* src, uint32_t src_size, const void* dst, uint32_t dst_size);
void bs_strswap(void* src, uint32_t src_size, char* dst, uint32_t dst_size);
char* bs_strrstr(const char* src, const char* dst);

#ifndef COMPILE_MAC
state_t convert(const char *fromset, const char *toset, char *from,uint32_t from_len, char *to,uint32_t to_len);
#endif

#ifdef __cplusplus
}
#endif

#endif
