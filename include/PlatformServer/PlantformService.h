#pragma once

//#include <Service/WindowsService.h>

namespace  DataLock{
	class PlantformService //: public CWindowsService
	{
	public:
		PlantformService(void);
		~PlantformService(void);
	public:
		// ִ�з���
		virtual bool ServiceRun();
		// ֹͣ����
		virtual void ServiceStop();
	};

}
