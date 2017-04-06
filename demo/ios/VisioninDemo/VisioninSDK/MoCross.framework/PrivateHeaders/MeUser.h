/**
 * file :	MeUser.h
 * author :	Rex
 * create :	2016-09-21 15:14
 * func : 
 * history:
 */

#ifndef	__MEUSER_H_
#define	__MEUSER_H_

#include "MeObject.h"

class MeUser: public MeObject, public MeCallback{
public:
    MeUser();
    MeUser(JSONObject* obj);
    
    // 验证码注册
    void signin(const char* username, const char* password, const char* authcode, MeCallback_func callback);
    // 登录
    void login(const char* username, const char* password, MeCallback* callback);
    void logout();
    
    static MeUser* currentUser();
    
    virtual void done(MeObject* obj, MeException* err, uint32_t size = 1);
    
protected:
    void init();
    
    static MeUser*      m_current_user;
    static const char*  m_version;
    
    MeCallback_func m_callback;
};

#endif
