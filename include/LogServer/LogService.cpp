#include "LogService.h"
#include "LogServerManager.h"
#include "ace/Init_ACE.h"

namespace DataLock
{
	LogService::LogService(void)
	{

	}

	LogService::~LogService(void)
	{

	}

	bool LogService::ServiceRun()
	{
		//ACE::init();

#ifdef _WIN32
		this->m_Status.dwWin32ExitCode = S_OK;
		this->m_Status.dwCheckPoint = 0;
		this->m_Status.dwWaitHint = 0;
		this->m_Status.dwCurrentState = SERVICE_RUNNING;
		::SetServiceStatus(this->m_hServiceStatus, &this->m_Status); 
#endif

		if ( !DataLock::Sgl_LogServerManager::instance()->start() )
		{
			//Stop();

			return false;
		}

		return true;
	}

	void LogService::ServiceStop()
	{
		DataLock::Sgl_LogServerManager::instance()->stop();
		// 注：此处有问题，关闭ACE后，进程出现异常
		ACE::fini();
	}
}
