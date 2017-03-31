/**
 * file :	MeCloud.h
 * author :	bushaofeng
 * create :	2016-08-27 01:09
 * func : 
 * history:
 */

#ifndef	__MECLOUD_H_
#define	__MECLOUD_H_

#include "McHttp.h"
#include "MeCallback.h"
#include "JSON.h"

using namespace mc;
class MeException: public JSONObject{
public:
	MeException(){}
    MeException(const char* str):JSONObject(str){}
    
	int errCode(){
        return intValue("errCode");
	}
	const char* errMsg(){
        return stringValue("errMsg");
	}
	const char* info(){
        return stringValue("info");
	}
};

class MeCloud{
public:
	static void initialize(const char* appId, const char* appKey);
	static MeCloud* shareInstance();
	MeCloud(const char* appId, const char* appKey);

	void get(const char* url, MeCallback* callback = NULL);
	void post(const char* url, const char* body, MeCallback* callback = NULL);
	void put(const char* url, const char* body, MeCallback* callback = NULL);
	void del(const char* url, MeCallback* callback = NULL);
	void download(const char* url, const char* path, DownCallback* callback = NULL);
	// 以下4个函数不能同时调用，第二个会把第一个抹掉
	const char* restUrl();
	const char* classUrl();
	const char* userUrl();
	const char* fileUrl();
	
	static const char* m_version;
	static const char* m_base_url;

protected:
	static MeCloud*	m_instance;

	HttpSession	m_http_session;
	char			m_url[URL_SIZE];
	char*			m_app_id;
	char*			m_app_key;
};
#endif
