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
class MeCallback: public HttpCallback{
public:
    virtual void done(int http_code, status_t st, char* text);
    virtual void done(MeObject* obj, MeException* err, uint32_t size = 1) = 0;
    const char* m_classname;
    MeObject*   m_object;
};

#endif
