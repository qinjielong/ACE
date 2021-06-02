#pragma once

#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Singleton.h>
#include <Ace/Thread_Mutex.h>
#include <protocol/MessagePack.pb.h>
#include <Ace/INET_Addr.h>
#include <hash_map>
#include "TCPChannel.h"

#include <ext/hash_map>  
 
using __gnu_cxx::hash_map;

using namespace std;
//using namespace stdext;

namespace DataLock{
	
	// ͨ��������
	class ChannelManager
	{
	public:
		ChannelManager(void);
		~ChannelManager(void);

	public:
		// ����ͨ��ID��ȡͨ��
		TCPChannel* get_channel(ACE_UINT32 id);
		// ����ͨ��������ͨ��ID
		ACE_UINT32 add_channel(TCPChannel* channel);
		// ɾ��ͨ��
		void delet_channel(ACE_UINT32 id);
		// ���ݱ�ʶ��ȡͨ��
		TCPChannel* get_channel_by_flag(ACE_UINT32 flag);
		// ��ȡ�б��ͨ������
		ACE_UINT32 get_channel_flag_number();
		// �ͷ�����ͨ��
		void release_all_channel();

	private:
		typedef hash_map<ACE_UINT32 ,TCPChannel*> Channel_Hash_Map;

		Channel_Hash_Map	channel_map_;	// ͨ����(��ϣ��)
		ACE_RW_Thread_Mutex rw_lock_;		// ��д��
		//int				id_cursor;		// ID�α�
	};

	typedef ACE_Singleton<ChannelManager,ACE_Thread_Mutex>  Sgl_ChannelManager;

} // end namespace


