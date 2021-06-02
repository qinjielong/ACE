//=============================================================================
/**
*  @file: TCPChannel.h
*
*  $Id:   TCPChannel.h 2013-10-12
*  
*  TCP������
*  1. ���
*  2. ����ɵ����ݰ���������Ϣ�����У�����Ϣ�����̶߳�ȡ��
*  @author Don kvin geng
*/
//=============================================================================
#pragma once

#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Thread_Mutex.h>
#include "protocol/MessagePack.pb.h"
#include "NetPacket.h"
#include <Ace/INET_Addr.h>
#include <Ace/Null_Mutex.h>
#include <Ace/Null_Condition.h>

namespace DataLock{
	
	//-�����Ӧ�ô�ACE_Service_Handler����
	//-ACE_Service_Handle��Ҫ���Ƕ�����һЩ�ص�����
	class TCPChannel : public ACE_Service_Handler
	{
	public:
		TCPChannel();
		~TCPChannel();

		//-�����Ӵ�����ɺ����潲��Open�����ᱻ���ã����ǿ���Open�����ж���Щʲô���룺
		// ����������¼�  //-���пͻ����������������ӽ����ɹ���Proactor������������
		virtual void open(ACE_HANDLE h, ACE_Message_Block&);
		// �˴�������ײ��� //-���û�Ҫ�������ݶ����˺󣬵����������
		virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
		// �˴���װ����ײ��� //-���û�Ҫд�������������Ϸ��ͳɹ���Proactor��ص��������
		virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
		
		// ���ӵ�ַ����
		virtual void addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address);
		// ��������
		void send(ACE_Message_Block& mb);
		// ��������
		void send(const char* data,ACE_UINT32 length);
		// ��ȡԶ��IP��ַ
		ACE_INET_Addr& get_peer_addr();
		// ��ȡͨ��ID
		ACE_UINT32	   get_id();
		// ������չ��ʶ
		void set_flag(ACE_UINT32 flag);
		// ��ȡ��չ��ʶ
		ACE_UINT32 get_flag();
		// �Ͽ�ͨ��
		void disconnect();

	public:
		// ���ý�����Ϣ������д�С
		static void set_rev_message_queue_size(ACE_UINT32 size);
		// ���÷�����Ϣ������д�С
		//static void set_send_meesage_queue_size(ACE_UINT32 size);
		
		// �����Ϣ��������Ϣ�������
		static int  putq_rev(ACE_Message_Block * mb);
		// ����Ϣ��������л�ȡ��Ϣ���Ӷ�����ɾ��ԭ��Ϣ
		static int  getq_rev(ACE_Message_Block *&mb);

	private:
		bool initiate_read_stream();
		// ������Ϣ
	//	void resolve_message(const DataLock::MessagePack & message_pack);
		// ������Ϣ
	//	void handler_message(const ACE_Message_Block & message_block);

	private:
		static ACE_Message_Queue<ACE_MT_SYNCH> message_queue;	// ������Ϣ���л���
		static ACE_UINT32					   queue_size;	    // ������Ϣ���л����С
	private:
		ACE_Asynch_Read_Stream  _reader;		// ��
		ACE_Asynch_Write_Stream _writer;		// д
		ACE_INET_Addr			_peer_addr;	    // ���ӵĵ�ַ
		bool					_waite;			// �ȴ��¸������
	//	ACE_UINT32				_copysize;		// �Ѹ������ݴ�С
		ACE_UINT32				_id;			// ͨ��ID
		ACE_UINT32				_flag;			// ��չ��ʶ
//		char*					_buffer;		// ���ݻ���
//		ACE_UINT32				_size;			// �������ݴ�С
		ACE_Message_Block*      _mb;			// �����ݻ���[����Ͳ�ճ��ʱʹ��]

	};
} // end namespace



