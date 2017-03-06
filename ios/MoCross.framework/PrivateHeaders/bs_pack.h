/**
 * file :	bs_pack.h
 * author :	bushaofeng
 * create :	2013-11-21 17:24
 * func : 
 * history:	
 *          2013-11-28 pack buffer改为外部传入，key与value全部存储在buffer中，不再动态创建
 */

#ifndef	__BS_PACK_H_
#define	__BS_PACK_H_

#include "bs_type.h"
#include "bs_struct.h"

#ifdef __cplusplus
extern "C"{
#endif

#define BS_PACK_ITEMSIZE    16
#define BS_PACK_KEYSIZE 16

typedef struct _bs_pack_item_t{
    char*               key;
    uint32_t            key_size;
    void*               value;
    uint32_t            value_size;
}bs_pack_item_t;

//暂时用array，后面改成hash
typedef struct _bs_pack_t{
    bool_t                  dyn;
    char*                   buf;
    // 内容长度
    uint32_t                len;
    // buffer长度
    uint32_t                size;
    bs_pack_item_t          item[BS_PACK_ITEMSIZE];
    uint32_t                item_len;
}bs_pack_t;

// 初始化
state_t bs_pack_init(bs_pack_t* pack, char* buf, uint32_t size);
state_t bs_pack_init_dyn(bs_pack_t* pack, uint32_t size);

//添加
state_t bs_pack_add(bs_pack_t* pack, const char* key, const void* value, uint32_t value_size);
state_t bs_pack_set(bs_pack_t* pack, const char* key, const void* value, uint32_t size);

// 无论value类型，以\0结尾，字符串类型可以直接使用
void* bs_pack_get(bs_pack_t* pack, const char* key);
void* bs_pack_get_size(bs_pack_t* pack, const char* key, uint32_t* size);
int bs_pack_getint(bs_pack_t* pack, const char* key);
char* bs_pack_getstr(bs_pack_t* pack, const char* key);

// 解析buffer到pack
state_t bs_pack_parse(bs_pack_t* pack, char* buf, uint32_t buf_size);

// 序列化到buffer
//uint32_t bs_pack_serialize(bs_pack_t* pack, char* buf, uint32_t buf_size);
#define bs_pack_serialize(pack) ((pack)->buf)
#define bs_pack_bufsize(pack)   ((pack)->len)

uint32_t bs_pack_getbufsize(const char* buf);
// 销毁
state_t bs_pack_destroy(bs_pack_t* pack);

#ifdef __cplusplus
}
#endif
#endif
