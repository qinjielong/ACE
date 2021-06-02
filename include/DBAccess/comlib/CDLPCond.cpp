#include "ISystemLayInterface.h"


/******************************************************************************
����˵������������
******************************************************************************/

/********************************************************************************************
  Function		: CDLPCond    
  DateTime		: 	
  Description	: ������������
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/

CDLPCond::CDLPCond()
{
	
#ifdef _WIN32
	//win32������������
	m_mutex = CreateMutexA (NULL, false, NULL); 
	m_DLPCond = NULL;
	m_DLPCond = CreateEvent(NULL, FALSE, FALSE, NULL);
#elif _LINUX
	//linux������������
	m_CondMutex = NULL;
	if (!m_CondMutex)
	{
		m_CondMutex=new CDLPMutex();
	}
	pthread_cond_init(&m_DLPCond, NULL);
#endif	
}


/********************************************************************************************
  Function		: ~CDLPCond    
  DateTime		: 	
  Description	: ������
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
CDLPCond::~CDLPCond()
{

#ifdef _WIN32
	CloseHandle(m_mutex);
	CloseHandle(m_DLPCond);
	m_DLPCond = NULL;
	m_mutex = NULL;
#elif _LINUX
	if (m_CondMutex)
	{
		delete	m_CondMutex;
		m_CondMutex = NULL;
	}
	pthread_cond_destroy(&m_DLPCond);
#endif
}


/********************************************************************************************
  Function		: Wait   
  DateTime		: 	
  Description	: �ȴ���������
  Input			: NULL
  Output		: NULL
  Return		: ����0�ɹ���������ʾʧ��
  Note			:				// ��ע
********************************************************************************************/
INT CDLPCond::Wait()
{
#ifdef _WIN32
	INT32 iRet = -1;
	WaitForSingleObject(m_mutex,INFINITE);	
	iRet =SignalObjectAndWait(m_mutex, m_DLPCond,INFINITE,FALSE) == WAIT_OBJECT_0 ? 0 : -1;
	ResetEvent(m_DLPCond);

	return	iRet;

#elif _LINUX
	INT32 iRet = 0;
	m_CondMutex->Lock();
	iRet = pthread_cond_wait(&m_DLPCond,&m_CondMutex->m_DLPMutex);
	m_CondMutex->Unlock();
	return iRet;
#endif
}


/********************************************************************************************
  Function		: WaitTimeout    
  DateTime		: 	
  Description	: �ȴ�������������ʱ����
  Input			: INT mseconds���ȴ���ʱ�䣬����
  Output		: NULL
  Return		: ����0�ɹ���������ʾʧ��
  Note			:				// ��ע
********************************************************************************************/
INT CDLPCond::WaitTimeout(INT mseconds)
{
#ifdef _WIN32

	INT iRet = -1;
	if (WaitForSingleObject(m_mutex,mseconds) == WAIT_OBJECT_0)
	{	
		iRet = SignalObjectAndWait(m_mutex, m_DLPCond,mseconds,FALSE) == WAIT_OBJECT_0 ? 0 : -1;
		ResetEvent(m_DLPCond);
	}
	return	iRet;

#elif _LINUX
	INT32	iRet = 0;
	struct timeval struTimeVal;
	struct timespec struTimeSpec;
	gettimeofday(&struTimeVal, NULL);
	struTimeSpec.tv_sec  = mseconds/1000;
	struTimeSpec.tv_nsec =1000L *(struTimeVal.tv_usec+(mseconds-struTimeSpec.tv_sec*1000)*1000L);
	struTimeSpec.tv_sec += struTimeVal.tv_sec;
	
	m_CondMutex->Lock();
	iRet = pthread_cond_timedwait( &m_DLPCond, &m_CondMutex->m_DLPMutex, &struTimeSpec);
	m_CondMutex->Unlock();

	return iRet;
#endif
}

/********************************************************************************************
  Function		: Signal    
  DateTime		: 	
  Description	: �����ź�
  Input			: NULL
  Output		: NULL
  Return		: ����0�ɹ���������ʾʧ��
  Note			:				// ��ע
********************************************************************************************/
INT CDLPCond::Signal()
{
#ifdef _WIN32
	DWORD dwRes;
	INT iRet=0;

	WaitForSingleObject(m_mutex,INFINITE);
	dwRes=SetEvent(m_DLPCond);
	if (dwRes==0)
	{
		iRet  = (GetLastError() == ERROR_SUCCESS ? TRUE : FALSE);
	}
	ReleaseMutex (m_mutex);  
	return iRet;
#elif _LINUX
	return pthread_cond_signal(&m_DLPCond);
#endif
}

//���͹㲥�ź�
/********************************************************************************************
  Function		: BroadcastSignal    
  DateTime		: 	
  Description	: ���͹㲥�ź�
  Input			: NULL
  Output		: NULL
  Return		: ����0�ɹ���������ʾʧ��
  Note			:				// ��ע
********************************************************************************************/
INT CDLPCond::BroadcastSignal()
{
#ifdef _WIN32
	DWORD dwRes;
	INT iRet=0;

	WaitForSingleObject(m_mutex,INFINITE);
	dwRes=SetEvent(m_DLPCond);
	if (dwRes==0)
	{
		iRet  = (GetLastError() == ERROR_SUCCESS ? TRUE : FALSE);
	}
	ReleaseMutex (m_mutex);  
	return iRet;
#elif _LINUX
	return pthread_cond_broadcast(&m_DLPCond);
#endif
}




/*
*********************************************************************
*
*@brief : CDLPCondEx ʵ��
*
*********************************************************************
*/
CDLPCondEx::CDLPCondEx(void)
{
#ifdef _WIN32
	m_iWaitConts = 0;
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if( !m_hEvent )
	{
		assert(0);
		_exit(-2);
	}
#else
	//linux������������
	::pthread_cond_init(&m_hEvent, NULL);
#endif	

}

CDLPCondEx::~CDLPCondEx(void)
{
#ifdef _WIN32
	if( m_hEvent )
	{
		::CloseHandle(m_hEvent );
		m_hEvent = NULL;
	}
#else
	::pthread_cond_destroy(&m_hEvent);
#endif	
}

INT	CDLPCondEx::Wait( CDLPMutex *pMutex )
{
#ifdef _WIN32
	DWORD wRet;
	m_iWaitConts++;
	pMutex->Unlock();
	wRet = WaitForSingleObject(m_hEvent,INFINITE);
	pMutex->Lock();
	m_iWaitConts--;
	if (wRet == WAIT_OBJECT_0)
	{
		return R_SUCCESS;
	} 			
	return R_ESYSTEM;
#else
	INT32 iRet = 0;	
	iRet = ::pthread_cond_wait(&m_hEvent,&pMutex->m_DLPMutex);
	return (iRet==0 ? R_SUCCESS : R_ESYSTEM);
#endif

}


INT	CDLPCondEx::WaitTimeout(CDLPMutex *pMutex, INT mseconds)
{
#ifdef _WIN32
	DWORD wRet;
	m_iWaitConts++;
	pMutex->Unlock();
	wRet = ::WaitForSingleObject(m_hEvent,mseconds);
	pMutex->Lock();
	m_iWaitConts--;

	if (wRet == WAIT_OBJECT_0)
	{
		return R_SUCCESS;
	} 
	else if( wRet == WAIT_TIMEOUT )
	{
		return R_ETIMEOUT;
	}
	return R_ESYSTEM;

#else
	struct timespec ts;
	struct timeval tv;
	struct timezone tz;
	int sec, usec;

	gettimeofday(&tv, &tz);
	sec = mseconds / 1000;
	mseconds = mseconds - (sec * 1000);
	assert( mseconds < 1000 );
	usec = mseconds * 1000;
	assert(tv.tv_usec < 1000000);
	ts.tv_sec = tv.tv_sec + sec;
	ts.tv_nsec = (tv.tv_usec + usec) * 1000;
	assert(ts.tv_nsec < 2000000000);
	if(ts.tv_nsec > 999999999)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}

	INT32 iRet = 0;		
	iRet = ::pthread_cond_timedwait(&m_hEvent,&pMutex->m_DLPMutex, &ts);
	if( iRet )
	{
		if(iRet==ETIMEDOUT )
		{
			return R_ETIMEOUT;
		}
		else
		{
			return R_ESYSTEM;
		}
	}
	return R_SUCCESS;
#endif

}

INT	CDLPCondEx::Signal(void)
{
#ifdef _WIN32
	if( ::SetEvent(m_hEvent) )
	{
		return R_SUCCESS;
	}
	return R_ESYSTEM;
#else
	if( ::pthread_cond_signal(&m_hEvent) )
	{
		return R_ESYSTEM;
	}
	return R_SUCCESS;
#endif

}


INT	CDLPCondEx::BroadcastSignal(void)
{
#ifdef _WIN32
	::SetEvent(m_hEvent);
	INT x = m_iWaitConts-1;
	for( ;x>0; x-- )
	{

		if( !::SetEvent(m_hEvent) )
		{
			assert(0);
		}
		MSLEEP(1);
	}
	//ResetEvent(m_hEvent);
	return R_SUCCESS;


#else
	if( ::pthread_cond_broadcast(&m_hEvent) )
	{
		return R_ESYSTEM;
	}
	return R_SUCCESS;
#endif
}