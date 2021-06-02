//=============================================================================
/**
*  @file: Base_Service_Handler.h
*
*  $Id:   Base_Service_Handler.h 2013-10-12
*  
*  ����������
*  1. ���
*  2. ·�ɿͻ���/�����͵Ľ�����Ϣ����������Զ�Ӧ�Ĵ���������
*  ע���������Ϳͻ��˵����ñ�Ϊ1:1����һ���ͻ��˶�Ӧһ�����Ĵ�����
*  @author Don kvin geng
*/
//=============================================================================
#pragma once

#include "Type.h"
#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Singleton.h>
#include <Ace/Thread_Mutex.h>
#include "protocol/MessagePack.pb.h"
#include "NetPacket.h"
#include <Ace/INET_Addr.h>

class Base_Service_Handler : public ACE_Service_Handler
{
public:
	Base_Service_Handler();
	virtual ~Base_Service_Handler();
	// ����������¼�
	virtual void open(ACE_HANDLE h, ACE_Message_Block&);
	// �˴�������ײ���
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
	// �˴���װ����ײ���
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
	// ���ӵ�ַ����
	virtual void addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address);
	// ��������
	void send(char* data,ACE_INT32 len);
	// ��ȡ�ͻ��˵�ַ
	ACE_INET_Addr& get_peer_addr();

protected:
	bool initiate_read_stream();
	// ������Ϣ
	//virtual void resolve_message(const dlp::MessagePack & msg) = 0;
	// ������Ϣ
	void handler_message(const ACE_Message_Block & msgBlock);

private:
	ACE_Asynch_Read_Stream  _reader;		// ��
	ACE_Asynch_Write_Stream _writer;		// д
	ACE_INET_Addr			_peer_addr;	    // ���ӵĵ�ַ
	char*					_buffer;		// ��������
	UINT32					_bufszie;		// ����������С
	bool					_waite;			// �ȴ��¸������
	UINT32					_copysize;		// �Ѹ������ݴ�С
};
