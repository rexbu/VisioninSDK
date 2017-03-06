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

#define URL_SIZE    1024

#define HTTP_OK         200
#define HTTP_NOTFOUND   404

#ifdef __cplusplus
extern "C"{
#endif
typedef struct url_t{
    uint16_t    protocal_size;
    uint16_t    domain_size;
    uint16_t    host_size;
    uint16_t    path_size;
    uint16_t    query_size;
    uint16_t    port;
    //char        url[URL_SIZE];
    char*       url;
    char*       protocal;
    char*       host;
    char*       domain;
    char*       path;
    char*       query;
}url_t;

typedef struct http_response_t{
    int         response_code;
    char*       body;
    uint32_t    body_size;
}http_response_t;

state_t url_parse(url_t* url, const char* url_str);
void url_print(url_t* url);
int http_get(const char* url, const bs_kv_t* header, void* value, uint32_t size);
int http_post(const char* url, const bs_kv_t* header, void* body, uint32_t body_size, void* value, uint32_t size);
state_t http_response_parse(http_response_t* res, const char* buffer, uint32_t size);
#ifdef __cplusplus
}
#endif

#endif
