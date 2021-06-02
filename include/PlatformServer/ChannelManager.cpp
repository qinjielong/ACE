#include "ChannelManager.h"
#include <Ace/OS.h>

namespace DataLock{

	ChannelManager::ChannelManager(void)
	{
	}

	ChannelManager::~ChannelManager(void)
	{
	}

	//获得连接通道
	TCPChannel* ChannelManager::get_channel(ACE_UINT32 id)
	{
		ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		TCPChannel* channel = channel_map_[id];

		return channel;
	}

	ACE_UINT32 ChannelManager::add_channel(TCPChannel* channel)
	{
		ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		ACE_UINT32 id          = 0;

		// 生成通道ID-流水号 需优化
		while ( true )
		{
			id++;

			Channel_Hash_Map::iterator item = channel_map_.find(id);
			if ( item != channel_map_.end() )
			{
				continue;
			}
			else
			{
				channel_map_[id] = channel; //从末尾添加
				return id;
			}
		}
		return -1;
	}

	void ChannelManager::delet_channel(ACE_UINT32 id)
	{
		ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);
		Channel_Hash_Map::iterator iter = channel_map_.find(id);
		if ( iter == channel_map_.end() )
		{
			return;
		}
		channel_map_.erase(iter);
	}

	TCPChannel* ChannelManager::get_channel_by_flag( ACE_UINT32 flag )
	{
		ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		Channel_Hash_Map::iterator iter = channel_map_.begin();
		
		for ( ;iter != channel_map_.end(); ++iter )
		{
			if(flag == iter->second->get_flag())
			{
				return iter->second;
			}
		}
		
		return 0;
	}

	void ChannelManager::release_all_channel()
	{
		ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		Channel_Hash_Map::iterator iter = channel_map_.begin();

		for ( ;iter != channel_map_.end(); ++iter )
		{
			iter->second->disconnect();
		}

		channel_map_.clear();
	}

	ACE_UINT32 ChannelManager::get_channel_flag_number()
	{
		ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		ACE_UINT32 number = 0;

		Channel_Hash_Map::iterator iter = channel_map_.begin();

		for ( ;iter != channel_map_.end(); ++iter )
		{
			if(iter->second->get_flag() > 0 )
			{
				number++;
			}
		}

		return number;
	}
} // end namespace
