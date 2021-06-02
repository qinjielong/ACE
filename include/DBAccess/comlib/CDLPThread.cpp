#include "ISystemLayInterface.h"
/******************************************************************************
����˵�����̲߳���
******************************************************************************/

/********************************************************************************************
  Function		: CDLPThread    
  DateTime		: 	
  Description	: CDLPThread���б����ĳ�ʼ��
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			: NULL
********************************************************************************************/
CDLPThread::CDLPThread()
{
	//��ʼ��������ʶ
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

//��������
/********************************************************************************************
  Function		: ~CDLPThread   
  DateTime		: 	
  Description	: �ͷ�CDLPThread���е���Դ
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
  Description	: widows:�ͷ��߳̾��
				  linux: �������
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

//CDLPThread�̹߳���
/********************************************************************************************
  Function		: CDLPThreadProcFunc    
  DateTime		: 	
  Description	: ִ���߳�
  Input			: HANDLE param���̺߳����Ĳ���
  Output		: NULL
  Return		: windows:����0��linux:�����̺߳����Ĳ���ָ��
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
	//�����߳�����״̬

	if(pDLPThread->m_fnUser)
	{
		//ִ���û��̺߳���
		pDLPThread->m_fnUser(pDLPThread,pDLPThread->m_pFnUserParam);
	}
	//�����źţ�����join�ȴ�
	//pDLPThread->m_DLPCond->Signal();

	pDLPThread->m_DLPMutexthreadfinish->Lock();
	pDLPThread->m_bthreadfinish = TRUE;
	pDLPThread->m_bRunninDLPtate = FALSE;	
	pDLPThread->m_DLPMutexthreadfinish->Unlock();

	pDLPThread->m_bExit = TRUE;
	//�ͷ���Դ
	pDLPThread->UnInitData();
	if (pDLPThread->m_bMutexLock)
	{
		pDLPThread->m_DLPMutexUnit->Unlock();
		pDLPThread->m_bMutexLock = FALSE;
	}
	//�����źţ�����join�ȴ�
	pDLPThread->m_DLPCond->Signal();

#ifdef _WIN32	
	return dwRet;
#elif _LINUX
	return pDLPThread;
#endif 
}


//CDLPThread�߳̿�ʼ
/********************************************************************************************
  Function		: Start    
  DateTime		: 	
  Description	: �����߳�
  Input			: DLPThreadCallbackFunction fnOnEvent���û��̺߳�����void *pUserParam���û��̲߳���
  Output		: NULL
  Return		: �ɹ�����TRUE��ʧ�ܷ���FALSE
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
			//�ͷ���Դ
			UnInitData();
		}
		else
		{
			m_bRunninDLPtate=TRUE;
		}
		m_DLPMutexthreadfinish->Unlock();
	}
#elif _WIN32
	//�����߳�
	//m_DLPThread = CreateThread(NULL,0,CDLPThreadProcFunc,HANDLE(this),0,NULL);

	//�����߳�
	m_DLPThread = (HANDLE)_beginthreadex(NULL,0,CDLPThreadProcFunc,HANDLE(this),0,NULL);
	
	if (m_DLPThread)
	{
		m_DLPMutexthreadfinish->Lock();
		if (m_bthreadfinish)
		{
			m_bRunninDLPtate=FALSE;
			//�ͷ���Դ
			UnInitData();
		}
		else
		{
			m_bRunninDLPtate=TRUE;
		}
		m_DLPMutexthreadfinish->Unlock();
	}
#elif _LINUX
	//�����߳�
	if(pthread_create(&m_DLPThread,0,CDLPThreadProcFunc,HANDLE(this))==0)
	{
		m_DLPMutexthreadfinish->Lock();
		if (m_bthreadfinish)
		{
			m_bRunninDLPtate=FALSE;
			//�ͷ���Դ
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


//ֹͣ�߳�
/********************************************************************************************
  Function		: Stop    
  DateTime		: 	
  Description	: ����ֹͣ�̱߳�ʶ
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			: NULL
********************************************************************************************/
BOOL CDLPThread::Stop()
{
	//�ж��߳��Ƿ�������
	if(m_bRunninDLPtate)
	{
		//����ֹͣ�̱߳�ʶ
		m_bExiting = TRUE;
	}
	return m_bExiting;

}

/********************************************************************************************
  Function		: IsRunning    
  DateTime		: 	
  Description	: �����߳�����״̬
  Input			: NULL
  Output		: NULL
  Return		: �߳��������з���TRUE,���򷵻�FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::IsRunning()
{
	return m_bRunninDLPtate;
}

//�̹߳���
/********************************************************************************************
  Function		: Suspend   
  DateTime		: 	
  Description	: �����߳�����״̬
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::Suspend()
{
	//�ж��߳�����״̬
	if(m_bRunninDLPtate)
	{
		//�����߳�����״̬
		m_DLPMutexPause->Lock();
		m_bPause = TRUE;
		m_DLPMutexPause->Unlock();
	}

	return m_bPause;

}

//�߳�˯��
/********************************************************************************************
  Function		: Resume    
  DateTime		: 	
  Description	: �����źŻ����̣߳������߳�����״̬
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::Resume()
{
	if(m_bRunninDLPtate)
	{
		//�ж��߳�����״̬
		if(m_bPause)
		{
			//�����߳�
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();
		}
	}
	//�����߳�����״̬
	return m_bPause;
}

//�ȴ��߳̽���
/********************************************************************************************
  Function		: Join   
  DateTime		: 	
  Description	: �ȴ��߳������˳�
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::Join()
{
	//�ж��߳��Ƿ������У��Ƿ���join��
	if(m_bRunninDLPtate&&!m_bJoin)
	{
		//�ж��̵߳�����״̬
		if(m_bPause)
		{
			//�����߳�
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//�����߳��˳���ʶ
		m_bExiting = TRUE;

#ifdef _WIN32
		//�ȴ��߳��˳�
		if(0 == m_DLPCond->Wait())
		{
			//�����߳��˳���־λ��join��־λ
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}
#elif _LINUX
		//�ȴ��߳��˳�
		if (pthread_join(m_DLPThread,NULL)==0)
		{
			m_bJoin=TRUE;
		}		
#endif
	}
	//�����߳�Join��־λ
	return m_bJoin;

}


/********************************************************************************************
  Function		: Join  
  DateTime		: 
  Description	: �ȴ��߳�һ��ʱ�䣬�ж��Ƿ��˳� 
  Input			: INT mseconds:�ȴ�ʱ�䣬����
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::Join(INT mseconds)
{
	//�ж��߳��Ƿ������У��Ƿ���join��
	if(m_bRunninDLPtate&&!m_bJoin)
	{
		//�ж��̵߳�����״̬
		if(m_bPause)
		{
			//�����߳�
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//�����߳��˳���ʶ
		m_bExiting = TRUE;

#ifdef _WIN32
		//�ȴ��߳��˳�
		if(0 == m_DLPCond->WaitTimeout(mseconds))
		{
			//�����߳��˳���־λ��join��־λ
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}

#elif _LINUX
		//�ȴ��߳��˳�
		if (pthread_join(m_DLPThread,NULL)==0)
		{
			m_bExit=TRUE;
			m_bJoin=TRUE;
		}		
#endif
	}
	//�����߳�Join��־λ
	return m_bJoin;

}

/********************************************************************************************
  Function		: Cancel   
  DateTime		: 	
  Description	: ǿ���˳��߳�
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPThread::Cancel()
{
	//�ж��߳�����״̬
	if(m_bRunninDLPtate)
	{
		//�����߳��˳�״̬
		m_bExiting = TRUE;
		
		//�ж��߳�����״̬
		if(m_bPause)
		{
			//�����߳�
			m_DLPMutexPause->Lock();
			m_DLPCondPause->Signal();
			m_bPause = FALSE;
			m_DLPMutexPause->Unlock();;
		}
		//�ж��߳��˳�״̬
		if(!m_bExit)
		{
			//ǿ����ֹ�߳�
			Kill();
		}
	}

}

/********************************************************************************************
  Function		: TestExit   
  DateTime		: 	
  Description	: ����̵߳��˳�״̬��ִ���̵߳�join���Ѻ�����
  Input			: NULL
  Output		: NULL
  Return		: ����TRUE���߳����˳�������FALSE,�߳�δ�˳�
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPThread::TestExit()
{

	//�ж��߳��˳���ʶ
	if(m_bExiting==TRUE)
	{
		m_bExit = TRUE;
	}
	//�ж��̵߳�����״̬
	else if(m_bPause==TRUE)
	{
		//�߳�����
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
	
	//�ͷ���Դ��Ϊ�߳�ִ����Ϻ��ͷţ����̴߳������Ľ�β�ͷţ�hf��2010.11.01�޸�
	//�ж��̵߳��˳�״̬
	//if (m_bExit==TRUE)
	//{
	//	//�ͷ��߳̾��
	//	UnInitData();
	//	return TRUE;
	//}

	return m_bExit;
}


/********************************************************************************************
  Function		: Kill    
  DateTime		: 	
  Description	: ǿ����ֹ�߳�
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPThread::Kill()
{
	//�ж��߳�����״̬���˳�״̬
	if(m_bRunninDLPtate || (! m_bExit))
	{
#ifdef _WIN32

		//��ֹ�߳�
		DWORD dwExitCode;
		GetExitCodeThread(m_DLPThread,&dwExitCode);
		TerminateThread(m_DLPThread,dwExitCode);
#elif _LINUX
		//ǿ���˳��߳�
		pthread_cancel(m_DLPThread);
#endif
		//�ͷ��߳̾��
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
  Description	:  �����߳�
  Input			:  NULL
  Output		:  NULL
  Return		:  NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPThread::Detach()
{
#ifdef _LINUX
	//�ж��̵߳ķ����ʶ��Join��ʶ
	if (!m_bDetached&&!m_bJoin)
	{
		//�����߳�
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
Description		:  ��ȡ�߳̾��
Input			:  NULL
Output			:  DLPThread:�߳̾����ʧ�ܷ���NULL
Return			:  NULL
Note			:				// ��ע
********************************************************************************************/
DLPThread CDLPThread::GetThreadHandle()
{

	if (m_DLPThread)
	{
		return	m_DLPThread;
	}
	return	NULL;
}
