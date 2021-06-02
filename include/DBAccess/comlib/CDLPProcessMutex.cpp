
#include "ISystemLayInterface.h"

/******************************************************************************
功能说明：进程间的锁
******************************************************************************/

/********************************************************************************************
Function		: CDLPProcessMutex   
DateTime		: 	
Description		: 创建进程锁
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// 备注
********************************************************************************************/
CDLPProcessMutex::CDLPProcessMutex(const char *czKey)
{
#ifdef _WIN32
	
	m_DLPProcessMutex=NULL;
	if (czKey==NULL)
	{
		return;
	}
	LPCSTR lpMutexName=(CHAR *)(czKey);
	m_DLPProcessMutex=CreateMutexA(NULL, FALSE, lpMutexName);
	
#elif _LINUX

	m_DLPProcessMutex=0;
	if (czKey==NULL)
	{
		return;
	}
	m_DLPProcessMutex=open(czKey,O_RDWR|O_CREAT,S_IWRITE);
	if(m_DLPProcessMutex<0)
	{
		return;
	}

#endif

}

/********************************************************************************************
Function		: ~CDLPProcessMutex   
DateTime		: 
Description		: 删除锁
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// 备注
********************************************************************************************/
CDLPProcessMutex::~CDLPProcessMutex()
{

#ifdef _WIN32
	if (m_DLPProcessMutex)
	{
		CloseHandle(m_DLPProcessMutex);
		m_DLPProcessMutex=NULL;
	}	
#elif _LINUX
	close(m_DLPProcessMutex);
#endif

}

/********************************************************************************************
Function		: LockProcess   
DateTime		: 	
Description		: 加锁
Input			: NULL
Output			: NULL
Return			: 成功返回TRUE,失败返回FALSE
Note			:				// 备注
********************************************************************************************/
BOOL CDLPProcessMutex::LockProcess()
{

#ifdef _WIN32
	if (m_DLPProcessMutex)
	{
		return	WaitForSingleObject(m_DLPProcessMutex, INFINITE)==WAIT_OBJECT_0 ? TRUE :FALSE;
	}
	else
	{
		return FALSE;
	}
#elif _LINUX

	if(lockf(m_DLPProcessMutex,F_LOCK,0)<0) //参数使用F_LOCK，则如果已经加锁，则阻塞到前一个进程释放锁
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#endif


}

/********************************************************************************************
Function		: TryLockProcess   
DateTime		: 	
Description		: 非阻塞的加锁
Input			: NULL
Output			: NULL
Return			: 成功返回TRUE,失败返回FALSE
Note			:				// 备注
********************************************************************************************/
BOOL CDLPProcessMutex::TryLockProcess()
{

#ifdef _WIN32
	if (m_DLPProcessMutex)
	{
		return	WaitForSingleObject(m_DLPProcessMutex, 0)==WAIT_OBJECT_0 ? TRUE :FALSE;
	}
	else
	{
		return FALSE;
	}
#elif _LINUX

	if(lockf(m_DLPProcessMutex,F_TLOCK,0)<0) //参数使用F_TLOCK，当文件已经被加锁时，调用进程不会阻塞而是直接打印错误信息并返回
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#endif

}

/********************************************************************************************
Function		: UnlockProcess    
DateTime		: 	
Description		: 解锁
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// 备注
********************************************************************************************/
void CDLPProcessMutex::UnlockProcess()
{

#ifdef _WIN32
	if (m_DLPProcessMutex)
	{
		ReleaseMutex(m_DLPProcessMutex);
	}	
#elif _LINUX
	lockf(m_DLPProcessMutex,F_ULOCK,0);
#endif

}
