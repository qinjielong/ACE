#include "CDBConnectionPool.h"

using	namespace	DBAccessModule;

CDBConnectionPool*		CDBConnectionPool::m_pInstance = NULL;
CDLPMutex				CDBConnectionPool::m_CsSingleton;
CDBConnectionPool::CDBConnectionPool(void)
{
	m_pConnectionFactory = NULL;
}

// ��������
CDBConnectionPool::~CDBConnectionPool(void)
{
	// �ͷŶ���
	if (m_pConnectionFactory!=NULL)
	{
		delete	m_pConnectionFactory;
		m_pConnectionFactory = NULL;
	}

}

//---------------------------------------------------------------------------------------------
// ������ʵ��
//---------------------------------------------------------------------------------------------
/**************************************************************************************************
  Function: Instance    
  DateTime: 
  Description:    	������������
  Input:          	NULL
  Output:         	NULL
  Return:         	CDBConnectionPool*���͵�ָ�����
  Note:				
**************************************************************************************************/
CDBConnectionPool*	CDBConnectionPool::Instance()
{
	if (m_pInstance == NULL)
	{
		CDLPAutoMutex	AutoLock(&m_CsSingleton);
		if (m_pInstance == NULL)
		{
			m_pInstance = new CDBConnectionPool();
		}
	}
	return	m_pInstance;

}

/**************************************************************************************************
  Function: Release    
  DateTime: 
  Description:    	�ͷŵ�����Դ
  Input:          	NULL
  Output:         	NULL
  Return:         	
					TRUE:�����ɹ�
  Note:				
**************************************************************************************************/
BOOL	CDBConnectionPool::Release()
{
	if(m_pInstance != NULL)
	{
		// �ر����е����Ӷ����ͷ����Ӷ���
		DeleteAllConnection();

		delete m_pInstance;
		m_pInstance = NULL;
	}

	return TRUE;	
}

//---------------------------------------------------------------------------------------------
// OuterInterface 
//---------------------------------------------------------------------------------------------

/**************************************************************************************************
  Function:  GetConnection   
  DateTime: 	
  Description:      �����ӳ���ȡ��һ�����Ӷ��󣬹��ⲿʹ��
  Input:          	
					  szServer ��   ���ݿ��������ַ
					  szDatabase��  ���ݿ�����
					  szUser  ��    ���ݿ��û���
					  szPass  ��    ���ݿ�����
					  iDbType ��     ���ݿ�����[sql ,oracle ��]

  Output:         	NULL
  Return:         	���Ӷ���ָ��IConnection*
  Note:				
**************************************************************************************************/
IConnection*	CDBConnectionPool::GetConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// ��μ�� 
	if (InputDataVerify(szServer,szDatabase,szUser,szPass,iDbType) == FALSE)
	{
		return	NULL;
	}

	// û�д������ʹ������Ӷ��󣬻��߶���ʹ����û�е��������Ŀ���ڴ���2��
	if ((GetConnectionPoolNum()==0 ) || (GetConnectionPoolNum() < coniConnectionPoolMaxMum && BOOLAllConnectionUsed()))
	{
		// �����������Ӷ��󣬴������ӳ���,ÿ�δ��� coniConnectionBatchNum ��������ʧ�ܣ�����NULL
		if (!BatchCreateConnHandle(szServer,szDatabase,szUser,szPass,iDbType,coniConnectionBatchNum))
		{
			return	NULL;
		}
		
	}

	// �Ѿ��������Ӷ�����
	if (GetConnectionPoolNum() > 0 )
	{
		// �����ӳ��л�ȡ���Ӷ���
		return	(IConnection*)GetOneConnectionFromPool();
	}

	return	NULL;

}


/**************************************************************************************************
  Function: InputDataVerify    
  DateTime: 	
  Description:    	�ж��û����������Ƿ�׼ȷ
  Input:          	
					  szServer ��   ���ݿ��������ַ
					  szDatabase��  ���ݿ�����
					  szUser  ��    ���ݿ��û���
					  szPass  ��    ���ݿ�����
					  iDbType ��    ���ݿ�����[sql ,oracle ��]
  Output:         	NULL
  Return:         	TRUE : ����Ϸ�   FALSE :�������
  Note:				
**************************************************************************************************/
BOOL			CDBConnectionPool::InputDataVerify(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// У����Ϣ
	if (strlen(szServer) > coniDataLen || strlen(szServer) <= coniZero)
	{
		return	FALSE;
	}

	if( strlen(szDatabase) > coniDataLen || strlen(szDatabase) <= coniZero)
	{
		return	FALSE;
	}

	if( strlen(szUser) > coniDataLen || strlen(szUser) <= coniZero)
	{
		return	FALSE;
	}

	if( strlen(szPass) > coniDataLen || strlen(szPass) <= coniZero)
	{
		return	FALSE;
	}

	if (iDbType == ORACLE  || iDbType == SQLSERVER || iDbType == MYSQL)
	{
		return	TRUE;
	}

	return FALSE;

}


//---------------------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------------------
/**************************************************************************************************
Function: BatchCreateConnHandle    
DateTime: 	
Description:    	   �����������Ӷ���,���þ���Ĺ������󴴽�
Input:          	
				       szServer ��   ���ݿ��������ַ
					   szDatabase��  ���ݿ�����
					   szUser  ��    ���ݿ��û���
					   szPass  ��    ���ݿ�����
					   iDbType ��     ���ݿ�����[sql ,oracle ��]
Output:         	   NULL
Return:         	  
				       TRUE:�ɹ�
	                   FALSE:ʧ��
Note:				// ��ע
**************************************************************************************************/
CConnection*    CDBConnectionPool::CreateConnectionHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// ���ݿ����Ӷ���
	CConnection*	pConnection = NULL;
	switch (iDbType)
	{
	case MYSQL:
		// MySQL ���ݿ�
		{	
			if (m_pConnectionFactory == NULL)
			{	
				// ��������
				m_pConnectionFactory	=	new CConcreteConnectionFactory();		
			}
			// ��������
			pConnection	=	m_pConnectionFactory->CreateMySQLConnection(szServer,szDatabase,szUser,szPass);
		}		
		break;	
		// ������ݿ⣬ֻ���ڴ˴������Ϳ���
	default:
		break;
	}

	// ���ش����ɹ��Ķ���
	return  pConnection;

}


/**************************************************************************************************
  Function: BatchCreateConnHandle    
  DateTime: 
  Description:    	  ������������,��ӵ����ӳ���
  Input:          	
					  szServer ��   ���ݿ��������ַ
					  szDatabase��  ���ݿ�����
					  szUser  ��    ���ݿ��û���
					  szPass  ��    ���ݿ�����
					  iDbType ��     ���ݿ�����[sql ,oracle ��]
  Output:         	  NULL
  Return:         	  
					  TRUE:�ɹ�
					  FALSE:ʧ��
  Note:				// ��ע
**************************************************************************************************/
BOOL            CDBConnectionPool::BatchCreateConnHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType,const INT iConnNum)
{
	BOOL  bRet = FALSE;
	// ����20�����Ӷ���
	for (int i=0;i< iConnNum ;i++)
	{
		CConnection*	pConnection = NULL;
		// ����������������
		pConnection = CreateConnectionHandle(szServer,szDatabase,szUser,szPass,iDbType);

		if(pConnection != NULL)
		{
			bRet = TRUE;
			// ��ӵ�����
			AddConnecttion(pConnection);
		}
		else
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;

}





//---------------------------------------------------------------------------------------------
// ���ӳض��е���ز���
//---------------------------------------------------------------------------------------------

/**************************************************************************************************
Function: AddConnecttion    
DateTime: 
Description:    	������Ӷ��󵽶�����
Input:          	pConnection:���Ӷ���
Output:         	NULL
Return:         	
					TRUE: �����ɹ�
					FALSE: ����ʧ��
Note:				// ��ע
**************************************************************************************************/
BOOL            CDBConnectionPool::AddConnecttion(CConnection*   pConnection)
{
	if (pConnection != NULL)
	{
		CDLPAutoMutex	AutoLock(&m_CsConnectionPool);//����һ���������󣬹���ʱ����������ʱ����
		m_ConnectionPool.push_back(pConnection);
		return	TRUE;
	}

	return  FALSE;

}

/**************************************************************************************************
  Function: DeleteConnection    
  DateTime: 
  Description:    	ɾ�������е����Ӷ���
  Input:          	pConnection:���Ӷ���
  Output:         	NULL
  Return:         	
					TRUE: �����ɹ�
					FALSE: ����ʧ��
  Note:				// ��ע
**************************************************************************************************/
BOOL            CDBConnectionPool::DeleteConnection(CConnection* pConnection)
{
	if (pConnection!=NULL)
	{
		CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
		for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
		{
			// ���Ӻ���Ⱦͱ�ʾ���
			if (pConnection->GetConnectID() == (*iter)->GetConnectID())
			{
				CConnection*	pCsConnnection = NULL;
				pCsConnnection = (*iter);
				if (pCsConnnection != NULL)
				{
					pCsConnnection->Close();
					delete pCsConnnection;
					pCsConnnection = NULL;
				}

				// ��������ɾ��
				m_ConnectionPool.erase(iter);
				break;
			}
		}
	}
	return  FALSE;

}

/**************************************************************************************************
  Function: DeleteAllConnection    
  DateTime: 
  Description:    	ɾ�����ӳ������е�����
  Input:          	NULL
  Output:         	NULL
  Return:         	
					TRUE:�ɹ�
					FALSE:ʧ��
  Note:				
**************************************************************************************************/
BOOL			CDBConnectionPool::DeleteAllConnection()
{
	CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
	for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
	{
		CConnection*	pConnectionObj = NULL;
		pConnectionObj = (*iter);
		if (pConnectionObj != NULL)
		{
			// �ر�
			pConnectionObj->Close();

			delete pConnectionObj;
			pConnectionObj = NULL;
		}
	}

	// ��ն���
	m_ConnectionPool.erase(m_ConnectionPool.begin(),m_ConnectionPool.end());
	return	TRUE;

}


/**************************************************************************************************
  Function: GetOneConnectionFromPool    
  DateTime: 
  Description:    	��ȡһ����Ч������
  Input:          	NULL
  Output:         	NULL
  Return:         	����һ����Ч�����Ӷ���
  Note:				
**************************************************************************************************/
CConnection*	CDBConnectionPool::GetOneConnectionFromPool()
{
	CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
	for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
	{
		CConnection*	pConnectionObj = NULL;
		pConnectionObj = (*iter);
		if (pConnectionObj != NULL)
		{
			// ���е�
			if (pConnectionObj->GetConnectionUseFlag() == CONNECTION_IDLESSE)
			{
				// ��Ϊʹ��
				pConnectionObj->SetConnectionUseFlag(CONNECTION_OCCUPY);
				return	pConnectionObj;
			}
		}
	}
	return NULL ;

}
/**************************************************************************************************
  Function: GetConnectionPoolNum
  DateTime: 
  Description:    	��ȡ���ӳ�Ԫ�ظ���    
  Input:          	NULL
  Output:         	NULL
  Return:           ���ӳ�Ԫ�ظ���
  Note:				
**************************************************************************************************/
INT				CDBConnectionPool::GetConnectionPoolNum()
{
	CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
	return (INT)m_ConnectionPool.size();

}

/**************************************************************************************************
  Function: ReleaseConnection     
  DateTime: 
  Description:    	�ͷ����ӣ������Ӷ���Ż����ӳ�
  Input:          	pConnection: ���Ӷ���
  Output:         	NULL
  Return:         	
					TRUE: �ͷųɹ�
					FALSE:�ͷ�ʧ��
  Note:				
**************************************************************************************************/
BOOL			CDBConnectionPool::ReleaseConnection(CConnection*	pConnection)
{
	CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
	for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
	{
		CConnection*	pConnectionObj = NULL;
		pConnectionObj = (*iter);
		if (pConnectionObj->GetConnectID() == pConnection->GetConnectID())
		{
			pConnectionObj->SetConnectionUseFlag(CONNECTION_IDLESSE);
			break;
		}
	}
	return TRUE;

}

/**************************************************************************************************
Function: BOOLAllConnectionUsed    
DateTime: 
Description:    	�ж����е������Ƿ���ʹ��,��ʹ��
Input:          	NULL
Output:         	NULL
Return:         	����TRUE,���򷵻�FALSE
Note:				// ��ע
**************************************************************************************************/
BOOL            CDBConnectionPool::BOOLAllConnectionUsed()
{	
	CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
	BOOL	bUseFlag  = TRUE;
	for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
	{
		CConnection*	pConnectionObj = NULL;
		pConnectionObj = (*iter);
		if (pConnectionObj != NULL)
		{
			if (pConnectionObj->GetConnectionUseFlag() == CONNECTION_IDLESSE)
			{
				bUseFlag = FALSE;
				break;
			}
		}
	}
	return bUseFlag ;

}

