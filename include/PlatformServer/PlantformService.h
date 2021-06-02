#pragma once

//#include <Service/WindowsService.h>

namespace  DataLock{
	class PlantformService //: public CWindowsService
	{
	public:
		PlantformService(void);
		~PlantformService(void);
	public:
		// 执行服务
		virtual bool ServiceRun();
		// 停止服务
		virtual void ServiceStop();
	};

}
