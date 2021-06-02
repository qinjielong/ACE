#include "ServiceAccessHandler.h"
#include <protocol/Service.pb.h>
#include "ChannelManager.h"
#include <ProtocolType.h>

namespace DataLock{

	ServiceAccessHandler::ServiceAccessHandler(void)
	{
	}

	ServiceAccessHandler::~ServiceAccessHandler(void)
	{
	}

	void ServiceAccessHandler::handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id)
	{
		switch(message_pack.cmd())
		{
		case CMD_SERVICE_JION:
			{
				TCPChannel* channel = Sgl_ChannelManager::instance()->get_channel(channel_id);

				if ( 0 == channel )
				{
					return ;
				}

				DataLock::ServiceJoinMessage* serviceJoin = new DataLock::ServiceJoinMessage;
				if( serviceJoin->ParseFromArray(message_pack.body().c_str(),message_pack.len()) )
				{
					// ע�����
					Sgl_ServiceManager::instance()->regist_service(
						serviceJoin->name(),
						serviceJoin->typeid_(),
						serviceJoin->serial(),
						serviceJoin->number(),
						channel->get_peer_addr().get_host_addr(),
						channel->get_peer_addr().get_port_number()
						);
				}

				// Ӧ��

				//channel->send()

				delete serviceJoin;
			}
			break;
		case CMD_SERVICE_LOGOUT:
			{
				DataLock::ServiceOutMessage* serviceOut = new DataLock::ServiceOutMessage;
				serviceOut->ParseFromArray(message_pack.body().c_str(),message_pack.len());
				// ж�ط���
				Sgl_ServiceManager::instance()->unregist_service(serviceOut->serial());

				delete serviceOut;
			}
		}

	}

}