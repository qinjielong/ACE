#include "ChannelManager.h"

namespace DataLock{

	ChannelManager::ChannelManager(void)
	{
	}

	ChannelManager::~ChannelManager(void)
	{
	}

	UDPChannel* ChannelManager::get_channel(ACE_UINT32 id)
	{
		ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_lock_);

		UDPChannel* channel = channel_map_[id];

		return channel;
	}

	ACE_UINT32 ChannelManager::add_channel(UDPChannel* channel)
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
				channel_map_[id] = channel;
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

	UDPChannel* ChannelManager::get_channel_by_flag( ACE_UINT32 flag )
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
} // end namespace
