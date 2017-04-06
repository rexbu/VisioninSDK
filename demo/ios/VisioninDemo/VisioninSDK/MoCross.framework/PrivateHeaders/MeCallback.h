/**
 * file :	MeCallback.h
 * author :	bushaofeng
 * create :	2016-08-27 23:28
 * func : 
 * history:
 */

#ifndef	__MECALLBACK_H_
#define	__MECALLBACK_H_

#include "McHttp.h"
using namespace mc;

class MeObject;
class MeException;
typedef void (*MeCallback_func)(MeObject* obj, MeException* err, uint32_t size);

class MeCallback: public HttpCallback{
public:
    MeCallback(const char* classname=NULL, MeObject* obj=NULL){
        m_classname = classname;
        m_object = obj;
    }
    
    virtual void done(int http_code, status_t st, char* text);
    virtual void done(MeObject* obj, MeException* err, uint32_t size = 1) = 0;
    const char* m_classname;
    MeObject*   m_object;
};

// 回调完成后会动态删除自己
class MeFuncCallback: public MeCallback{
public:
    MeFuncCallback(MeCallback_func callback=NULL, const char* classname=NULL, MeObject* obj=NULL);
    
    virtual void done(MeObject* obj, MeException* err, uint32_t size = 1);
    
    // 是否动态删除自己，目前全部删除
    bool            m_dynamic;
protected:
    MeCallback_func m_callback;
};
#endif
