#include "PlantformService.h"
#include "PlatformServerManager.h"


namespace DataLock
{
	PlantformService::PlantformService(void)
	{

	}

	PlantformService::~PlantformService(void)
	{

	}

	bool PlantformService::ServiceRun()
	{
		ACE::init();

#ifdef _WIN32
		this->m_Status.dwWin32ExitCode = S_OK;
		this->m_Status.dwCheckPoint = 0;
		this->m_Status.dwWaitHint = 0;
		this->m_Status.dwCurrentState = SERVICE_RUNNING;
		::SetServiceStatus(this->m_hServiceStatus, &this->m_Status); 
#endif

		//-µ¥ÀýÄ£Ê½
		if ( !DataLock::Sgl_PlatformServerManager::instance()->start() )
		{
			//Stop();
			return false;
		}

		return true;
	}

	void PlantformService::ServiceStop()
	{
		DataLock::Sgl_PlatformServerManager::instance()->stop();
		ACE::fini();
	}
}
