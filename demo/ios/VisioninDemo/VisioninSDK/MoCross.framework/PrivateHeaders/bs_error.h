/**
 * bushaofneg. 2011.8.24
 * bs_error.h
 * history:
 */

#ifndef __BS_ERROR_H_
#define __BS_ERROR_H_

#define BS_SUCCESS      0
#define BS_INVALID      -1
// 没有达到所需结果，但无需处理，不是错误
#define BS_NOMATTER     -2
// 等待结果中，尚未出错
#define BS_WAITING		-3

// 文件相关
#define BS_NOTFOUND     -10
#define BS_HASEXIST     -11
#define BS_CREATERR     -12

// 内存相关
#define BS_NOMEM        -22
#define BS_NOBUFFER     -23

// 参数相关
#define BS_PARAERR      -31
#define BS_PARANULL     -32
#define BS_JSONPARSE	-33

// 通信相关
#define BS_HOSTERR      -41
#define BS_SOCKERR      -42
#define BS_STREAMERR    -43
#define BS_EPOLLERR     -44
#define BS_TIMEOUT      -45
#define BS_CONNERR      -46
#define BS_SENDERR      -47
#define BS_RECVERR      -48

// 进程相关
#define BS_PROCERR      -51
#define BS_PROCSHUT     -52

// 权限相关
#define BS_AUTHORITY    -61

// 数据库相关
#define BS_DBERR        -71

// 容量相关
#define BS_FULL         -81
#define BS_EMPTY        -82
#define BS_CROSSBORDER  -83

// 环境相关
#define BS_ENVIERR      -91

#endif
