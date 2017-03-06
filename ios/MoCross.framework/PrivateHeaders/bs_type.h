/**
 * @file bs_type.h
 * @author bushaofeng(com@baidu.com)
 * @date 2011/11/21 16:31:25
 * @brief 
 *  
 **/




#ifndef  __BS_TYPE_H_
#define  __BS_TYPE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <net/if.h>
//#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <math.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef int             state_t;
typedef int             status_t;

#ifndef _STDINT_H
#ifndef __int8_t_defined
#ifndef _INT8_T
typedef char            int8_t;
#endif
typedef unsigned char   uint8_t;
typedef short           int16_t;
typedef unsigned short  uint16_t;
typedef int             int32_t;
typedef unsigned int    uint32_t;
typedef long long       int64_t;
typedef unsigned long long uint64_t;
#endif
#endif

typedef unsigned char   byte;
typedef int             bool_t;
#define BS_TRUE         1
#define BS_FALSE        0
#define BS_NOTBOOL      -1

typedef int (* compare_f)(void* e1, void* e2);

typedef enum _data_type_t{
    BS_TYPE_INT = 0,
    BS_TYPE_INT8,
    BS_TYPE_INT16,
    BS_TYPE_INT32,
    BS_TYPE_INT64,
    BS_TYPE_STRING,
    BS_TYPE_OBJECT,
    BS_TYPE_NUM
}data_type_t;
    
typedef enum _code_t{
    BS_CODE_GB2312,
    BS_CODE_GB18030,
    BS_CODE_UTF8,
    BS_CODE_ANSI,
    BS_CODE_UNICODE,
    BS_CODE_NUM
}code_t;

typedef enum _lang_t{
    BS_CHINESE,
    BS_LANGNUM
}lang_t;

#ifdef __cplusplus
}
#endif
#endif  //__BS_TYPE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
