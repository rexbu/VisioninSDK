#ifndef __BS_CONF_H_
#define __BS_CONF_H_

#include <stdio.h>
#include "bs_common.h"
#include "bs_struct.h"
#include "bs_def.h"
#include "bs_error.h"
#include "bs_vector.h"

#ifdef __cplusplus
extern "C"{
#endif

#define _CONF_NAME_SIZE     64
#define _CONF_VALUE_SIZE    256
typedef struct{
    char        name[_CONF_NAME_SIZE];
    char        value[_CONF_VALUE_SIZE];
}_conf_elem_t;

/*
typedef struct{
    array_t(_conf_elem_t)       list;
}bs_conf_t;
*/
    
typedef vector_t(_conf_elem_t)  bs_conf_t;
    
#define bs_conf_getstr_def(conf, name, value)   (bs_conf_getstr((conf),(name))== NULL ? (value):bs_conf_getstr((conf),(name)))
#define bs_conf_getint_def(conf, name, value)   (bs_conf_getint((conf),(name))==-1 ? (value):bs_conf_getint((conf),(name)))
#define bs_conf_init(conf)                      vector_init(conf)
    
int bs_conf_read(bs_conf_t* pconf, const char* path);
const char* bs_conf_getstr(const bs_conf_t* pconf, const char* name);
int bs_conf_getint(const bs_conf_t* pconf, const char* name);
size_t bs_conf_size(const bs_conf_t* pconf);
const char* bs_conf_getname(const bs_conf_t* pconf, uint32_t i);
const char* bs_conf_getstr_idx(const bs_conf_t* pconf, uint32_t i);
int bs_conf_getint_idx(const bs_conf_t* pconf, uint32_t i);
#define bs_conf_destroy(conf)   vector_destroy(conf)

state_t bs_conf_set(bs_conf_t* pconf, const char* name, const void* value, uint32_t size);
state_t bs_conf_setstr(bs_conf_t* pconf, const char* name, const char* value);
state_t bs_conf_setint(bs_conf_t* pconf, const char* name, int value);

// bs_kv_t
typedef vector_t(_conf_elem_t)  bs_kv_t;
#define bs_kv_size(kv)                      bs_conf_size((const bs_conf_t*)(kv))
// 所有key+value的总长度
#define bs_kv_len(kv)                       (sizeof(_conf_elem_t)*bs_kv_size(kv))
    
#define bs_kv_init(kv)                      bs_conf_init((bs_conf_t*)(kv))
#define bs_kv_set(kv, name, value, size)    bs_conf_set((bs_conf_t*)(kv), (name), (value), (size))
#define bs_kv_setstr(kv, name, value)       bs_conf_setstr((bs_conf_t*)(kv), (name), (value))
#define bs_kv_setint(kv, name, value)       bs_conf_setint((bs_conf_t*)(kv), (name), (value))
#define bs_kv_getname(kv, i)                bs_conf_getname((const bs_conf_t*)(kv), (i))
#define bs_kv_getstr_idx(kv, i)             bs_conf_getstr_idx((const bs_conf_t*)(kv), (i))
#define bs_kv_destroy(kv)                   bs_conf_destroy((bs_conf_t*)(kv))
    
#ifdef __cplusplus
}
#endif
#endif
