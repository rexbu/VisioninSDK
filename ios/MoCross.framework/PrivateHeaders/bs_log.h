/**
 * file :   bs_log.h
 * author : bushaofeng
 * create : 2011.8.27
 * func:    日志
 * history: 2011.8.27   初始化
 *          2013.11.19  加锁
 */

#ifndef __BS_LOG_H_
#define __BS_LOG_H_

#include <stdio.h>
#include "bs_type.h"
#ifdef __ANDROID__
#include <android/log.h>
#else
#include <sys/syslog.h>
#endif
#include "bs_def.h"
#include "bs_error.h"
#include "bs_lock.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __ANDROID__
#define	LOG_EMERG	ANDROID_LOG_SILENT/* system is unusable */
#define	LOG_ALERT	ANDROID_LOG_FATAL/* action must be taken immediately */
#define	LOG_CRIT	ANDROID_LOG_DEFAULT/* critical conditions */
#define	LOG_ERR		ANDROID_LOG_ERROR/* error conditions */
#define	LOG_WARNING	ANDROID_LOG_WARN/* warning conditions */
#define	LOG_NOTICE	ANDROID_LOG_VERBOSE/* normal but significant condition */
#define	LOG_INFO	ANDROID_LOG_INFO/* informational */
#define	LOG_DEBUG	ANDROID_LOG_DEBUG
#else
#define	LOG_EMERG	0/* system is unusable */
#define	LOG_ALERT	1/* action must be taken immediately */
#define	LOG_CRIT	2/* critical conditions */
#define	LOG_ERR		3/* error conditions */
#define	LOG_WARNING	4/* warning conditions */
#define	LOG_NOTICE	5/* normal but significant condition */
#define	LOG_INFO	6/* informational */
#define	LOG_DEBUG	7/* debug-level messages */
#endif

#define LOG_NUM         8

#ifdef __ANDROID__

#define err_log(log, args...)		__android_log_print(LOG_ERR, __FUNCTION__, log, ##args);
#define warn_log(log, args...)		__android_log_print(LOG_WARNING, __FUNCTION__, log, ##args);
#define info_log(log, args...)		__android_log_print(LOG_INFO, __FUNCTION__, log, ##args);
#define debug_log(log, args...)		__android_log_print(LOG_DEBUG, __FUNCTION__, log, ##args);

#else

#define err_log(log, args...)   bs_log(g_log, LOG_ERR, log, ##args);

#define warn_log(log, args...)  bs_log(g_log, LOG_WARNING, log, ##args);

#define info_log(log, args...)  bs_log(g_log, LOG_INFO, log, ##args);

#define debug_log(log, args...)   do{ \
char    buf[BS_DEF_STRLEN];     \
sprintf(buf, log, ##args);      \
bs_log(g_log, LOG_DEBUG, "%s: %s", __FUNCTION__, buf);    \
}while(0)

#endif

    
typedef struct{
    char        path[BS_DEF_STRLEN];
    FILE*       type_file[LOG_NUM];
    FILE*       file;
    int         flag[LOG_NUM];
    struct tm   ltm;
    bs_lock_t   lock;
}bs_log_t;

extern bs_log_t*    g_log;
    
int bs_log_type(const char* name);
// 默认开启fatal日志
state_t bs_log_init(const char* path);
state_t bs_log_init_entity(bs_log_t* plog, const char* path);
state_t bs_log_set(bs_log_t* plog, int log, int flag);
state_t bs_log_setflag(bs_log_t* plog, const int* flag, uint32_t size);
state_t bs_log_close(bs_log_t* plog);
state_t bs_log(bs_log_t* plog, int log, const char fmt[], ...);
state_t bs_log_destroy();
    
#ifdef __cplusplus
}
#endif
#endif
