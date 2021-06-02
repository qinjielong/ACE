#pragma once
#include "Type.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// ������ṹ
#ifdef _WIN32
#pragma pack(push, 1)	// 1�ֽڶ���
#define PACK_ALIGN
#else
#define PACK_ALIGN	__attribute__((packed))
#endif//

// ���������ֽ���(���)
typedef struct tagNetPacketHead
{
	UINT32	  ver;		// �汾��
	UINT32	  seq;		// ��ˮ��
	UINT32    count;	// �ð�����[udpЭ����ʹ��]
	UINT32    num;		// �����[udpЭ����ʹ��]
	UINT32	  len;		// ���峤��

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


//htons ���� ��unsigned short���ʹ�������ת�������ֽ���
//ntohs ���� ��unsigned short���ʹ������ֽ���ת��������
//htonl ���� ��unsigned long���ʹ�������ת�������ֽ���
//ntohl ���� ��unsigned long���ʹ������ֽ���ת��������

typedef struct tagNetPack
{
	NetPacketHead		head;		// ��ͷ
	char				body[1];	// ���� [ʹ��googlebuffer���л�����]
} PACK_ALIGN NetPacket,*PNetPacket;

#ifdef _WIN32
#pragma pack(pop)
#undef PACK_ALIGN
#else
#undef PACK_ALIGN
#endif //~
//
//////////////////////////////////////////////////////////////////////////