#ifndef CCONCRETECONNECTIONFACTORY_DEF_H
#define CCONCRETECONNECTIONFACTORY_DEF_H


#include "IConnectionFactory.h"
/**************************************************************************************************

Description 	: ���幤����ʵ��
**************************************************************************************************/
namespace	DBAccessModule
{

	class CConcreteConnectionFactory :public IConnectionFactory
	{
	public:
		CConcreteConnectionFactory(void);
		virtual ~CConcreteConnectionFactory(void);

	public:
		// �ӿ�ʵ��
		// MySQL ���ݿ�����
		CConnection*		CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass);
	};

}

#endif // CCONCRETECONNECTIONFACTORY_DEF_H


