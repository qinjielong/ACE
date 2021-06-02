#include "ISystemLayInterface.h"

/******************************************************************************
功能说明：自动锁
******************************************************************************/

/********************************************************************************************
  Function		: CDLPAutoMutex函数   
  DateTime		: 	
  Description	: 加锁
  Input			: CDLPMutex *locker：普通锁的指针
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
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
  Description	: 解锁
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// 备注
********************************************************************************************/
CDLPAutoMutex::~CDLPAutoMutex()
{

	if( m_locker )
	{
		m_locker->Unlock();
	}

}

