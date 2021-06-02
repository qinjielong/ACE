
#include "ISystemLayInterface.h"

/******************************************************************************
功能说明：普通锁
******************************************************************************/

/********************************************************************************************
  Function		: CDLPMutex   
  DateTime		: 	
  Description	: 初始化锁
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
CDLPMutex::CDLPMutex()
{
#ifdef _WIN32
	InitializeCriticalSection(&m_DLPMutex);
#elif _LINUX
	pthread_mutex_init( &m_DLPMutex,NULL);
#endif

}


/********************************************************************************************
  Function		: ~CDLPMutex   
  DateTime		: 	
  Description	: 删除锁
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
CDLPMutex::~CDLPMutex()
{
	//Unlock();

#ifdef _WIN32
	DeleteCriticalSection(&m_DLPMutex);
#elif _LINUX
	pthread_mutex_destroy(&m_DLPMutex);
#endif

}


/********************************************************************************************
  Function		: Lock   
  DateTime		: 	
  Description	: 加锁
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPMutex::Lock()
{
	BOOL bRet=TRUE;

#ifdef _WIN32
	EnterCriticalSection(&m_DLPMutex);
#elif _LINUX
	if(pthread_mutex_lock(&m_DLPMutex)!=0)
	{
		bRet=FALSE;
	};
#endif
	return bRet;
}

//
/********************************************************************************************
  Function		: TryLock   
  DateTime		: 	
  Description	: 非阻塞的加锁
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPMutex::TryLock()
{
	BOOL bRet=TRUE;

#ifdef _WIN32
	bRet=TryEnterCriticalSection(&m_DLPMutex);
#elif _LINUX
	if (pthread_mutex_trylock(&m_DLPMutex)!=0)
	{
		bRet=FALSE;
	}

#endif
	return bRet;
}


/********************************************************************************************
  Function		: Unlock    
  DateTime		: 	
  Description	: 解锁
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
void CDLPMutex::Unlock()
{
#ifdef _WIN32
		LeaveCriticalSection(&m_DLPMutex);
#elif _LINUX
		pthread_mutex_unlock(&m_DLPMutex);	
#endif

}


