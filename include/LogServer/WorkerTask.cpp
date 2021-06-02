#include "WorkerTask.h"
#include "UDPChannel.h"
#include "Ace/OS.h"

namespace DataLock{

	UDP_Worker_Task::UDP_Worker_Task(void)
	{
	}

	UDP_Worker_Task::~UDP_Worker_Task(void)
	{
	}

	int UDP_Worker_Task::open()
	{
		return this->activate(); //-激活这个线程
	}

	void UDP_Worker_Task::close()
	{
		ACE_Message_Block *pmb = new( ACE_Allocator::instance()->malloc( sizeof(ACE_Message_Block) ) ) 
			ACE_Message_Block(0, ACE_Message_Block::MB_STOP );

		this->putq(pmb);
	}

	bool UDP_Worker_Task::regist_message_handler(INT32 type,ILogicHandler* handler)
	{
		Logic_Handler_Map::iterator item = handler_map_.find(type);

		if( item == handler_map_.end() )
		{
			handler_map_.insert(std::make_pair(type,handler));
			return true;
		}

		return false;
	}

	void UDP_Worker_Task::unregist_message_handler(ILogicHandler* handler)
	{
		Logic_Handler_Map::iterator item = handler_map_.begin();
		for( ; item != handler_map_.end(); ++item)
		{
			if( item->second == handler )
			{
				delete item->second;
				handler_map_.erase(item);
				return ;
			}
		}
	}

	int UDP_Worker_Task::svc()
	{
		while(true)
		{
			//ACE_Message_Block* message = 0;
			//if( -1 == UDPChannel::getq_rev(message) )
			//{
			//	// 暂停10毫秒
			//	ACE_OS::sleep(ACE_Time_Value(0, 10000));
			//	continue;
			//}

			ACE_Message_Block* message = 0;

			ACE_Time_Value tvTmp = ACE_OS::gettimeofday();
			tvTmp += ACE_Time_Value(0,10000);
			this->getq(message,&tvTmp); //-getq是阻塞的，所以设置超时

			if ( 0 == message )
			{
				continue;
			}

			if ( 0 != message )
			{
				if( message->msg_type() == ACE_Message_Block::MB_STOP )
				{
					printf("msg MB_STOP type.\n");
					message->release();
					break;
				}
			}

			ACE_UINT32 channel_id = 0;
			DataLock::MessagePack* message_pack = new DataLock::MessagePack;

			channel_id = (ACE_UINT32)(*message->rd_ptr());
			if( message_pack->ParseFromArray(message->rd_ptr()+sizeof(ACE_UINT32),message->length()-sizeof(ACE_UINT32)) )
			{
				this->route_message_pack(*message_pack,channel_id);
			}
			
			message->release();
			delete message_pack;
		}

		return 0;
	}

	void UDP_Worker_Task::route_message_pack(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id)
	{
		Logic_Handler_Map::iterator item = handler_map_.begin();
		for( ; item != handler_map_.end(); ++item)
		{
			if( item->first == message_pack.type() )
			{
			//	item->second->handle((char*)(message_pack.body().c_str()),message_pack.len(),message_pack.cmd(),channel_id);
				item->second->handle(message_pack,channel_id);
				return ;
			}
		}
	}
}
