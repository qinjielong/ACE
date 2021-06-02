#pragma once

#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Singleton.h>
#include <Ace/Thread_Mutex.h>
#include <protocol/MessagePack.pb.h>
#include <Ace/INET_Addr.h>
#include <hash_map>
#include "UDPChannel.h"
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
		UDPChannel* get_channel(ACE_UINT32 id);
		// ����ͨ��������ͨ��ID
		ACE_UINT32 add_channel(UDPChannel* channel);
		// ɾ��ͨ��
		void delet_channel(ACE_UINT32 id);
		// ���ݱ�ʶ��ȡͨ��
		UDPChannel* get_channel_by_flag(ACE_UINT32 flag);
		// �ͷ�����ͨ��
		void release_all_channel();

	private:
		typedef hash_map<ACE_UINT32 ,UDPChannel*> Channel_Hash_Map;

		Channel_Hash_Map	channel_map_;	// ͨ����
		ACE_RW_Thread_Mutex rw_lock_;		// ��д��
		//int				id_cursor;		// ID�α�
	};

	typedef ACE_Singleton<ChannelManager,ACE_Thread_Mutex>  Sgl_ChannelManager;

} // end namespace


