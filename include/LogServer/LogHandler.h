#pragma once

#include "LogicHandler.h"

namespace DataLock{

	class LogHandler : public ILogicHandler
	{
	public:
		LogHandler(void);
		virtual ~LogHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);
	private:
		// 终端系统日志
		void ClientSysDeal(const MessagePack & message_pack);
		// 终端用户日志
		void UserLogDeal(const MessagePack & message_pack);
	};
}

