#pragma once

//#include <Service/WindowsService.h>

//��־������


namespace DataLock
{
	class LogService     //: public CWindowsService
	{
	public:
		LogService(void);
		~LogService(void);
	public:
		// ִ�з���
		virtual bool ServiceRun();
		// ֹͣ����
		virtual void ServiceStop();
	};

}
