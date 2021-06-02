
#ifdef WINCE
#pragma once
#include <Windows.h>
//ģ��win32�µ�_InterlockedAnd.
//���lpAddend���߳��б��ı���lpAddend!=oldValue,������ȴ�.
long _InterlockedAnd(PLONG lpAddend,LONG flag)
{
	long lReturn;

	while (1)
	{
		long oldValue = *lpAddend;

		lReturn = InterlockedCompareExchange(lpAddend, oldValue & flag, oldValue);
		if (lReturn == oldValue)
			break;
	}

	return lReturn;
}
long _InterlockedOr(PLONG lpAddend,LONG flag)
{
	long lReturn;
	while (1)
	{
		long oldValue = *lpAddend;

		lReturn = InterlockedCompareExchange(lpAddend, oldValue | flag, oldValue);
		if (lReturn == oldValue)
			break;
	}

	return lReturn;
}
#elif _WIN32
#pragma once
#include <intrin.h>
#endif
#include "ISystemLayInterface.h"

/******************************************************************************
����˵������д��
�����д�����ص㣺 
һ�����ݽṹ�򵥣���ʹ������event������������������� 
������Ϊд���ȵĶ�д����֧�ֶ��д�̡߳� 
�����乤����ʽ�ǳ���Ч����û���߳�ִ��д����ʱ��������Ļ�ȡ���ͷŹ��̲������κ�ϵͳ���á� 
�ġ������������̲�������ѯ�ȴ��� 
******************************************************************************/

/********************************************************************************************
Function		: CDLPWRMutex   
DateTime		: 	
Description		: ��ʼ����
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
********************************************************************************************/
CDLPWRMutex::CDLPWRMutex()

#ifdef _WIN32

//��ʼ����д����ʶ����
: m_readCount(0)
, m_writeCount(0)
{
	//������д���¼�
	m_DLPReadEvent = CreateEvent( NULL, TRUE, TRUE, NULL );
	m_DLPWriteEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

#ifdef _MUTEX_DEBUG
	m_iRRefDebug = 0;
	m_iWRefDebug = 0;
#endif 
}

#elif _LINUX
{
	//��ʼ����д��
	if (pthread_rwlock_init( &m_DLPRwmutex, NULL )!=0)
	{
		return;
	}
}	
#endif


/********************************************************************************************
Function		: ~CDLPWRMutex   
DateTime		: 
Description		: ������
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
********************************************************************************************/
CDLPWRMutex::~CDLPWRMutex()
{
#ifdef _WIN32

	//�رն�д��
	CloseHandle( m_DLPReadEvent );
	CloseHandle( m_DLPWriteEvent);
#elif _LINUX

	//���ٶ�д��
	if (pthread_rwlock_destroy( &m_DLPRwmutex )!=0)
	{
		return;
	}
#endif

}

/********************************************************************************************
Function		: LockReader   
DateTime		: 
Description		: �Ӷ���
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
********************************************************************************************/
BOOL CDLPWRMutex::LockReader()
{
	BOOL bRet=TRUE;
#ifdef _WIN32
	//ѭ���Ӷ�������
	while(1)
	{
		//��������������������ֵ�����Ľ��
		LONG count = InterlockedIncrement( &m_readCount );
		//�жϼӶ����Ƿ�ɹ���С��0��CAN_NOT_READ_SIGN�����λ���Ӷ���ʧ��
		if( count < 0 )
		{
			//����������1��ȡ����������
			InterlockedDecrement( &m_readCount );

			//��ȡ����������ֵ���ж��Ƿ����̼߳�д��
			count = InterlockedCompareExchange( &m_readCount, CAN_NOT_READ_SIGN,
				CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN );
			if( count == (CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN) )
			{
				//����д���¼��ź�
				SetEvent( m_DLPWriteEvent );
			}
			//�ȴ������¼��ź�
			WaitForSingleObject( m_DLPReadEvent, INFINITE );
		}
		else
		{
#ifdef _MUTEX_DEBUG
			assert(bRet);
			long iR = InterlockedIncrement( &m_iRRefDebug );
			long  iW = m_iWRefDebug;
			if( iW )
			{
				assert(0);
			}	
#endif
			return bRet;
		}
	}

#elif _LINUX

	//�Ӷ���
	if(pthread_rwlock_rdlock(&m_DLPRwmutex)!=0)
	{
		bRet=FALSE;
	}
#endif
	return bRet;

}

/********************************************************************************************
Function		: TryLockReader   
DateTime		: 
Description		: �������ļӶ���
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
********************************************************************************************/
BOOL CDLPWRMutex::TryLockReader()
{
	BOOL bRet=TRUE;

#ifdef _WIN32
	//��������������������ֵ�����Ľ��
	LONG count = InterlockedIncrement( &m_readCount );
	//�жϼӶ����Ƿ�ɹ���С��0����д�����Ӷ���ʧ��
	if( count < 0 )
	{
		//����������1��ȡ���˴μ�������
		InterlockedDecrement( &m_readCount );
		bRet=FALSE;
	}
	else
	{
#ifdef _MUTEX_DEBUG
		if( bRet )
		{

			long iR = InterlockedIncrement( &m_iRRefDebug );
			long  iW = m_iWRefDebug;
			if( iW )
			{
				assert(0);
			}
		}
#endif
		return bRet;
	}

#elif _LINUX
	if (pthread_rwlock_tryrdlock( &m_DLPRwmutex )!=0)
	{
		bRet=FALSE;
	}
#endif

	return bRet;

}

/********************************************************************************************
Function		: UnlockReader    
DateTime		: 
Description		: �����
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
********************************************************************************************/
void CDLPWRMutex::UnlockReader()
{

#ifdef _WIN32
#ifdef _MUTEX_DEBUG
	long iR = InterlockedDecrement( &m_iRRefDebug );
	long iW = m_iWRefDebug;
	if( iW || iR<0  )
	{
		assert(0);
	}
#endif
	//����������1
	InterlockedDecrement( &m_readCount );

	//�ж��Ƿ����߳�Ҫ��д��
	LONG count = InterlockedCompareExchange( &m_readCount, CAN_NOT_READ_SIGN,
		CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN );	
	if( count == (CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN) )
	{
		//����д���¼��ź�
		SetEvent( m_DLPWriteEvent );
	}

#elif _LINUX
	if (pthread_rwlock_unlock( &m_DLPRwmutex )!=0)
	{
		return;
	}
#endif

}


/********************************************************************************************
Function		: LockWrite   
DateTime		: 
Description		: ��д��
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
********************************************************************************************/
BOOL CDLPWRMutex::LockWrite()
{

	BOOL bRet=TRUE;

#ifdef _WIN32

	//����д��������1
	LONG count = InterlockedIncrement( &m_writeCount );
	//�ж�д���Ƿ�ɹ�������1��û�������̼߳�д��
	if( count == 1 )
	{
		//�ж��Ƿ��ж��������û�ж�������д�������߳��Ѽ�������ȴ�д���ź�
		if( !DLPStopReadWaitSetWrite() )
		{
			//д�������ɹ�������
			#ifdef _MUTEX_DEBUG
						assert(bRet);   
						long iW = InterlockedIncrement ( &m_iWRefDebug );
						long iR = m_iRRefDebug;
						if( iR || iW>1)
						{
							assert(0);
						}
			#endif
			return bRet;
		}
	}
	//�������̼߳��������ȴ�д���¼��ź�
	WaitForSingleObject( m_DLPWriteEvent, INFINITE );

	//д�������ɹ�������
#ifdef _MUTEX_DEBUG
	assert(bRet);   
	long iW = InterlockedIncrement ( &m_iWRefDebug );
	long iR = m_iRRefDebug;
	if( iR || iW>1)
	{
		assert(0);
	}
#endif

#elif _LINUX
	if(pthread_rwlock_wrlock( &m_DLPRwmutex )!=0)
	{
		bRet=FALSE;
	}

#endif
	
	return bRet;

}

/********************************************************************************************
Function		: TryLockWrite   
DateTime		: 
Description		: �������ļ�д��
Input			: NULL
Output			: NULL
Return			: �ɹ�����TRUE,ʧ�ܷ���FALSE
Note			:				// ��ע
********************************************************************************************/
BOOL CDLPWRMutex::TryLockWrite()
{
	BOOL bRet=TRUE;

#ifdef _WIN32
	//����д������	
	LONG count = InterlockedIncrement( &m_writeCount );
	//д�������ɹ�
	if( count == 1 )
	{
		//�ж��Ƿ��ж���
		if( !DLPStopReadWaitSetWrite() )
		{
			//д�������ɹ�������
#ifdef _MUTEX_DEBUG
			assert(bRet);   
			long iW = InterlockedIncrement ( &m_iWRefDebug );
			long iR = m_iRRefDebug;
			if( iR || iW>1)
			{
				assert(0);
			}
#endif
			return bRet;
		}
	}
	//����ʧ�ܣ�����
	InterlockedDecrement( &m_writeCount );
	return	FALSE;

#elif _LINUX
	if(pthread_rwlock_trywrlock(&m_DLPRwmutex)!=0)
	{
		bRet=FALSE;
	}

#endif

	return bRet;

}

/********************************************************************************************
Function		: UnlockWrite    
DateTime		: 
Description		: ��д��
Input			: NULL
Output			: NULL
Return			: NULL
Note			:				// ��ע
********************************************************************************************/
void CDLPWRMutex::UnlockWrite()
{

#ifdef _WIN32
#ifdef _MUTEX_DEBUG
	long iW = InterlockedDecrement( &m_iWRefDebug );
	long iR = m_iRRefDebug;
	if( iR || iW )
	{
		assert(0);
	}
#endif
	LONG count = m_writeCount;
	//�����̼߳���д��
	if( count == 1 )
	{
		//�������������CAN_NOT_READ_SIGN��ʶ
		_InterlockedAnd( &m_readCount, ~CAN_NOT_READ_SIGN );
		//���Ͷ����¼��ź�
		SetEvent( m_DLPReadEvent );
		//����д��
		count = InterlockedDecrement( &m_writeCount );
		//�����ɹ�
		if( !count )
		{
			return;
		}
		//�������߳�Ҫ��д��
		if( DLPStopReadWaitSetWrite() )
		{
			return;
		}
	}
	else
	{
		//д��������1
		InterlockedDecrement( &m_writeCount );
	}
	//����д���¼��ź�
	SetEvent( m_DLPWriteEvent );

#elif _LINUX
	if (pthread_rwlock_unlock( &m_DLPRwmutex )!=0)
	{
		return;
	}
#endif

}

#ifdef _WIN32

//
/********************************************************************************************
  Function		: DLPStopReadWaitSetWrite   
  DateTime		: 
  Description	: ֹͣ��������д��
  Input			: NULL
  Output		: NULL
  Return		: ����TRUE���������߳�Ҫ��д��������FALSE���������̼߳�д��
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPWRMutex::DLPStopReadWaitSetWrite()
{
	//���ö����¼�
	ResetEvent( m_DLPReadEvent );

	//�ж϶������������ö���������CAN_NOT_READ_SIGN���
	LONG count = _InterlockedOr( &m_readCount, CAN_NOT_READ_SIGN );
	//����0��˵����δ��ռ�ã����ط�0����һ���߳�ռ����
	if( count )
	{
		//���ö�����CAN_NOT_WRITE_SIGN���
		_InterlockedOr( &m_readCount, CAN_NOT_WRITE_SIGN );

		//�ж϶���������д�����
		count = InterlockedCompareExchange( &m_readCount, CAN_NOT_READ_SIGN,
			CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN );
		//�������̼߳���
		if( count != (CAN_NOT_READ_SIGN | CAN_NOT_WRITE_SIGN) )
		{			
			return TRUE;
		}
	}
	//����δ����
	return FALSE;
}
#endif
