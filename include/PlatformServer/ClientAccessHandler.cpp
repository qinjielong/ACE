#include "ClientAccessHandler.h"
#include "PlatformServerManager.h"
#include "ChannelManager.h"
#include <ProtocolType.h>
#include <protocol/ClientJoin.pb.h>
#include "ace/Message_Block.h"
#include <Json/json.h>


namespace DataLock{

	ACE_Thread_Mutex ClientAccessHandler::mutex_;

	ClientAccessHandler::ClientAccessHandler(void)
	{
	}

	ClientAccessHandler::~ClientAccessHandler(void)
	{
	}

	void ClientAccessHandler::handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id)
	{
		ServiceInforMap    inforMap;
		ClientJoinMessage* client = new ClientJoinMessage;

		//�����ͻ�������Ϣ
		if ( !client->ParseFromArray(message_pack.body().c_str(),message_pack.len()) )
		{
			delete client;
			return ;
		}

		/////////////////////////////
		// ������֤�ͻ��˺Ϸ���
		//
		/////////////////////////////

		FindServicesToMap(client->srvid(),inforMap);

		INT32   length = 0 ;
		char*   buffer = 0 ;
		char*   body   = 0 ;
		string  srvsJson;
		ServiceInforMap::iterator item = inforMap.begin();

		if ( !inforMap.empty() )
		{
			// ���л�ΪJSON��ʽ
			Json::Value root;
			Json::FastWriter writer;
			
			for( ; item != inforMap.end();++item )
			{
				Json::Value person;
				person["type"] = item->first;
				person["linkName"] = item->second;
				root.append(person);             //׷��
			}
			srvsJson = writer.write(root);
		}
		
		// Ӧ����Ϣ
		ClientJoinMessageRsp* joinMessageRsp = new ClientJoinMessageRsp;

		joinMessageRsp->set_res(true);
		joinMessageRsp->set_servers(srvsJson);
		length = joinMessageRsp->ByteSize();
		buffer   = new char[length];
		//���л�
		joinMessageRsp->SerializeToArray(buffer,length);

		
		//��Ϣ��
		DataLock::MessagePack* rsp_message_pack = new DataLock::MessagePack;
		rsp_message_pack->set_id(message_pack.id());
		rsp_message_pack->set_cmd(CMD_CLIENT_JION_RES);
		rsp_message_pack->set_type(PROTOCOL_CLIENT_JION);
		rsp_message_pack->set_body(buffer,length);
		rsp_message_pack->set_len(length);
		length = rsp_message_pack->ByteSize();

		body = new char[length];
		rsp_message_pack->SerializeToArray(body,length);

		// ���ͷ�����Ϣ
		Sgl_ChannelManager::instance()->get_channel(channel_id)->send(body,length);

		delete buffer;
		delete body;

		delete client;
		delete joinMessageRsp;
		delete rsp_message_pack;
	}

	void ClientAccessHandler::FindServicesToMap(INT32 id,ServiceInforMap & map)
	{
		if( 0 == id )
		{
			// ��־������
			GetServiceToMap(SERVER_LOG,map);
			// ����������
			GetServiceToMap(SERVER_UPDATE,map);
		}
		else
		{
			//ƽ̨������
			GetServiceToMap(id,map);
		}
	}

	// ��ȡ����������Ϣ
	void ClientAccessHandler::GetServiceToMap(INT32 id,ServiceInforMap & map)
	{
		Service* service = Sgl_ServiceManager::instance()->get_service_infor(id);

		if(0 != service)
		{
			char link_name[128] = {0x00};
			//::sprintf_s(link_name,"%s%d",service->ip.c_str(),service->port);
			::sprintf(link_name,"%s%d",service->ip.c_str(),service->port);
			map.insert(make_pair(service->id,link_name));
		}
	}

}