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
		// �ն�ϵͳ��־
		void ClientSysDeal(const MessagePack & message_pack);
		// �ն��û���־
		void UserLogDeal(const MessagePack & message_pack);
	};
}

