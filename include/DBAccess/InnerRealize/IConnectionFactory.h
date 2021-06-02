#ifndef ICONNECTIONFACTORY_DEF_H
#define ICONNECTIONFACTORY_DEF_H

// ͷ�ļ�
#include "DbAccessDataDef.h"
#include "MySQLConnection.h"

/**************************************************************************************************

Description :  ���ӹ����ӿ�
**************************************************************************************************/
namespace	DBAccessModule
{

	class IConnectionFactory
	{
	public:
		IConnectionFactory(void);
		virtual ~IConnectionFactory(void);

	protected:
		// ���Ӷ���������ID
		static          INT         m_iAutoConnectID;  	

	public:
		// �������������Ӷ���ID
		inline  INT             GenerateAutoConnectID() const 
		{
			return  m_iAutoConnectID++;
		}

	public:
		// MySQL ���ݿ�����
		virtual CConnection*		CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass) = 0;

	};

}
#endif // ICONNECTIONFACTORY_DEF_H
