// 头文件
#include "CConcreteConnectionFactory.h"
using	namespace	DBAccessModule;
CConcreteConnectionFactory::CConcreteConnectionFactory(void)
{
}

CConcreteConnectionFactory::~CConcreteConnectionFactory(void)
{
}

/**************************************************************************************************
  Function		: CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass)
  DateTime		: 
  Author 		:  
  Description	: MySQL 连接对象具体工厂创建方法
  Input			:
					szServer ：   数据库服务器地址
					szDatabase：  数据库名称
					szUser  ：    数据库用户名
					szPass  ：    数据库密码
  Output		: NULL
  Return		: CConnection*
  Note			: MySQL数据库连接对象
**************************************************************************************************/
CConnection*		CConcreteConnectionFactory::CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass)
{
	CMySQLConnection*	pMySQLConnection = NULL; //继承CConnection
	pMySQLConnection = new CMySQLConnection();
	if (pMySQLConnection != NULL)
	{
		// 设置连接ID
		pMySQLConnection->SetConnectID(GenerateAutoConnectID());
		// 初始化信息
		pMySQLConnection->Initial(szServer,szDatabase,szUser,szPass);
		// 打开数据库连接
		if (pMySQLConnection->Open())
		{
			// 返回连接对象
			return (CConnection*)pMySQLConnection;
		}		
		else
		{
			// 如果数据库打开失败，释放对象
			delete pMySQLConnection;
			pMySQLConnection = NULL;
		}
	}
	return	(CConnection*)NULL;
}