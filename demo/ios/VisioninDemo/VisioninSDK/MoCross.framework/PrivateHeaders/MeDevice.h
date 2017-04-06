/**
 * file :	MeDevice.h
 * author :	Rex
 * create :	2017-04-04 16:57
 * func : 
 * history:
 */

#ifndef	__MEDEVICE_H_
#define	__MEDEVICE_H_

#include "MeObject.h"

class MeDevice: public MeObject, public MeCallback{
public:
    MeDevice();
    MeDevice(JSONObject* obj);
    
    // 当前设备
    static MeDevice* currentDevice();
    // 注册设备
    void login(MeCallback* callback);
    void logout();
    
    virtual void done(MeObject* obj, MeException* err, uint32_t size = 1);
    
protected:
    static MeDevice*    m_current_device;
};

#endif
