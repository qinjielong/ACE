// ͷ�ļ�
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
  Description	: MySQL ���Ӷ�����幤����������
  Input			:
					szServer ��   ���ݿ��������ַ
					szDatabase��  ���ݿ�����
					szUser  ��    ���ݿ��û���
					szPass  ��    ���ݿ�����
  Output		: NULL
  Return		: CConnection*
  Note			: MySQL���ݿ����Ӷ���
**************************************************************************************************/
CConnection*		CConcreteConnectionFactory::CreateMySQLConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass)
{
	CMySQLConnection*	pMySQLConnection = NULL; //�̳�CConnection
	pMySQLConnection = new CMySQLConnection();
	if (pMySQLConnection != NULL)
	{
		// ��������ID
		pMySQLConnection->SetConnectID(GenerateAutoConnectID());
		// ��ʼ����Ϣ
		pMySQLConnection->Initial(szServer,szDatabase,szUser,szPass);
		// �����ݿ�����
		if (pMySQLConnection->Open())
		{
			// �������Ӷ���
			return (CConnection*)pMySQLConnection;
		}		
		else
		{
			// ������ݿ��ʧ�ܣ��ͷŶ���
			delete pMySQLConnection;
			pMySQLConnection = NULL;
		}
	}
	return	(CConnection*)NULL;
}