#pragma once

#include "LogicHandler.h"
#include <vector>
#include "Type.h"

// 客户端消息处理器
// 远程命令处理器
namespace DataLock{

	class RemoteCmdHandler  : public ILogicHandler
	{
	public:
		RemoteCmdHandler(void);
		~RemoteCmdHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		typedef std::vector<std::string> UserIDArray;
	private:
		// 解析用户ID
		void ResolveUserIDs(std::string userIDs,UserIDArray & array);
		// 通告消息处理
		void NotifyMessageDeal(const MessagePack & message_pack);
		// 发送变更及远程命令消息到客户端
		void SendNotfiyMessageToClient(INT32 protocolID,INT32 cmd,unsigned long userID);
	};

}