/**
 * file :	JSON.h
 * author :	Rex
 * create :	2016-09-19 18:52
 * func :   JSON封装，基于cJson实现
 * history:
 */

#ifndef	__JSON_H_
#define	__JSON_H_

#include <iostream>
#include <vector>
#include <typeinfo>
#include "bs.h"
#include "cJSON.h"

// 只支持Ojbect和Array类型拷贝
cJSON* JSONCopy(cJSON* src, cJSON* dest);
namespace mc {
    
class JSON{
public:
    // 对应JSONObject和JSONArray的默认构造函数，自己创建root
    JSON();
    ~JSON();
    
    typedef struct json_t{
        struct json_t*  next,*prev;
        struct JSON*    child;
        
        int             type;
        
        char*           valuestring;
        int             valueint;
        double          valuedouble;
        
        char*           string;
    }json_t;
    
    virtual bool parse(const char* buf);
    virtual void copy(JSON* json, bool auth);
    virtual const char* toString();
    virtual bool serialize(const char* path);
    virtual bool deserialize(const char* path);
    
    cJSON* getCJSON() { return m_root; }
    int type() { return m_root->type;}
    
    // 是否是空
    bool empty(){
        return m_root->child==NULL;
    }
    
    // 是否自动销毁
    void setDestruct(bool flag){
        m_auto_destruct = flag;
    }
    
protected:
    cJSON*          m_root;
    bool            m_auto_destruct;
    std::string     m_json_string;
};

class JSONArray;
class JSONObject:public JSON{
public:
    JSONObject();
    // 不转移权限
    JSONObject(cJSON* root, bool copy = false);
    JSONObject(const char* buf);
    // 使用JSON初始化, 是否转移cJSON的释放权限
    JSONObject(JSON* cjson, bool transAuth=true);
    // 拷贝构造函数使用深拷贝
    JSONObject(const JSONObject& cjson);
    
    virtual void put(const char* name, const char* value);
    virtual void put(const char* name, double value);
    virtual void put(const char* name, int value);
    virtual void put(const char* name, long value);
    virtual void put(const char* name, float value);
    virtual void put(const char* name, bool value);
    virtual void putNull(const char* name);
    // 如果不转移释放权限，则会拷贝一份
    virtual void put(const char* name, JSON* json);
    virtual void put(const char* name, cJSON* json);
    //virtual void copy(JSONObject* obj);
    
    bool has(const char* name);
    // 如果key不存在返回dv
    virtual int intValue(const char* name, int dv = 0);
    virtual long longValue(const char* name, long dv = 0);
    virtual float floatValue(const char* name, float dv = 0);
    virtual double doubleValue(const char* name, double dv = 0);
    virtual bool_t boolValue(const char* name);
    virtual const char* stringValue(const char* name);
    virtual JSONObject jsonValue(const char* name);
    virtual JSONArray arrayValue(const char* name);
    
    virtual void clear(){
        if (m_root!=NULL) {
            cJSON_Delete(m_root);
        }
        
        m_root = cJSON_CreateObject();
    }
    
    std::vector<std::string> keys();
};

class JSONArray: public JSON{
public:
    JSONArray();
    JSONArray(cJSON* root);
    JSONArray(const char* buf);
    JSONArray(JSON* cjson, bool transAuth=true);
    // 拷贝构造函数使用深拷贝
    JSONArray(const JSONArray& cjson);
    
    virtual void append(const char* value);
    virtual void append(int value);
    virtual void append(bool value);
    virtual void append(float value);
    virtual void append(double value);
    virtual void append(JSON* value);
    
    virtual int* intValue(uint32_t index);
    virtual double* doubleValue(uint32_t index);
    virtual bool_t boolValue(uint32_t index);
    virtual char* stringValue(uint32_t index);
    virtual JSONObject jsonValue(uint32_t index);
    virtual JSONArray arrayValue(uint32_t index);
    
    void clear(){
        cJSON_Delete(m_root);
        m_root = cJSON_CreateArray();
    }
    
    uint32_t size(){
        return cJSON_GetArraySize(m_root);
    }
};
}

#endif
