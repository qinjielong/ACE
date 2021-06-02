#include "RemoteCmdHandler.h"
#include "UserManager.h"
#include <ProtocolType.h>
#include <protocol/MessagePack.pb.h>
#include <protocol/ClientUser.pb.h>
#include "ChannelManager.h"
#include <ace/Date_Time.h>
//#include <lexical_cast.h>

namespace DataLock{

	RemoteCmdHandler::RemoteCmdHandler(void)
	{
	}

	RemoteCmdHandler::~RemoteCmdHandler(void)
	{
	}

	void RemoteCmdHandler::ResolveUserIDs( std::string userIDs,UserIDArray & array )
	{
		array.clear();

		std::string flag = ",";
		std::string::size_type fpos = 0;
		std::string::size_type bpos = 0;

		while ( std::string::npos != (bpos = userIDs.find(flag,fpos)) )
		{
			std::string id = userIDs.substr(fpos,bpos-fpos);
			fpos = bpos+1;

			array.push_back(id);
		}

		std::string id = userIDs.substr(fpos);
		array.push_back(id);
	}

	void RemoteCmdHandler::handle( const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id )
	{
		//////////////////////////////////////////////////////////////////////////
		// 管理端更新和远程卸载处理

		// 用户信息更新
		if ( PROTOCOL_CLIENT_USER == message_pack.type() && CMD_USER_INFOR_CHANGE == message_pack.cmd() )
		{
			NotifyMessageDeal(message_pack);
		}
		// 策略更新
		else if (PROTOCOL_POLICY == message_pack.type() && CMD_CHANGE_POLICY == message_pack.cmd())
		{
			NotifyMessageDeal(message_pack);
		}
		// 权限变更
		else if (PROTOCOL_AUTHORITY == message_pack.type() && CMD_CHANGE_AUTHORITY == message_pack.cmd())
		{
		//	NotifyMessageDeal(message_pack);
		}
		// 客户端远程卸载
		else if (PROTOCOL_CLIENT_REMOTE_CMD == message_pack.type() && CMD_UNINSTALL == message_pack.cmd())
		{
			NotifyMessageDeal(message_pack);
		}

		//////////////////////////////////////////////////////////////////////////
		// 客户端信息同步处理
		if (PROTOCOL_CLIENT_USER == message_pack.type() && CMD_USER_INFOR_SYN == message_pack.cmd())
		{
			DataLock::ClientUserInfor* userInfor = new DataLock::ClientUserInfor;

			if ( !userInfor->ParseFromArray(message_pack.body().c_str(),message_pack.len()) )
			{
				delete userInfor;
				return ;
			}

			ACE_TCHAR ip[25] = {0x00};
			ACE_TCHAR time[21] = {0x00};
			ACE_Date_Time tvTime(ACE_OS::gettimeofday()); 

			TCPChannel* channel = Sgl_ChannelManager::instance()->get_channel(channel_id);
			channel->get_peer_addr().addr_to_string(ip,sizeof(ip));
			channel->set_flag(userInfor->userid());

			sprintf(time,"%d-%d-%d %d:%d:%d",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());

			userInfor->set_ip(ip);
			userInfor->set_longintime(time);

#ifdef _DEBUG
			printf("%s : syn machinename=%s channel id=%d ip=%s userid=%d\n",time,userInfor->machinename().c_str(),channel_id,ip,channel->get_flag());
#endif

			// 更新到数据库
			Sgl_UserManager::instance()->UpdateUserInfor(*userInfor);

			delete userInfor;
		}
	}

	void RemoteCmdHandler::NotifyMessageDeal( const MessagePack & message_pack )
	{
		DataLock::ManagerNotifyMessage* msg =  new DataLock::ManagerNotifyMessage;
		msg->ParseFromArray(message_pack.body().c_str(),message_pack.len());
		std::string ids = msg->ids();

		// 解析用户信息
		UserIDArray array;

		ResolveUserIDs(ids,array);

		for ( size_t i=0; i < array.size();i++ )
		{
			// 发送消息到客户端
			//SendNotfiyMessageToClient(message_pack.type(),message_pack.cmd(),lexical_cast<unsigned long>(array[i]));
		}

		delete msg;
	}

	void RemoteCmdHandler::SendNotfiyMessageToClient(INT32 protocolID,INT32 cmd,unsigned long userID )
	{
		// 查询客户端通道
		TCPChannel* channel = Sgl_ChannelManager::instance()->get_channel_by_flag(userID);

		if ( 0 == channel)
		{
			return ;
		}
		
		INT32   length = 0 ;
		char*   buffer = 0 ;
		DataLock::MessagePack* messge = new DataLock::MessagePack;

		messge->set_id("");
		messge->set_cmd(cmd);
		messge->set_type(protocolID);
		messge->set_body(0,0);
		messge->set_len(0);
		length = messge->ByteSize();
		buffer = new char[length];
		messge->SerializeToArray(buffer,length);

		// 发送服务信息
		channel->send(buffer,length);
	}

}