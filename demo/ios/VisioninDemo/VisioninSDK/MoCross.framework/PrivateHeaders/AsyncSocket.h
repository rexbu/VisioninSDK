/**
 * file :	AsyncSocket.h
 * author :	bushaofeng
 * create :	2014-10-04 22:39
 * func :   异步Socket, 一次发包不能超过mtu大小
 * history:
 */

#ifndef	__ASYNCSOCKET_H_
#define	__ASYNCSOCKET_H_

#include "bs.h"
#include "Thread.h"
#ifdef __ANDROID__
#include <jni.h>
#endif

typedef struct sock_msg_t{
    object_t            base;
    int                 sock;
    int                 size;
    char*               buf;
    struct sockaddr_in  addr;
    // 额外参数，目前用法指向AsyncSocket，用于AsyncQueue的回调
    void*               arg;
    char                temp[SOCKET_UDP_MTU];
} sock_msg_t;

void* sock_msg_init(void* p);
void  sock_msg_destroy(void* p);

// message的异步执行线程
void* sock_on_message(void* arg);

class AsyncSocket{
public:
    AsyncSocket(int sock, int sock_type = SOCK_STREAM);
    AsyncSocket();
    virtual ~AsyncSocket(){}
    
    int getSocket() {
        return m_sock;
    }
    
    virtual void onRead();
    virtual void onWrite() = 0;
    virtual void onError(int error) = 0;
    virtual void onMessage(sock_msg_t* msg) = 0;

protected:
    int                 m_sock;
    int                 m_type;
};

#endif
