/**
 * file :	Http.h
 * author :	bushaofeng
 * create :	2016-08-25 00:51
 * func : 
 * history:
 */

#ifndef	__Http_H_
#define	__Http_H_

#include "bs.h"
#include "curl.h"
#include <string>
#include <vector>

#ifdef __ANDROID__
#include <jni.h>
extern JavaVM* g_jvm;
#endif

using namespace std;
/* http头 */
typedef pair_t(string_t, string_t)  http_header_t;

void* http_header_init(void* p);
void http_header_set(http_header_t* header, const char* key, const char* value);
void http_header_destroy(void* p);

namespace mc {
    
/// 回调
class HttpCallback{
public:
    virtual void done(int http_code, status_t st, char* text) = 0;
};
class DownCallback{
public:
    /// writen:本次下载字节数, total_writen：已经下载字节数，total_expect_write：总下载字节数
    virtual void progress(uint64_t writen, uint64_t total_writen, uint64_t total_expect_write) = 0;
    virtual void done(int http_code, status_t st, const char* location) = 0;
};

typedef enum{
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE
}http_method_t;

class IOSEnv;
/// http
class HttpSession{
public:
    HttpSession();
    ~HttpSession();
    virtual void get(const char* url, HttpCallback* callback = NULL);
    virtual void post(const char* url, const char* body, uint32_t length = 0,HttpCallback* callback = NULL);
    virtual void put(const char* url, const char* body, uint32_t length = 0,HttpCallback* callback = NULL);
    virtual void del(const char* url, HttpCallback* callback = NULL);
    virtual void http(const char* url, http_method_t method, const char* body = NULL, uint32_t length = 0, HttpCallback* callback = NULL);
    virtual void download(const char* url, DownCallback* callback);
    void addHttpHeader(const char* key, const char* value);
    
    static pthread_mutex_t* m_thread_locks; // 多线程ssl锁
    const static uint32_t   HTTP_TIMEOUT = 10;      // 过期时间
protected:
    void setOpt(CURL* curl, http_method_t method, const char* body, uint32_t length);
    vector<pair<string, string> >    m_headers;
    
#ifdef __IOS__
    IOSEnv*     m_iosenv;
#endif
};

/// https
class HttpsSession: public HttpSession{
public:
};
    
}
#endif
