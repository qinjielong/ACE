
#include "ISystemLayInterface.h"

/******************************************************************************
����˵������ͨ��
******************************************************************************/

/********************************************************************************************
  Function		: CDLPMutex   
  DateTime		: 	
  Description	: ��ʼ����
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
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
  Description	: ɾ����
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
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
  Description	: ����
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
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
  Description	: �������ļ���
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE,ʧ�ܷ���FALSE
  Note			:				// ��ע
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
  Description	: ����
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPMutex::Unlock()
{
#ifdef _WIN32
		LeaveCriticalSection(&m_DLPMutex);
#elif _LINUX
		pthread_mutex_unlock(&m_DLPMutex);	
#endif

}


