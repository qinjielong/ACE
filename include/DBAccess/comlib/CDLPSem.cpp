/******************************************************************************
����˵�����ź���
******************************************************************************/

#include "ISystemLayInterface.h"

/********************************************************************************************
  Function		: CDLPSem   
  DateTime		: 	
  Description	: �����ź���
  Input			: const char *czKey ���ź��������ƣ�BOOL bProcess���Ƿ��ǽ��̼���ź���
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
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

	//�ж��ź����Ƿ��Ѵ��ڣ������ڣ�����ź���
	if( (m_DLPSem=OpenSemaphoreA(SEMAPHORE_ALL_ACCESS,FALSE,wcKey))==NULL)
	{
		//���û���������̴�������źŵƣ��򴴽��ź�������ִ�гɹ��������ź�������ľ�������ʾ����
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

	//�ж��ź����Ƿ��Ѵ��ڣ������ڣ�����ź���
	if ((m_DLPSem=sem_open(wcKey, 0))==SEM_FAILED)
	{
		//�����źŵƣ��ɹ�ʱ����ָ���ź�����ָ�룬����ʱΪSEM_FAILED
		if ((m_DLPSem=sem_open(wcKey,O_RDWR | O_CREAT | O_EXCL,0644, 1))==SEM_FAILED)
		{
			m_bIsValid=FALSE;
			return;
		}
	}
	m_bIsValid=TRUE;

#endif

}

//��������
/********************************************************************************************
  Function		: ~CDLPSem   
  DateTime		: 
  Description	: �ͷ��ź�����ָ��
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
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
  Description	: �����ź�������Ч��ʶ
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPSem::IsValid()
{
	return m_bIsValid;
}

//�����ź���
/********************************************************************************************
  Function		: Signal    
  DateTime		: 	
  Description	: ����һ���ź�������
  Input			:  NULL
  Output		:         	// �����������˵����
  Return		: TRUE��ʾ�ɹ���FALSE��ʾʧ��
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPSem::Signal()
{
	BOOL bRet=TRUE;
#ifdef _WIN32

	//����һ���ź�������,TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ�
	bRet=ReleaseSemaphore(m_DLPSem, 1, NULL);

#elif _LINUX

	//����ָ�����ź�����ֵ��1��Ȼ�������ڵȴ����źŵ�ֵ��Ϊ�����������̡߳����ɹ��򷵻�0�����򷵻�-1��
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
  Description	: �ȴ���ȡ�ź���
  Input			: NULL
  Output		: NULL
  Return		: �ɹ�����TRUE��ʧ�ܷ���FALSE
  Note			:				// ��ע
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
Description		: �ȴ�һ��ʱ���ȡ�ź���
Input			: UINT mSeconds���ȴ���ʱ�䣬����
Output			: NULL
Return			: �ɹ�����TRUE��ʧ�ܷ���FALSE
Note			:				// ��ע
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
