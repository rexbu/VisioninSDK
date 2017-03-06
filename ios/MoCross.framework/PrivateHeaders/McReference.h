/**
 * file :	McReference.h
 * author :	Rex
 * create :	2016-09-27 14:55
 * func :   引用计数
 * history:
 */

#ifndef	__MCREFERENCE_H_
#define	__MCREFERENCE_H_

#include "bs.h"

namespace mc {
    class Reference{
    public:
        Reference(){
            m_referencecount = 0;
            m_is_reference = true;
        }
        
        bool lock(){
            if (m_is_reference) {
                m_referencecount++;
            }
            
            return true;
        }
        
        bool unLock(){
            if (m_is_reference && m_referencecount<=0)
            {
                err_log("unlock error! referencecount[%d]", m_referencecount);
                return false;
            }
            
            if (m_is_reference) {
                m_referencecount--;
            }
            
            return true;
        }
        
        bool idle(){
            return m_referencecount==0;
        }
        
        void disableReference(){
            m_referencecount = false;
        }
        
        bool release(){
            m_referencecount = 0;
            return true;
        }
        
    protected:
        int     m_referencecount;
        bool    m_is_reference;
    };
}

#endif
