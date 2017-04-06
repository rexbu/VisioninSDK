/**
 * file :	thread.h
 * author :	bushaofeng
 * create :	2014-10-04 22:30
 * func :   线程基类
 * history:
 */

#ifndef __THREAD_H__
#define __THREAD_H__

#include <iostream>
#include "bs.h"

class Thread{
public:
    Thread(void* para){
        mPara = para;
    }
    Thread(){
        mPara = NULL;
    }
    virtual ~Thread(){}
    
    virtual pthread_t start();
    virtual void run() = 0;
    
public:
    void*       mPara;
    pthread_t   m_thread;
};

class LoopThread:public Thread{
public:
    LoopThread(void* para):Thread(para){
        m_running = false;
    }
    LoopThread():Thread(){
        m_running = false;
    }
    virtual pthread_t start(){
        if (m_running) {
            return m_thread;
        }
        
        m_running = true;
        return Thread::start();
    }
    
    virtual void loop() = 0;
    virtual void run(){
        while(m_running){
            loop();
        }
    }
    virtual void stop(){
        m_running = false;
    }

protected:
    bool        m_running;
};
#endif
