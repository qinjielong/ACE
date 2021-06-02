
#include "ISystemLayInterface.h"

/******************************************************************************
����˵�������̼����
******************************************************************************/

/********************************************************************************************
Function		: CDLPProcessMutex   
DateTime		: 	
Description		: ����������
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
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
Description		: ɾ����
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
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
Description		: ����
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
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

	if(lockf(m_DLPProcessMutex,F_LOCK,0)<0) //����ʹ��F_LOCK��������Ѿ���������������ǰһ�������ͷ���
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
Description		: �������ļ���
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
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

	if(lockf(m_DLPProcessMutex,F_TLOCK,0)<0) //����ʹ��F_TLOCK�����ļ��Ѿ�������ʱ�����ý��̲�����������ֱ�Ӵ�ӡ������Ϣ������
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
Description		: ����
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
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
