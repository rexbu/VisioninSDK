/**
 * file :	SocketFrame.h
 * author :	bushaofeng
 * create :	2014-10-05 06:43
 * func :
 * history:
 */

#ifndef __SOCKETFRAME_H_
#define __SOCKETFRAME_H_

#include "bs.h"
#include "AsyncSocket.h"
#include "AsyncFrame.h"

class SocketFrame: public AsyncFrame{
public:
    /**
     * timeout: select过期时间，单位微秒，0代表永不过期一直等待
     */
    static SocketFrame* initialize(uint32_t timeout=0){
        if (m_instance == NULL) {
            m_instance = new SocketFrame(timeout);
        }
        return m_instance;
    }
    
    static SocketFrame* instance(){
        // 如果没有调用initialize初始化，则使用默认参数
        return initialize(0);
    }
    static void destroy(){
        delete m_instance;
    }
    
    SocketFrame(uint32_t time=0);
    void loop();
    
    /*
     * 与select同线程不能进行append/remove，容易造成死锁
     * AsyncSocket的onRead/onWrite一般与select同线程，不要进行append/remove操作
     */
    state_t append(AsyncSocket* socket);
    state_t remove(AsyncSocket* socket);
    state_t isExist(AsyncSocket* socket);
    
protected:
    static SocketFrame* m_instance;
    // select立刻返回后的处理函数
    virtual void interruptHandle(){};
    
protected:
    bs_lock_t           m_lock;
    AsyncSocket*        m_sock_map[0xffff];
};

#endif
