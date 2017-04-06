/**
 * file :	McTimer.h
 * author :	Rex
 * create :	2017-04-01 14:46
 * func :   定时器
 * history:
 */

#ifndef	__MCTIMER_H_
#define	__MCTIMER_H_

#include "bs.h"
#include "Thread.h"

namespace mc {
    typedef enum{
        TIMER_SLEEP,
        TIMER_SELECT,
        TIMER_RTC,
    }timer_type_t;
    
    class Timer: public LoopThread{
    public:
        Timer(long utime, void* (*callback)(void*), void* para = NULL, timer_type_t type = TIMER_SLEEP);
        
        virtual void loop();
        
    protected:
        long            m_utime;
        timer_type_t    m_timer_type;
        
        void* (*m_callback)(void*);
        void*           m_para;
    };
}
#endif
