/**
 * file :	HttpEntity.h
 * author :	Rex
 * create :	2017-03-16 14:04
 * func :   一个高性能异步http
 * history:
 */

#ifndef	__HTTPENTITY_H_
#define	__HTTPENTITY_H_

#include "bs_url.h"
#include "AsyncSocket.h"
#include "SocketFrame.h"
#include "ThreadPool.h"
#include "McHttp.h"

using namespace mc;
namespace model {
// 基于bs_url
class HttpEntity: public AsyncSocket{
public:
    HttpEntity(HttpCallback* callback = NULL, ThreadPool* thread=NULL, SocketFrame* sockframe = NULL);
    ~HttpEntity();
    
    virtual state_t get(const char* url);
    virtual state_t post(const char* url, const char* body, uint32_t length = 0);
    virtual state_t put(const char* url, const char* body, uint32_t length = 0);
    virtual state_t del(const char* url);
    virtual state_t http(const char* url, const char* method, const char* body = NULL, uint32_t length = 0);
    
    void addHttpHeader(const char* key, const char* value);
    
    state_t perform(http_t* http);
    virtual void onRead();
    virtual void onWrite();
    virtual void onError(int error);
    virtual void onMessage(sock_msg_t* msg);
    
    void close();
    
    HttpCallback*   m_callback;
protected:
    ThreadPool*     m_http_thread;
    SocketFrame*    m_sock_frame;
    char            m_current_domain[32];
    int             m_current_port;
};
}

#endif
