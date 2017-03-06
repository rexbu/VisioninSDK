/**
 * file :	SharedPreferences.h
 * author :	Rex
 * create :	2016-10-21 15:46
 * func : 
 * history:
 */

#ifndef	__SHAREDPREFERENCES_H_
#define	__SHAREDPREFERENCES_H_

#include "McFile.h"
#include "JSON.h"

namespace mc {
    /// 轻量级存储
    class SharedPreferences: public JSONObject, public File{
    public:
        SharedPreferences(const char* name);
        
        bool commit();
        
        static const char* path(const char* name, char* path, uint32_t size);
    protected:
        static const char*  m_root_path;
        
        char        m_path[MC_FILE_PATH_SIZE];
    };
}

#endif
