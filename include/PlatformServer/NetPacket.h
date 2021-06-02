#pragma once
#include "Type.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// 网络包结构
#ifdef _WIN32
#pragma pack(push, 1)	// 1字节对齐
#define PACK_ALIGN
#else
#define PACK_ALIGN	__attribute__((packed))
#endif//

// 采用网络字节序(大端)
typedef struct tagNetPacketHead
{
	UINT32	  ver;		// 版本号
	UINT32	  seq;		// 流水号
	UINT32    count;	// 该包总数[udp协议中使用]
	UINT32    num;		// 包编号[udp协议中使用]
	UINT32	  len;		// 包体长度

	void hton(void)
	{
		ver   = ::htonl(ver);
		seq   = ::htonl(seq);
		count = ::htonl(count);
		num   = ::htonl(num);
		len   = ::htonl(len);
	}
	void ntoh(void)
	{
		ver   = ::ntohl(ver);
		seq   = ::ntohl(seq);
		count = ::ntohl(count);
		num   = ::ntohl(num);
		len   = ::ntohl(len);
	}

} PACK_ALIGN NetPacketHead,*PNetPacketHead;


//htons —— 把unsigned short类型从主机序转成网络字节序
//ntohs —— 把unsigned short类型从网络字节序转成主机序
//htonl —— 把unsigned long类型从主机序转成网络字节序
//ntohl —— 把unsigned long类型从网络字节序转成主机序

typedef struct tagNetPack
{
	NetPacketHead		head;		// 包头
	char				body[1];	// 包体 [使用googlebuffer序列化数据]
} PACK_ALIGN NetPacket,*PNetPacket;

#ifdef _WIN32
#pragma pack(pop)
#undef PACK_ALIGN
#else
#undef PACK_ALIGN
#endif //~
//
//////////////////////////////////////////////////////////////////////////