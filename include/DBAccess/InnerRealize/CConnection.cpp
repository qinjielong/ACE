#include "CConnection.h"

using	namespace	DBAccessModule;

CConnection::CConnection(void)
{
	// ����
	memset(m_szServer,0x0,coniDataLen);
	memset(m_szDatabase,0x0,coniDataLen);
	memset(m_szUser,0x0,coniDataLen);
	memset(m_szPass,0x0,coniDataLen);
	m_eConnectionUseFlag = CONNECTION_IDLESSE ;    
	// Ψһ������ID,��������ID�����ӳ����ҵ�Ψһ������
	m_iConnectID = 0;

}

CConnection::~CConnection(void)
{
	// ����
	memset(m_szServer,0x0,coniDataLen);
	memset(m_szDatabase,0x0,coniDataLen);
	memset(m_szUser,0x0,coniDataLen);
	memset(m_szPass,0x0,coniDataLen);
	m_eConnectionUseFlag = CONNECTION_IDLESSE ;    
	// Ψһ������ID,��������ID�����ӳ����ҵ�Ψһ������
	m_iConnectID = 0;

}


/**************************************************************************************************
  Function:  Initial  
  DateTime: 
  Description: ��ʼ�����ݿ�����������ݿ����ƣ��û������������Ϣ
  Input:          	 // �������˵��������ÿ����������
  Output:         	// �����������˵����
  Return:         	// ��������ֵ��˵��
  Note:				// ��ע
**************************************************************************************************/
BOOL	CConnection::Initial(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass)
{
	memset(m_szServer,0x0,coniDataLen);
	memcpy(m_szServer,szServer,strlen(szServer));
	
	memset(m_szDatabase,0x0,coniDataLen);
	memcpy(m_szDatabase,szDatabase,strlen(szDatabase));

	memset(m_szUser,0x0,coniDataLen);
	memcpy(m_szUser,szUser,strlen(szUser));


	memset(m_szPass,0x0,coniDataLen);
	memcpy(m_szPass,szPass,strlen(szPass));

	return  TRUE;

}

