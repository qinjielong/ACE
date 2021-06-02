#ifndef ICONNECTIONFACTORY_DEF_H
#define ICONNECTIONFACTORY_DEF_H

// 头文件
#include "DbAccessDataDef.h"
#include "MySQLConnection.h"

/**************************************************************************************************

Description :  连接工厂接口
**************************************************************************************************/
namespace	DBAccessModule
{

	class IConnectionFactory
	{
	public:
		IConnectionFactory(void);
		virtual ~IConnectionFactory(void);

	protected:
		// 连接对象自增长ID
		static          INT         m_iAutoConnectID;  	

	public:
		// 返回自增长连接对象ID
		inline  INT             GenerateAutoConnectID() const 
		{
			return  m_iAutoConnectID++;
		}

	public:
		// MySQL 数据库连接
		virtual CConnection*		CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass) = 0;

	};

}
#endif // ICONNECTIONFACTORY_DEF_H
