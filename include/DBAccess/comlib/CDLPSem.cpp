/******************************************************************************
功能说明：信号量
******************************************************************************/

#include "ISystemLayInterface.h"

/********************************************************************************************
  Function		: CDLPSem   
  DateTime		: 	
  Description	: 创建信号量
  Input			: const char *czKey ：信号量的名称；BOOL bProcess：是否是进程间的信号量
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
#ifdef WINCE
HANDLE OpenSemaphore(DWORD dwState,bool bFlag,WCHAR* wcKey)
{
	return  CreateSemaphore(NULL,DLP_INIT_SEM_COUNT,DLP_MAX_SEM_COUNT,wcKey);
}
#endif

CDLPSem::CDLPSem( const char *czKey,BOOL bProcess)
{
	m_bIsValid=FALSE;
	m_DLPSem=NULL;


#ifdef _WIN32

	CHAR * wcKey = (CHAR *)(czKey);

	if(NULL == czKey)
	{
		return;
	}

	//判断信号量是否已存在，若存在，则打开信号量
	if( (m_DLPSem=OpenSemaphoreA(SEMAPHORE_ALL_ACCESS,FALSE,wcKey))==NULL)
	{
		//如果没有其他进程创建这个信号灯，则创建信号量，如执行成功，返回信号量对象的句柄；零表示出错
		m_DLPSem = CreateSemaphoreA(NULL,DLP_INIT_SEM_COUNT,DLP_MAX_SEM_COUNT,wcKey);
	} 

	if (m_DLPSem)
	{
		m_bIsValid=TRUE;
	}

#elif _LINUX

	char *wcKey=(char *)(czKey);
	if (NULL==czKey)
	{
		return;
	}

	//判断信号量是否已存在，若存在，则打开信号量
	if ((m_DLPSem=sem_open(wcKey, 0))==SEM_FAILED)
	{
		//创建信号灯，成功时返回指向信号量的指针，出错时为SEM_FAILED
		if ((m_DLPSem=sem_open(wcKey,O_RDWR | O_CREAT | O_EXCL,0644, 1))==SEM_FAILED)
		{
			m_bIsValid=FALSE;
			return;
		}
	}
	m_bIsValid=TRUE;

#endif

}

//析构函数
/********************************************************************************************
  Function		: ~CDLPSem   
  DateTime		: 
  Description	: 释放信号量的指针
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
CDLPSem::~CDLPSem()
{

	if (m_DLPSem)
	{
#ifdef _WIN32
		CloseHandle(m_DLPSem);
		m_DLPSem=NULL;
#elif _LINUX
		sem_close(m_DLPSem);
		m_DLPSem=NULL;
#endif

	}
	m_bIsValid=FALSE;

}


/********************************************************************************************
  Function		: IsValid   
  DateTime		: 
  Description	: 返回信号量的有效标识
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPSem::IsValid()
{
	return m_bIsValid;
}

//发送信号量
/********************************************************************************************
  Function		: Signal    
  DateTime		: 	
  Description	: 递增一个信号量计数
  Input			:  NULL
  Output		:         	// 对输出参数的说明。
  Return		: TRUE表示成功，FALSE表示失败
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPSem::Signal()
{
	BOOL bRet=TRUE;
#ifdef _WIN32

	//递增一个信号量计数,TRUE表示成功，FALSE表示失败。
	bRet=ReleaseSemaphore(m_DLPSem, 1, NULL);

#elif _LINUX

	//把所指定的信号量的值加1，然后唤醒正在等待该信号灯值变为正数的任意线程。若成功则返回0，否则返回-1。
	if(sem_post(m_DLPSem)!=0)
	{
		bRet=FALSE;
	}

#endif
	return bRet;
}


/********************************************************************************************
  Function		: Wait    
  DateTime		: 
  Description	: 等待获取信号量
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE，失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPSem::Wait()
{

	BOOL bRet=TRUE;

#ifdef _WIN32

	bRet=WaitForSingleObject(m_DLPSem, INFINITE) == WAIT_OBJECT_0 ? TRUE :FALSE;

#elif _LINUX

	if(sem_wait(m_DLPSem)!=0)
	{
		bRet=FALSE;
	}

#endif

	return bRet;
}

/********************************************************************************************
Function		: Wait    
DateTime		: 
Description		: 等待一段时间获取信号量
Input			: UINT mSeconds：等待的时间，毫秒
Output			: NULL
Return			: 成功返回TRUE，失败返回FALSE
Note			:				// 备注
********************************************************************************************/
BOOL CDLPSem::Wait(UINT mSeconds)
{
	BOOL bRet=TRUE;

#ifdef _WIN32

	DWORD dwWaitTime=(DWORD)mSeconds;
	bRet= WaitForSingleObject(m_DLPSem, dwWaitTime) == WAIT_TIMEOUT  ? FALSE :TRUE;

#elif _LINUX

	struct timeval struTimeVal;
	struct timespec struTimeSpec;
	gettimeofday(&struTimeVal, NULL);
	struTimeSpec.tv_sec  = mSeconds/1000;
	struTimeSpec.tv_nsec =1000L *(struTimeVal.tv_usec+(mSeconds-struTimeSpec.tv_sec*1000)*1000L);
	struTimeSpec.tv_sec += struTimeVal.tv_sec;

	if(sem_timedwait(m_DLPSem,  &struTimeSpec)!=0)
	{
		bRet=FALSE;
	}

#endif

	return bRet;
}
