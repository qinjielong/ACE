#pragma once

#include <LogicHandler.h>
#include "Type.h"

namespace DataLock{
	
	// ƽ̨������
	class PlatformHandlerFactory
	{
	public:
		PlatformHandlerFactory();
		virtual ~PlatformHandlerFactory(void);

	public:
		virtual ILogicHandler* create_handler(INT32 type);

	};

}


