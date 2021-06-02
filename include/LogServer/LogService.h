#pragma once

//#include <Service/WindowsService.h>

//日志服务器


namespace DataLock
{
	class LogService     //: public CWindowsService
	{
	public:
		LogService(void);
		~LogService(void);
	public:
		// 执行服务
		virtual bool ServiceRun();
		// 停止服务
		virtual void ServiceStop();
	};

}
