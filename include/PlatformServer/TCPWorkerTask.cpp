#include "TCPWorkerTask.h"
#include "TCPChannel.h"
#include <Ace/OS.h>

namespace DataLock{

	TCP_Worker_Task::TCP_Worker_Task(void)
	{
	}

	TCP_Worker_Task::~TCP_Worker_Task(void)
	{
	}

	int TCP_Worker_Task::open()
	{
		return this->activate();
	}

	void TCP_Worker_Task::close()
	{
		ACE_Message_Block *pmb = new( ACE_Allocator::instance()->malloc( sizeof(ACE_Message_Block) ) ) 
			ACE_Message_Block(0, ACE_Message_Block::MB_STOP );

		this->putq(pmb);
	}

	bool TCP_Worker_Task::regist_message_handler(INT32 type,ILogicHandler* handler)
	{
		Logic_Handler_Map::iterator item = handler_map_.find(type);

		if( item == handler_map_.end() ) //表示没有找到
		{
			handler_map_.insert(std::make_pair(type,handler));
			return true;
		}

		return false;
	}

	void TCP_Worker_Task::unregist_message_handler(ILogicHandler* handler)
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

	//线程函数(这个线程与其他线程进行通讯的时候, 通过操纵消息队列来实现putq()、getq())
	int TCP_Worker_Task::svc()
	{
		while(true)
		{
			ACE_Message_Block* message = 0; //消息块

			ACE_Time_Value tvTmp = ACE_OS::gettimeofday();
			tvTmp += ACE_Time_Value(0,10000);
			this->getq(message,&tvTmp);   //设置时间获取消息
			
			if( -1 == TCPChannel::getq_rev(message) )
			{
				// 暂停10毫秒
				ACE_OS::sleep(ACE_Time_Value(0, 10000));
				continue;
			}

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

	void TCP_Worker_Task::route_message_pack(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id)
	{
		//消息处理表
		Logic_Handler_Map::iterator item = handler_map_.begin();
		for( ; item != handler_map_.end(); ++item)
		{
			//如果找到与注册的消息类型一致
			if( item->first == message_pack.type() )
			{
			//	item->second->handle((char*)(message_pack.body().c_str()),message_pack.len(),message_pack.cmd(),channel_id);
				//选择相应的处理器处理message_pack
				item->second->handle(message_pack,channel_id); 
				return ;
			}
		}
	}
}
