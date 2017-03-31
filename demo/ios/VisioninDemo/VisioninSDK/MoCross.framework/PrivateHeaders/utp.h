/**
 * file :	utp.h
 * author :	Rex
 * create :	2017-03-23 13:31
 * func : 
 * history:
 */

#ifndef	__UTP_H_
#define	__UTP_H_

#include <iostream>
#include <queue>
#include "AsyncSocket.h"

using namespace std;

typedef struct utp_slice_t {
    unsigned short	serial;		// 包序号
    unsigned short	slicesize;	// 包数量
    unsigned short	datasize;	// 数据长度
    unsigned short	identifier;	// 总序号
    unsigned int	crc_code;	// 校验码
    unsigned char	data[1400];
}utp_slice_t;

#define UTP_PAYLOAD_SIZE        (sizeof(utp_slice_t)-offsetof(utp_slice_t, data))
#define UTP_SLICE_SIZE(utp)     (offsetof(utp_slice_t, data)+(size_t)((utp)->datasize))
typedef struct utp_pack_t {
    vector<utp_slice_t>	slices;
    unsigned int		statflag;	// 每位表示是否已经接收，最多32个包，大约32k
    sockaddr_in         addr;
}utp_pack_t;

#define UTP_BUFFER_SIZE     64

class UTP: public AsyncSocket{
public:
    static void initialize(int port);
    static UTP* shareInstance();
    
    UTP(int port);
    ~UTP();
    
    virtual void onRead();
    virtual void onWrite();
    virtual void onError(int error);
    virtual void onMessage(sock_msg_t* msg);
    
    virtual void send(sockaddr_in* addr, const char* buffer, uint32_t size);
    virtual void send(const char* ip, int port, const char* buffer, uint32_t size);
    void sendSerial();
    
protected:
    static UTP*             m_instance;
    
    vector<utp_pack_t>      m_recv_queue;
    vector<utp_pack_t>      m_send_queue;
    // 如果有多个源，需要考虑区分不同源的id
    uint32_t                m_identifier;
    // 当前正在发送的包id
    uint32_t                m_send_id;
    
    LoopThread*             m_send_thread;
};

#endif
