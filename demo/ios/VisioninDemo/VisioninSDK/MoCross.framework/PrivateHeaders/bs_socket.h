/**
 * file :	bs_socket.h
 * author :	bushaofeng
 * created :	2013-11-18 15:04
 * func : 
 */

#ifndef	__BS_SOCKET_H_
#define	__BS_SOCKET_H_

#include "bs_type.h"
#include "bs_error.h"
#include "bs_def.h"

#ifdef __cplusplus
extern "C"{
#endif

#define SOCKET_TCP_MTU      1452
#define SOCKET_UDP_MTU      1464
#define addr2int(addr)      (((uint64_t)(addr).sin_addr.s_addr<<32)|(addr).sin_port)
#define same_lan(addr1, addr2)  (((addr1)->sin_addr.s_addr&0xffffff) == ((addr2)->sin_addr.s_addr&0xffffff))
#define same_nat(nat1, nat2, local1, local2)    (((nat1)->sin_addr.s_addr==(nat2)->sin_addr.s_addr) && same_lan(local1, local2))
    
#define int2addr(addr, val)     do{  \
    (addr).sin_family = AF_INET;        \
    (addr).sin_addr.s_addr = val>>32;   \
    (addr).sin_port = val & 0xffff;     \
}while(0)

typedef struct bs_addr_t{
    char        ip[IP_SIZE];
    int         port;
}bs_addr_t;
    
int socket_udp(bool_t is_nonblock);
int socket_tcp(bool_t is_nonblock);
state_t socket_unblock(int sock);
state_t bs_sock_keepalive(int sock);
state_t bs_sock_nodelay(int sock);
// 对sock设置KEEPALIVE和NODELAY
state_t bs_sock_optimize(int sock);
state_t bs_sock_bind(int sock, int port);
state_t bs_sock_addr(struct sockaddr_in* addr, const char* ip, int port);
state_t bs_sock_connect(int sock, const char* ip, int port);
state_t bs_sock_localip(char* ip, int size);
    
int bs_sock_recvfrom(int sock,struct sockaddr_in* addr, char* buf, size_t size);
int bs_sock_sendto(int sock,struct sockaddr_in* addr, char* buf, size_t size);

struct sockaddr_in bs_int2addr(uint64_t value);

#define bs_sock_getip(addr)     inet_ntoa((addr)->sin_addr)
#define bs_sock_getport(addr)   ntohs((addr)->sin_port)

#ifdef __cplusplus
}
#endif
#endif
