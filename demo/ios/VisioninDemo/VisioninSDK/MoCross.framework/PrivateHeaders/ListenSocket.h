/**
 * file :	ListenSocket.h
 * author :	bushaofeng
 * create :	2014-10-09 18:36
 * func :   监听
 * history:
 */

#ifndef	__LISTENSOCKET_H_
#define	__LISTENSOCKET_H_

#include "bs.h"
#include "AsyncSocket.h"
#include "SocketFrame.h"
#include "ThreadPool.h"

class TCPSocket: public AsyncSocket{
public:
    TCPSocket(int port):m_port(port){
        m_sock = socket_tcp(BS_TRUE);
        bs_sock_bind(m_sock, m_port);
        listen(m_sock, BACKLOG_SIZE);
    }
    
    void onRead();
    void onWrite(){}
    void onError(int error){}
    void onMessage(sock_msg_t* msg){
        int sock = *((int*)msg->buf);
        AsyncSocket* socket = createSocket(sock);
        SocketFrame::instance()->append(socket);
    }
    
    // 纯虚函数，需要实现的生成AsyncSocket类型
    virtual AsyncSocket* createSocket(int sock) = 0;
    const static int BACKLOG_SIZE = 64;
    
protected:
    int         m_port;
};

class UDPSocket: public AsyncSocket{
public:
    UDPSocket(int port):m_port(port){
        m_sock = socket_udp(BS_TRUE);
        bs_sock_optimize(m_sock);
        bs_sock_bind(m_sock, port);
    }
    
    virtual void onRead();
    virtual void onWrite(){}
    virtual void onError(int error){}
    
protected:
    int         m_port;
};
#endif
