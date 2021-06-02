#include "ISystemLayInterface.h"

/******************************************************************************
����˵�����Զ���
******************************************************************************/

/********************************************************************************************
  Function		: CDLPAutoMutex����   
  DateTime		: 	
  Description	: ����
  Input			: CDLPMutex *locker����ͨ����ָ��
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
CDLPAutoMutex::CDLPAutoMutex(CDLPMutex *locker): m_locker( locker )
{

	if( m_locker ) 
	{
		m_locker->Lock();
	}

}

//
/********************************************************************************************
  Function		: ~CDLPAutoMutex   
  DateTime		: 	
  Description	: ����
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
CDLPAutoMutex::~CDLPAutoMutex()
{

	if( m_locker )
	{
		m_locker->Unlock();
	}

}

