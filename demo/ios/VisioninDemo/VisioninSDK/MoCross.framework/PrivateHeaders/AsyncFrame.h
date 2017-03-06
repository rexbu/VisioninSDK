/**
 * file :	AsyncFrame.h
 * author :	bushaofeng
 * create :	2015-03-23 16:55
 * func :   异步框架
 * history:
 */

#ifndef __ASYNCFRAME_H_
#define __ASYNCFRAME_H_

#include "bs.h"
#include "Thread.h"

class AsyncFrame: public LoopThread{
public:
    AsyncFrame(uint32_t time=0);
    ~AsyncFrame(){}
    // Override
    void stop();
    void loop();
    // 让select立刻返回
    void interrupt();
    
protected:
    // select立刻返回后的处理函数
    virtual void interruptHandle(){};
    
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
};

#endif 
