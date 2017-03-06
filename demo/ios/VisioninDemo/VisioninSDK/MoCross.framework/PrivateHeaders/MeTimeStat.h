/**
 * file :	MeTimeStat.h
 * author :	Rex
 * create :	2016-10-31 23:11
 * func : 
 * history:
 */

#ifndef	__METIMESTAT_H_
#define	__METIMESTAT_H_

#include <time.h>
#include "MeObject.h"

class MeTimeStat: public MeObject, public MeCallback{
public:
    MeTimeStat(const char* name="StatLog");
    void start();
    void refresh();
    void stop();
    
    virtual void done(MeObject* obj, MeException* err, uint32_t size=1);
protected:
    time_t  m_start;
    time_t  m_prevtime;
    long    m_duration;
};

#endif
