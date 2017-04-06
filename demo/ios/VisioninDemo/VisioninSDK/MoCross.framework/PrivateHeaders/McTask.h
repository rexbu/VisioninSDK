/**
 * file :	McTask.h
 * author :	Rex
 * create :	2016-11-03 17:00
 * func :   基于管道的异步，推荐使用TheadPool
 * history:
 */

#ifndef	__MCTASK_H_
#define	__MCTASK_H_

#include <iostream>
#include <vector>
#include <pthread.h>
 #include "Thread.h"

using namespace std;
typedef void* (* mc_async_task_f)(void* para);

class McAsyncTaskQueue;
namespace mc {
// 异步任务
struct async_task_t{
    void*       argv;
    // 任务完成后执行函数，一般为清理参数内存
    mc_async_task_f run;
};

class AsyncTaskQueue: public LoopThread{
public:
    AsyncTaskQueue(uint32_t time=0);
    ~AsyncTaskQueue(){}
    // Override
    virtual void stop();
    virtual void loop();
    bool running(){ return m_processing; }
    uint32_t taskNum(){ return (uint32_t)m_tasks.size(); }
    
    // 让select立刻返回，并执行任务
    void add(mc_async_task_f run, void* para);
    
protected:
    // 管道，用于控制select结束
    int             m_pipe[2];
    int             m_max_sock;
    // 0代表超时或者select interrupt，无socket准备就绪
    int             m_select_rv;
    
    fd_set          m_read_set;
    fd_set          m_write_set;
    fd_set          m_error_set;
    timeval*        m_timeout;
    timeval         m_time_entity;
    
    // 任务队列
    vector<async_task_t> m_tasks;
    bool            m_processing;   // 是否在执行任务
    pthread_mutex_t m_lock;
};

class AsyncFrame{
public:
    static void initialize(int queueNum = 1);
    static AsyncFrame* shareInstance();
    static void destroyInstance();
    
    // 添加一个异步任务
    void addTask(mc_async_task_f run, void* para);
    // 队列数量
    uint32_t queueNum(){ return (uint32_t)m_task_queue.size(); }
protected:
    static AsyncFrame* m_instance;
    
    AsyncFrame(int queueNum);
    ~AsyncFrame();
    
    vector<AsyncTaskQueue*>  m_task_queue;
};
}
#endif
