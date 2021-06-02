
#ifndef CDBCONNECTIONPOOL_DEF_H
#define CDBCONNECTIONPOOL_DEF_H

// ͷ�ļ�
#include "IDBAccessModule.h"			//  �ⲿ�ӿ�ͷ�ļ���
#include "CConnection.h"								//	���������
#include "CConcreteConnectionFactory.h"					//  ���幤�������
#include "DbAccessDataDef.h"							//  ������������
#include "ISystemLayInterface.h"

/**************************************************************************************************

Description : ���ӳض�����
**************************************************************************************************/
namespace	DBAccessModule
{

	class CDBConnectionPool : public IConnectionPool //�̳г�����
	{
	protected:
		// ���ṩ�ⲿ��������ķ������õ����ṩ�ķ���
		CDBConnectionPool(void);
		virtual ~CDBConnectionPool(void);

	private:
		// ����ģ��
		static	CDBConnectionPool*		m_pInstance;
		// ������
		static	CDLPMutex				m_CsSingleton;
	public:
		// ��������
		static	CDBConnectionPool*		Instance();
		// �ͷ���Դ
		BOOL	Release();

	public:
		// ��ȡ���Ӷ���ӿ� [5/22/2010 liujs] 
		IConnection*	GetConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType);


	private:
		// �ж��û����������Ƿ�׼ȷ
		BOOL			InputDataVerify(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType);

	public:
		// �����������Ӷ���
		CConnection*    CreateConnectionHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType);
		// ������������,��ӵ����ӳ���
		BOOL            BatchCreateConnHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType,const INT iConnNum);
		// �ж����е������Ƿ���ʹ��,��ʹ�ã�����TRUE,���򷵻�FALSE
		BOOL            BOOLAllConnectionUsed();

	public:
		// ��ӵ�������
		BOOL            AddConnecttion(CConnection*   pConnection);
		// ɾ�������е�Ԫ��
		BOOL            DeleteConnection(CConnection* pConnection);
		// ɾ�����е�Ԫ��
		BOOL			DeleteAllConnection();
		// ��ȡһ����Ч������
		CConnection*	GetOneConnectionFromPool();
		// ��ȡ���ӳ�Ԫ�ظ���
		INT				GetConnectionPoolNum();
		// �ͷ����ӣ������Ӷ���Ż����ӳ�
		BOOL			ReleaseConnection(CConnection*	pConnection);


	protected:
		// ���ӳ�,��ʱ����vector�У������칹��ķ�ʽ��֧�ֶ����͵Ĵ���
		// Ψһ������ID,��������ID�����ӳ����ҵ�Ψһ������
		Connection_Vector			m_ConnectionPool;  
		// ������������
		CDLPMutex					m_CsConnectionPool; //����һ��������(��ͨ��)
		// ���ӳ��󹤳�����
		IConnectionFactory*			m_pConnectionFactory;

	};


}
#endif // CDBCONNECTIONPOOL_DEF_H

