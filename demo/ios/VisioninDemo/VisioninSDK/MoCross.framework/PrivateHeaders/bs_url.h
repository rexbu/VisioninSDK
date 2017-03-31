/**
 * file :	bs_url.h
 * author :	bushaofeng
 * create :	2014-10-06 23:00
 * func : 
 * history:
 */

#ifndef	__BS_URL_H_
#define	__BS_URL_H_

#include "bs_type.h"
#include "bs_common.h"
#include "bs_conf.h"
#include "bs_object.h"
#include "bs_data.h"

#define URL_SIZE    1024

#define HTTP_INVALID    -1
#define HTTP_OK         200
#define HTTP_NOTFOUND   404

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE
}http_method_t;
    
typedef struct url_t{
    object_t    base;
    uint16_t    port;
    //char        url[URL_SIZE];
    data_t      url;
    data_t      protocal;
    data_t      host;
    data_t      domain;
    data_t      path;
    data_t      query;
}url_t;

void* url_init(void* p);
void url_destroy(void* p);
state_t url_parse(url_t* url, const char* url_str);
void url_print(url_t* url);

typedef struct http_res_t http_res_t; 
typedef struct http_t{
    url_t       url;    // url中已经有base，不需要再有
    data_t      req;
    char*       body;
    uint32_t    body_size;
}http_t;

void* http_init(void* p);
void http_destroy(void* p);
/// 创建http
http_t* http_create(const char* url, const char* method);
/// 设置http头
void http_set_header(http_t* http, const char* key, const char* value);
/// 设置http body
void http_set_body(http_t* http, const char* body, uint32_t body_size);
/// 执行http，内部会delete http结构体
http_res_t* http_perform(http_t* http);
/// 下载文件到本地. return ==0:正确；>0:http错误码 <0:连接http失败
state_t http_download(http_t* http, const char* file);

struct http_res_t{
    data_t      response;
    int         response_code;
    char*       body;
    uint32_t    body_size;
};

void* http_res_init(void * p);
void http_res_destroy(void* p);

state_t http_response_parse(http_res_t* res, const char* buffer, uint32_t size);
#ifdef __cplusplus
}
#endif

#endif
