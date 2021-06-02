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
	
	// 通道管理器
	class ChannelManager
	{
	public:
		ChannelManager(void);
		~ChannelManager(void);

	public:
		// 根据通道ID获取通道
		UDPChannel* get_channel(ACE_UINT32 id);
		// 加入通道并返回通道ID
		ACE_UINT32 add_channel(UDPChannel* channel);
		// 删除通道
		void delet_channel(ACE_UINT32 id);
		// 根据标识获取通道
		UDPChannel* get_channel_by_flag(ACE_UINT32 flag);
		// 释放所有通道
		void release_all_channel();

	private:
		typedef hash_map<ACE_UINT32 ,UDPChannel*> Channel_Hash_Map;

		Channel_Hash_Map	channel_map_;	// 通道表
		ACE_RW_Thread_Mutex rw_lock_;		// 读写锁
		//int				id_cursor;		// ID游标
	};

	typedef ACE_Singleton<ChannelManager,ACE_Thread_Mutex>  Sgl_ChannelManager;

} // end namespace


