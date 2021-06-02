#include "ISystemLayInterface.h"
/******************************************************************************
功能说明：线程部分
******************************************************************************/

/********************************************************************************************
  Function		: CDLPThread    
  DateTime		: 	
  Description	: CDLPThread类中变量的初始化
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			: NULL
********************************************************************************************/
CDLPThread::CDLPThread()
{
	//初始化变量标识
	m_bExit = FALSE;
	m_bExiting = FALSE;
	m_bPause = FALSE;
	m_bRunninDLPtate = FALSE;
	m_bJoin=FALSE;
	m_bDetached=FALSE;
	m_bMutexLock =	FALSE;
	m_bthreadfinish = FALSE;

	m_DLPCond = NULL;
	m_DLPCondPause = NULL;
	m_DLPMutexPause=NULL;
	m_DLPMutexUnit = NULL;
	m_DLPMutexthreadfinish = NULL;
	

	if(!m_DLPCond)
	{
		m_DLPCond = new CDLPCond;
	}
	if (!m_DLPCondPause)
	{
		m_DLPCondPause = new	CDLPCond;
	}
	if(!m_DLPMutexPause)
	{
		m_DLPMutexPause = new CDLPMutex;
	}
	if (!m_DLPMutexUnit)
	{
		m_DLPMutexUnit = new CDLPMutex;
	}
	if (!m_DLPMutexthreadfinish)
	{
		m_DLPMutexthreadfinish = new CDLPMutex;
	}

	m_fnUser = NULL;
	m_pFnUserParam = NULL;

#ifdef	_WIN32	
	m_DLPThread = NULL;
#endif

}

//析构函数
/********************************************************************************************
  Function		: ~CDLPThread   
  DateTime		: 	
  Description	: 释放CDLPThread类中的资源
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			: NULL
********************************************************************************************/
CDLPThread::~CDLPThread()
{
	Stop();
	if (m_DLPThread)
	{
		m_DLPMutexUnit->Lock();
		UnInitData();
		m_DLPMutexUnit->Unlock();
	}	
	if(m_DLPCond)
	{
		delete m_DLPCond;
		m_DLPCond = NULL;
	}
	if (m_DLPCondPause)
	{
		delete	m_DLPCondPause;
		m_DLPCondPause = NULL;
	}
	if(m_DLPMutexPause)
	{
		delete m_DLPMutexPause;
		m_DLPMutexPause = NULL;
	}
	if (m_DLPMutexUnit)
	{
		delete	m_DLPMutexUnit;
		m_DLPMutexUnit = NULL;
	}
	if (m_DLPMutexthreadfinish)
	{
		delete m_DLPMutexthreadfinish;
		m_DLPMutexthreadfinish = NULL;
	}
	
}

/********************************************************************************************
  Function		: UnInitData    
  DateTime		: 	
  Description	: widows:释放线程句柄
				  linux: 分离程序
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			: NULL
********************************************************************************************/
void CDLPThread::UnInitData()
{
	
#ifdef _WIN32
	if(m_DLPThread)
	{
		CloseHandle(m_DLPThread);
		m_DLPThread = NULL;
	}
#elif _LINUX
	Detach();
#endif
	m_bRunninDLPtate = FALSE;
}

//CDLPThread线程过程
/********************************************************************************************
  Function		: CDLPThreadProcFunc    
  DateTime		: 	
  Description	: 执行线程
  Input			: HANDLE param：线程函数的参数
  Output		: NULL
  Return		: windows:返回0，linux:返回线程函数的参数指针
  Note			: NULL
********************************************************************************************/
#ifdef _WIN32
//DWORD WINAPI CDLPThreadProcFunc(HANDLE param)
unsigned   _stdcall CDLPThreadProcFunc(HANDLE param)
#elif _LINUX
void *CDLPThreadProcFunc(HANDLE param)
#endif 
{	
	CDLPThread * pDLPThread = (CDLPThread *)param;

#ifdef _WIN32
	DWORD dwRet = 0;
	if(! pDLPThread)
	{
		return dwRet;
	}
#endif
	//设置线程运行状态

	if(pDLPThread->m_fnUser)
	{
		//执行用户线程函数
		pDLPThread->m_fnUser(pDLPThread,pDLPThread->m_pFnUserParam);
	}
	//发送信号，唤醒join等待
	//pDLPThread->m_DLPCond->Signal();

	pDLPThread->m_DLPMutexthreadfinish->Lock();
	pDLPThread->m_bthreadfinish = TRUE;
	pDLPThread->m_bRunninDLPtate = FALSE;	
	pDLPThread->m_DLPMutexthreadfinish->Unlock();

	pDLPThread->m_bExit = TRUE;
	//释放资源
	pDLPThread->UnInitData();
	if (pDLPThread->m_bMutexLock)
	{
		pDLPThread->m_DLPMutexUnit->Unlock();
		pDLPThread->m_bMutexLock = FALSE;
	}
	//发送信号，唤醒join等待
	pDLPThread->m_DLPCond->Signal();

#ifdef _WIN32	
	return dwRet;
#elif _LINUX
	return pDLPThread;
#endif 
}


//CDLPThread线程开始
/********************************************************************************************
  Function		: Start    
  DateTime		: 	
  Description	: 创建线程
  Input			: DLPThreadCallbackFunction fnOnEvent：用户线程函数；void *pUserParam：用户线程参数
  Output		: NULL
  Return		: 成功返回TRUE，失败返回FALSE
  Note			: NULL
********************************************************************************************/
BOOL CDLPThread::Start(DLPThreadCallbackFunction fnOnEvent/* =NULL */, void *pUserParam/* =NULL */)
{
	m_fnUser = fnOnEvent;
	m_pFnUserParam = pUserParam;
	m_bRunninDLPtate=FALSE;
	if (m_bMutexLock)
	{
		return	FALSE;
	}
	m_DLPMutexUnit->Lock();
	m_bMutexLock = TRUE;

#ifdef WINCE
	m_DLPThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CDLPThreadProcFunc,HANDLE(this),0,NULL);
	if (m_DLPThread)
	{
		m_DLPMutexthreadfinish->Lock();
		if (m_bthreadfinish)
		{
			m_bRunninDLPtate=FALSE;
			//释放资源
			UnInitData();
		}
		else
		{
			m_bRunninDLPtate=TRUE;
		}
		m_DLPMutexthreadfinish->Unlock();
	}
#elif _WIN32
	//创建线程
	//m_DLPThread = CreateThread(NULL,0,CDLPThreadProcFunc,HANDLE(this),0,NULL);

	//创建线程
	m_DLPThread = (HANDLE)_beginthreadex(NULL,0,CDLPThreadProcFunc,HANDLE(this),0,NULL);
	
	if (m_DLPThread)
	{
		m_DLPMutexthreadfinish->Lock();
		if (m_bthreadfinish)
		{
			m_bRunninDLPtate=FALSE;
			//释放资源
			UnInitData();
		}
		else
		{
			m_bRunninDLPtate=TRUE;
		}
		m_DLPMutexthreadfinish->Unlock();
	}
#elif _LINUX
	//创建线程
	if(pthread_create(&m_DLPThread,0,CDLPThreadProcFunc,HANDLE(this))==0)
	{
		m_DLPMutexthreadfinish->Lock();
		if (m_bthreadfinish)
		{
			m_bRunninDLPtate=FALSE;
			//释放资源
			UnInitData();
		}
		else
		{
			m_bRunninDLPtate=TRUE;
		}
		m_DLPMutexthreadfinish->Unlock();
	}
#endif 
	else
	{
		m_DLPMutexUnit->Unlock();
		m_bMutexLock = FALSE;
	}

	return m_bRunninDLPtate;
}


//停止线程
/********************************************************************************************
  Function		: Stop    
  DateTime		: 	
  Description	: 设置停止线程标识
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			: NULL
********************************************************************************************/
BOOL CDLPThread::Stop()
{
	//判断线程是否在运行
	if(m_bRunninDLPtate)
	{
		//设置停止线程标识
		m_bExiting = TRUE;
	}
	return m_bExiting;

}

/********************************************************************************************
  Function		: IsRunning    
  DateTime		: 	
  Description	: 返回线程运行状态
  Input			: NULL
  Output		: NULL
  Return		: 线程正在运行返回TRUE,否则返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::IsRunning()
{
	return m_bRunninDLPtate;
}

//线程挂起
/********************************************************************************************
  Function		: Suspend   
  DateTime		: 	
  Description	: 设置线程休眠状态
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::Suspend()
{
	//判断线程运行状态
	if(m_bRunninDLPtate)
	{
		//设置线程休眠状态
		m_DLPMutexPause->Lock();
		m_bPause = TRUE;
		m_DLPMutexPause->Unlock();
	}

	return m_bPause;

}

//线程睡眠
/********************************************************************************************
  Function		: Resume    
  DateTime		: 	
  Description	: 发送信号唤醒线程，设置线程休眠状态
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::Resume()
{
	if(m_bRunninDLPtate)
	{
		//判断线程休眠状态
		if(m_bPause)
		{
			//唤醒线程
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();
		}
	}
	//返回线程休眠状态
	return m_bPause;
}

//等待线程结束
/********************************************************************************************
  Function		: Join   
  DateTime		: 	
  Description	: 等待线程正常退出
  Input			: NULL
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::Join()
{
	//判断线程是否在运行，是否已join过
	if(m_bRunninDLPtate&&!m_bJoin)
	{
		//判断线程的休眠状态
		if(m_bPause)
		{
			//唤醒线程
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//设置线程退出标识
		m_bExiting = TRUE;

#ifdef _WIN32
		//等待线程退出
		if(0 == m_DLPCond->Wait())
		{
			//设置线程退出标志位和join标志位
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}
#elif _LINUX
		//等待线程退出
		if (pthread_join(m_DLPThread,NULL)==0)
		{
			m_bJoin=TRUE;
		}		
#endif
	}
	//返回线程Join标志位
	return m_bJoin;

}


/********************************************************************************************
  Function		: Join  
  DateTime		: 
  Description	: 等待线程一段时间，判断是否退出 
  Input			: INT mseconds:等待时间，毫秒
  Output		: NULL
  Return		: 成功返回TRUE,失败返回FALSE
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::Join(INT mseconds)
{
	//判断线程是否在运行，是否已join过
	if(m_bRunninDLPtate&&!m_bJoin)
	{
		//判断线程的休眠状态
		if(m_bPause)
		{
			//唤醒线程
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//设置线程退出标识
		m_bExiting = TRUE;

#ifdef _WIN32
		//等待线程退出
		if(0 == m_DLPCond->WaitTimeout(mseconds))
		{
			//设置线程退出标志位和join标志位
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}

#elif _LINUX
		//等待线程退出
		if (pthread_join(m_DLPThread,NULL)==0)
		{
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}		
#endif
	}
	//返回线程Join标志位
	return m_bJoin;

}

/********************************************************************************************
  Function		: Cancel   
  DateTime		: 	
  Description	: 强制退出线程
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
void CDLPThread::Cancel()
{
	//判断线程运行状态
	if(m_bRunninDLPtate)
	{
		//设置线程退出状态
		m_bExiting = TRUE;
		
		//判断线程休眠状态
		if(m_bPause)
		{
			//唤醒线程
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//判断线程退出状态
		if(!m_bExit)
		{
			//强制终止线程
			Kill();
		}
	}

}

/********************************************************************************************
  Function		: TestExit   
  DateTime		: 	
  Description	: 检测线程的退出状态，执行线程的join唤醒和休眠
  Input			: NULL
  Output		: NULL
  Return		: 返回TRUE，线程已退出，返回FALSE,线程未退出
  Note			:				// 备注
********************************************************************************************/
BOOL CDLPThread::TestExit()
{

	//判断线程退出标识
	if(m_bExiting==TRUE)
	{
		m_bExit = TRUE;
	}
	//判断线程的休眠状态
	else if(m_bPause==TRUE)
	{
		//线程休眠
		m_DLPMutexPause->Lock();
		if (m_bPause)
		{
			m_DLPMutexPause->Unlock();
			m_DLPCondPause->Wait();			
		}
		else
		{
			m_DLPMutexPause->Unlock();
		}
		
	}
	
	//释放资源改为线程执行完毕后释放，在线程代理函数的结尾释放，hf于2010.11.01修改
	//判断线程的退出状态
	//if (m_bExit==TRUE)
	//{
	//	//释放线程句柄
	//	UnInitData();
	//	return TRUE;
	//}

	return m_bExit;
}


/********************************************************************************************
  Function		: Kill    
  DateTime		: 	
  Description	: 强制终止线程
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
void CDLPThread::Kill()
{
	//判断线程运行状态和退出状态
	if(m_bRunninDLPtate || (! m_bExit))
	{
#ifdef _WIN32

		//终止线程
		DWORD dwExitCode;
		GetExitCodeThread(m_DLPThread,&dwExitCode);
		TerminateThread(m_DLPThread,dwExitCode);
#elif _LINUX
		//强制退出线程
		pthread_cancel(m_DLPThread);
#endif
		//释放线程句柄
		UnInitData();
		if (m_bMutexLock)
		{
			m_DLPMutexUnit->Unlock();
			m_bMutexLock = FALSE;
		}
	}
}

/********************************************************************************************
  Function		:  Detach   
  DateTime		: 	
  Description	:  分离线程
  Input			:  NULL
  Output		:  NULL
  Return		:  NULL
  Note			:				// 备注
********************************************************************************************/
void CDLPThread::Detach()
{
#ifdef _LINUX
	//判断线程的分离标识和Join标识
	if (!m_bDetached&&!m_bJoin)
	{
		//分离线程
		if (pthread_detach(m_DLPThread)==0)
		{
			m_bDetached=TRUE;
		}		
	}
#endif

}
/********************************************************************************************
Function		:  GetThreadHandle   
DateTime		: 	
Description		:  获取线程句柄
Input			:  NULL
Output			:  DLPThread:线程句柄，失败返回NULL
Return			:  NULL
Note			:				// 备注
********************************************************************************************/
DLPThread CDLPThread::GetThreadHandle()
{

	if (m_DLPThread)
	{
		return	m_DLPThread;
	}
	return	NULL;
}
