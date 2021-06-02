#pragma once

#include "LogicHandlerFactory.h"
#include "ServiceManager.h"

namespace DataLock{

	// 服务加入/断开处理器
	class ServiceAccessHandler : public ILogicHandler
	{
	public:
		ServiceAccessHandler(void);
		virtual ~ServiceAccessHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);	
	};
}

