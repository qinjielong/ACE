#include "CDBConnectionPool.h"

using	namespace	DBAccessModule;

CDBConnectionPool*		CDBConnectionPool::m_pInstance = NULL;
CDLPMutex				CDBConnectionPool::m_CsSingleton;
CDBConnectionPool::CDBConnectionPool(void)
{
	m_pConnectionFactory = NULL;
}

// 析构函数
CDBConnectionPool::~CDBConnectionPool(void)
{
	// 释放对象
	if (m_pConnectionFactory!=NULL)
	{
		delete	m_pConnectionFactory;
		m_pConnectionFactory = NULL;
	}

}

//---------------------------------------------------------------------------------------------
// 单例的实现
//---------------------------------------------------------------------------------------------
/**************************************************************************************************
  Function: Instance    
  DateTime: 
  Description:    	创建单例对象
  Input:          	NULL
  Output:         	NULL
  Return:         	CDBConnectionPool*类型的指针变量
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
  Description:    	释放单例资源
  Input:          	NULL
  Output:         	NULL
  Return:         	
					TRUE:操作成功
  Note:				
**************************************************************************************************/
BOOL	CDBConnectionPool::Release()
{
	if(m_pInstance != NULL)
	{
		// 关闭所有的连接对象，释放连接对象
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
  Description:      从连接池中取出一个连接对象，供外部使用
  Input:          	
					  szServer ：   数据库服务器地址
					  szDatabase：  数据库名称
					  szUser  ：    数据库用户名
					  szPass  ：    数据库密码
					  iDbType ：     数据库类型[sql ,oracle 等]

  Output:         	NULL
  Return:         	连接对象指针IConnection*
  Note:				
**************************************************************************************************/
IConnection*	CDBConnectionPool::GetConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// 入参检查 
	if (InputDataVerify(szServer,szDatabase,szUser,szPass,iDbType) == FALSE)
	{
		return	NULL;
	}

	// 没有创建，就创建连接对象，或者都在使用且没有到达最大数目就在创建2个
	if ((GetConnectionPoolNum()==0 ) || (GetConnectionPoolNum() < coniConnectionPoolMaxMum && BOOLAllConnectionUsed()))
	{
		// 批量创建连接对象，存入连接池中,每次创建 coniConnectionBatchNum 个，创建失败，返回NULL
		if (!BatchCreateConnHandle(szServer,szDatabase,szUser,szPass,iDbType,coniConnectionBatchNum))
		{
			return	NULL;
		}
		
	}

	// 已经创建连接对象了
	if (GetConnectionPoolNum() > 0 )
	{
		// 从连接池中获取连接对象
		return	(IConnection*)GetOneConnectionFromPool();
	}

	return	NULL;

}


/**************************************************************************************************
  Function: InputDataVerify    
  DateTime: 	
  Description:    	判断用户输入条件是否准确
  Input:          	
					  szServer ：   数据库服务器地址
					  szDatabase：  数据库名称
					  szUser  ：    数据库用户名
					  szPass  ：    数据库密码
					  iDbType ：    数据库类型[sql ,oracle 等]
  Output:         	NULL
  Return:         	TRUE : 输入合法   FALSE :输出错误
  Note:				
**************************************************************************************************/
BOOL			CDBConnectionPool::InputDataVerify(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// 校验信息
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
// 创建对象
//---------------------------------------------------------------------------------------------
/**************************************************************************************************
Function: BatchCreateConnHandle    
DateTime: 	
Description:    	   创建当个连接对象,利用具体的工厂对象创建
Input:          	
				       szServer ：   数据库服务器地址
					   szDatabase：  数据库名称
					   szUser  ：    数据库用户名
					   szPass  ：    数据库密码
					   iDbType ：     数据库类型[sql ,oracle 等]
Output:         	   NULL
Return:         	  
				       TRUE:成功
	                   FALSE:失败
Note:				// 备注
**************************************************************************************************/
CConnection*    CDBConnectionPool::CreateConnectionHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)
{
	// 数据库连接对象
	CConnection*	pConnection = NULL;
	switch (iDbType)
	{
	case MYSQL:
		// MySQL 数据库
		{	
			if (m_pConnectionFactory == NULL)
			{	
				// 创建工厂
				m_pConnectionFactory	=	new CConcreteConnectionFactory();		
			}
			// 创建对象
			pConnection	=	m_pConnectionFactory->CreateMySQLConnection(szServer,szDatabase,szUser,szPass);
		}		
		break;	
		// 别的数据库，只需在此处创建就可以
	default:
		break;
	}

	// 返回创建成功的对象
	return  pConnection;

}


/**************************************************************************************************
  Function: BatchCreateConnHandle    
  DateTime: 
  Description:    	  批量创建对象,添加到连接池中
  Input:          	
					  szServer ：   数据库服务器地址
					  szDatabase：  数据库名称
					  szUser  ：    数据库用户名
					  szPass  ：    数据库密码
					  iDbType ：     数据库类型[sql ,oracle 等]
  Output:         	  NULL
  Return:         	  
					  TRUE:成功
					  FALSE:失败
  Note:				// 备注
**************************************************************************************************/
BOOL            CDBConnectionPool::BatchCreateConnHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType,const INT iConnNum)
{
	BOOL  bRet = FALSE;
	// 创建20个连接对象
	for (int i=0;i< iConnNum ;i++)
	{
		CConnection*	pConnection = NULL;
		// 功过工厂创建对象
		pConnection = CreateConnectionHandle(szServer,szDatabase,szUser,szPass,iDbType);

		if(pConnection != NULL)
		{
			bRet = TRUE;
			// 添加到队列
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
// 连接池队列的相关操作
//---------------------------------------------------------------------------------------------

/**************************************************************************************************
Function: AddConnecttion    
DateTime: 
Description:    	添加连接对象到队列中
Input:          	pConnection:连接对象
Output:         	NULL
Return:         	
					TRUE: 操作成功
					FALSE: 操作失败
Note:				// 备注
**************************************************************************************************/
BOOL            CDBConnectionPool::AddConnecttion(CConnection*   pConnection)
{
	if (pConnection != NULL)
	{
		CDLPAutoMutex	AutoLock(&m_CsConnectionPool);//创建一个加锁对象，构造时加锁，析构时解锁
		m_ConnectionPool.push_back(pConnection);
		return	TRUE;
	}

	return  FALSE;

}

/**************************************************************************************************
  Function: DeleteConnection    
  DateTime: 
  Description:    	删除队列中的连接对象
  Input:          	pConnection:连接对象
  Output:         	NULL
  Return:         	
					TRUE: 操作成功
					FALSE: 操作失败
  Note:				// 备注
**************************************************************************************************/
BOOL            CDBConnectionPool::DeleteConnection(CConnection* pConnection)
{
	if (pConnection!=NULL)
	{
		CDLPAutoMutex	AutoLock(&m_CsConnectionPool);
		for (Connection_Vector_Iterator	iter = m_ConnectionPool.begin();iter != m_ConnectionPool.end();iter++)
		{
			// 连接号相等就表示相等
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

				// 从容器中删除
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
  Description:    	删除连接池中所有的连接
  Input:          	NULL
  Output:         	NULL
  Return:         	
					TRUE:成功
					FALSE:失败
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
			// 关闭
			pConnectionObj->Close();

			delete pConnectionObj;
			pConnectionObj = NULL;
		}
	}

	// 清空队列
	m_ConnectionPool.erase(m_ConnectionPool.begin(),m_ConnectionPool.end());
	return	TRUE;

}


/**************************************************************************************************
  Function: GetOneConnectionFromPool    
  DateTime: 
  Description:    	获取一个有效的连接
  Input:          	NULL
  Output:         	NULL
  Return:         	返回一个有效的连接对象
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
			// 空闲的
			if (pConnectionObj->GetConnectionUseFlag() == CONNECTION_IDLESSE)
			{
				// 设为使用
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
  Description:    	获取连接池元素个数    
  Input:          	NULL
  Output:         	NULL
  Return:           连接池元素个数
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
  Description:    	释放连接，将连接对象放回连接池
  Input:          	pConnection: 连接对象
  Output:         	NULL
  Return:         	
					TRUE: 释放成功
					FALSE:释放失败
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
Description:    	判断所有的连接是否在使用,在使用
Input:          	NULL
Output:         	NULL
Return:         	返回TRUE,否则返回FALSE
Note:				// 备注
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

