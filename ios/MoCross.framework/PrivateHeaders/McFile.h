/**
 * file :	File.h
 * author :	bushaofeng
 * create :	2016-08-27 01:01
 * func :   文件及文件夹管理
 * history:
 */

#ifndef	__File_H_
#define	__File_H_

#include <dirent.h>
#include "bs.h"
#include "JSON.h"

#define MC_FILE_PATH_SIZE   1024
extern char g_package_name[256];

/// 文件封装
namespace mc {

class Folder{
public:
    Folder(const char* path);
    ~Folder(){
        closedir(m_dir);
    }
    // 返回type后缀的文件
    const char* type(const char* type);
    const char* name(const char* name);
    
protected:
    DIR*            m_dir;
    std::string     m_path;
    char            m_buffer[1024];
};

class File{
public:
    File();
    File(const char* path);
    ~File() {
        string_destroy(&m_path);
        if (m_bytes!=NULL) {
            free(m_bytes);
        }
    }
    
    ///TODO: 需要区分是文件不存在还是无打开权限
    bool open(const char* path);
    /// path=NULL表示先执行了open
    byte* read(const char* path=NULL);
    /// 和FileManager区别是这里只判断文件，不判断文件夹
    bool exist();
    /// 文件大小
    size_t size();
    
protected:
    string_t        m_path;
    bool            m_exist;
    struct stat     m_info;
    uint8_t*        m_bytes;
    size_t          m_bytes_size;
};

/// 文件管理
class FileManager{
public:
    FileManager* shareInstance();
    
    static bool isFile(const char* path);
    static bool isDir(const char* path);
    /// 不区分文件或者文件夹
    static bool exist(const char* path);
    static void mkdir(const char* dir);
    static void move(const char* from, const char* to);
    static void remove(const char* path);
    static int write(const char* path, void* data, size_t size);
    /// 根目录
    static const char* rootPath();
    // 资源文件目录
    static const char* resourcePath();
    static const char* resource(const char* file, char* path = NULL, uint32_t size = 0);
    // 临时存储目录
    static const char* tempPath();
    
protected:
    static FileManager*     m_instance;
    static const char*      m_root_path;
    static const char*      m_resource_path;
    static char             m_tmp_path[1024];
    static char             m_path_buffer[1024];
};

#ifdef __ANDROID__
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
class Resource{
public:
    Resource(jobject assetManager);
    Resource();
    ~Resource();
    int read(const char* file);
    size_t size(){return m_bytes_size;}
    uint8_t* bytes(){return m_bytes;}

protected:
    static AAssetManager*   m_asset_manager;

    bool            m_exist;
    uint8_t*        m_bytes;
    size_t          m_bytes_size;
    AAsset*         m_asset;
}; 

#endif
}
#endif
