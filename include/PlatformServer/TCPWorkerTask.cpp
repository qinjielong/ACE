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

		if( item == handler_map_.end() ) //��ʾû���ҵ�
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

	//�̺߳���(����߳��������߳̽���ͨѶ��ʱ��, ͨ��������Ϣ������ʵ��putq()��getq())
	int TCP_Worker_Task::svc()
	{
		while(true)
		{
			ACE_Message_Block* message = 0; //��Ϣ��

			ACE_Time_Value tvTmp = ACE_OS::gettimeofday();
			tvTmp += ACE_Time_Value(0,10000);
			this->getq(message,&tvTmp);   //����ʱ���ȡ��Ϣ
			
			if( -1 == TCPChannel::getq_rev(message) )
			{
				// ��ͣ10����
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
		//��Ϣ�����
		Logic_Handler_Map::iterator item = handler_map_.begin();
		for( ; item != handler_map_.end(); ++item)
		{
			//����ҵ���ע�����Ϣ����һ��
			if( item->first == message_pack.type() )
			{
			//	item->second->handle((char*)(message_pack.body().c_str()),message_pack.len(),message_pack.cmd(),channel_id);
				//ѡ����Ӧ�Ĵ���������message_pack
				item->second->handle(message_pack,channel_id); 
				return ;
			}
		}
	}
}
