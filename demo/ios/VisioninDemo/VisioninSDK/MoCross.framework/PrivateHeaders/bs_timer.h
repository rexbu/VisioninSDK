/**
 * file :	bs_timer.h
 * author :	bushaofeng
 * create :	2013-11-29 22:05
 * func : 
 * history:
 */

#ifndef	__BS_TIMER_H_
#define	__BS_TIMER_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_struct.h"

#ifdef __cplusplus
extern "C"{
#endif

// 默认定时器个数
#define TIMER_DEF_SIZE          1024
// 默认10ms检查一次定时器
#define TIMER_INTERVAL_SERVER   1000
#define TIMER_INTERVAL_MOBILE   5000

typedef struct bs_timer_t   bs_timer_t;
typedef void (* _timer_f)(bs_timer_t*);
typedef void (* _timers_f)(void* para);

struct bs_timer_t{
    uint32_t        id;
    _timer_f        func;
    void*           para;
    //  执行次数
    uint32_t        run_cnt;
    // 任务启动的时钟周期
    uint32_t        time_size;
    // timer时钟周期，每累积到time_size清零
    uint32_t        time_cnt;
    // 任务执行stop_cnt次后删除
    uint32_t        stop_cnt;
};

#define timer_id(t)         ((t)->id)
#define timer_func(t)       ((t)->func)
#define timer_para(t)       ((t)->para)
#define timer_runcnt(t)     ((t)->run_cnt)
#define timer_interval(t)   ((t)->time_size)
    
#define bs_timer_suspend()  signal(SIGALRM, bs_timer_procnull)
#define bs_timer_resume()   signal(SIGALRM, bs_timer_proc)
    
#define bs_timer_sleep(tm)  select(1, NULL, NULL, NULL, (tm));

state_t bs_timer_init(uint32_t interval);
// 正确返回一个id，错误返回-1
bs_timer_t* bs_timer_set(_timer_f func, void* para, struct timeval tm);
bs_timer_t* bs_timer_set_now(_timer_f func, void* para, struct timeval tm);
bs_timer_t* bs_timer_set_num(_timer_f func, void* para, struct timeval tm, uint32_t num);
int bs_timer_set_until(_timer_f func, void* para, struct timeval start, struct timeval end);

void bs_timer_select(struct timeval* tm, _timers_f func, void* para);
    
state_t bs_timer_del(int id);
void bs_timer_signal(int sig);
void bs_timer_proc();
void bs_timer_procnull();

#ifdef __cplusplus
}
#endif
#endif
