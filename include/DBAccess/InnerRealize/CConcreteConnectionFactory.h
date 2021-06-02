#ifndef CCONCRETECONNECTIONFACTORY_DEF_H
#define CCONCRETECONNECTIONFACTORY_DEF_H


#include "IConnectionFactory.h"
/**************************************************************************************************

Description 	: 具体工厂类实现
**************************************************************************************************/
namespace	DBAccessModule
{

	class CConcreteConnectionFactory :public IConnectionFactory
	{
	public:
		CConcreteConnectionFactory(void);
		virtual ~CConcreteConnectionFactory(void);

	public:
		// 接口实现
		// MySQL 数据库连接
		CConnection*		CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass);
	};

}

#endif // CCONCRETECONNECTIONFACTORY_DEF_H


