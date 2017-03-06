/**
 * file :	MeObject.h
 * author :	bushaofeng
 * create :	2016-08-27 01:09
 * func : 
 * history:
 */

#ifndef	__MEOBJECT_H_
#define	__MEOBJECT_H_

#include <string>
#include <map>
#include "MeCloud.h"
#include "JSON.h"

#define ME_CLASS_SIZE   64
using namespace mc;
using namespace std;

#define ME_TYPE_POINTER  "__pointer"
class MeObject: public JSONObject{
public:
    MeObject(); // 只用于MeCallback生成MeObject数组，数组必须使用默认构造函数
    // 默认转移释放权限，如果不转移会深拷贝
    MeObject(MeObject* obj, bool auth = true);
	MeObject(const char* className, JSONObject* obj = NULL);
    ~MeObject();

    void setClassName(const char* className);
    const char* className(){ return m_classname; }
    void setDb(const char* db);
    const char* objectId(){ return m_objectid; }

    virtual void put(const char* name, const char* value);
    virtual void put(const char* name, double value);
    virtual void put(const char* name, int value);
    virtual void put(const char* name, long value);
    virtual void put(const char* name, float value);
    virtual void put(const char* name, bool value);
    virtual void put(const char* name, MeObject* obj);
    
    MeObject* objectValue(const char* key);
    
	void increase(const char* key, int num);
    void increase(const char* key, long num);
	void increase(const char* key, float num);

	void save(MeCallback* callback);

    // 是否转移释放权限，如果不转移则深拷贝
    virtual void copy(JSONObject* obj, bool auth= true);
    void copySelf(JSONObject *obj, bool auth = true);
    
    /// 从文件反序列化
    virtual bool deserialize(const char* path);
    
protected:
    char            m_classname[ME_CLASS_SIZE];
    char            m_db[ME_CLASS_SIZE];
    char            m_url[URL_SIZE];
    
    const char*             m_objectid;
	JSONObject              m_set_dirty;
    JSONObject              m_inc_dirty;
    //TODO: 暂时只考虑了从服务器下载数据的解析
    map<string, MeObject*>  m_object_map;
    
    // 存储save传进来的callback，目前只支持一个callback，一次save完成之后才能进行下次save
    MeCallback*     m_callback;
};

#endif
