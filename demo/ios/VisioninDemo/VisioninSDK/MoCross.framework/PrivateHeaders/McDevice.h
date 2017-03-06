/**
 * file :	McDevice.h
 * author :	Rex
 * create :	2016-10-26 15:28
 * func : 
 * history:
 */

#ifndef	__MCDEVICE_H_
#define	__MCDEVICE_H_

#include <vector>
#include <map>
#include "bs.h"

namespace mc {
    const char* device_id();
    const char* bundle_id();
    const char* package_name();
    const char* device_version();
    const char* system_version();
    int device_index();
    
    void guid(char uid[24]);
}

#endif
