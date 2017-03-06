/**
 * file :	MeFile.h
 * author :	Rex
 * create :	2016-09-20 23:46
 * func : 
 * history:
 */

#ifndef	__MEFILE_H_
#define	__MEFILE_H_

#include "bs.h"
#include "MeObject.h"
#include "McHttp.h"
#include "McZip.h"

using namespace mc;
class MeFile: public MeObject{
public:
    MeFile(JSONObject* obj);
    MeFile(MeFile* file);
    
    const char* getUrl();
    /// 下载
    void download(DownCallback* callback);
    /// 返回存储路径
    const char* filePath();
    /// 解压
    bool unzip(ZipCallback* callback);
    
    /// 解压后文件名，如果不是zip文件返回NULL
    const char* unzipFileName();
    /// 解压后文件路径，如果不是zip文件返回nil
    const char* unzipFilePath();
    
    /// 根目录
    static const char*  rootPath();
    
protected:
    static const char*  m_root_path;
    
    void init();
    
    const char* m_name;
    char        m_url[URL_SIZE];
    char        m_path[1024];
    
    char        m_unzip_name[256];
    char        m_unzip_path[1024];
};

#endif
