#pragma once

#include <LogicHandler.h>
#include "Type.h"

namespace DataLock{
	
	// 平台处理工厂
	class PlatformHandlerFactory
	{
	public:
		PlatformHandlerFactory();
		virtual ~PlatformHandlerFactory(void);

	public:
		virtual ILogicHandler* create_handler(INT32 type);

	};

}


