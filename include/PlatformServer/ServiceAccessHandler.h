#pragma once

#include "LogicHandlerFactory.h"
#include "ServiceManager.h"

namespace DataLock{

	// �������/�Ͽ�������
	class ServiceAccessHandler : public ILogicHandler
	{
	public:
		ServiceAccessHandler(void);
		virtual ~ServiceAccessHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);	
	};
}

