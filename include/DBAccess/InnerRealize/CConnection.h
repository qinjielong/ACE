#ifndef CCONNECTION_DEF_H
#define CCONNECTION_DEF_H

// OuterInterfaceͷ�ļ�
#include "IDBAccessModule.h"
#include "DbAccessDataDef.h"
/**************************************************************************************************

Description : 
���Ӷ�����
**************************************************************************************************/
namespace	DBAccessModule
{

	class CConnection : public IConnection
	{
	public:
		CConnection(void);
		virtual	~CConnection(void);

	protected:
		// ��������ַ
		char	m_szServer[coniDataLen];
		// ���ݿ�����,ORACLE���ݿ⣬��д��m_strServerһ��������ODBC�����ʱ������Դ�ʹ洢���ñ���
		char	m_szDatabase[coniDataLen];	
		// �û���
		char	m_szUser[coniDataLen];	
		// ����
		char	m_szPass[coniDataLen];	

	protected:
		// ʹ�ñ�ʶ
		EnumConnectionUseFlag	m_eConnectionUseFlag;    
		// Ψһ������ID,��������ID�����ӳ����ҵ�Ψһ������
		INT     m_iConnectID;

	public:
		// ��ȡ�Ƿ�ռ�ñ�ʶ
		inline	EnumConnectionUseFlag	GetConnectionUseFlag() const 
		{
			return	m_eConnectionUseFlag;
		}
		// �����Ƿ�ռ�ñ�ʶ
		inline	void		SetConnectionUseFlag(EnumConnectionUseFlag eConnectionUseFlag)
		{
			m_eConnectionUseFlag = eConnectionUseFlag;
		}	
		// ��ȡΨһ����ID
		inline  INT     GetConnectID() const 
		{
			return  m_iConnectID;
		}
		// ����Ψһ����ID
		inline  void    SetConnectID(const INT iConnnectID)
		{
			m_iConnectID = iConnnectID;
		}
		// ��ʼ�����ݿ���Ϣ
		BOOL	Initial(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass);

	public:
		//------------------------------------------------------------------------------------------------
		// �ڲ��ӿڵĶ���
		// �����ݿ�
		virtual	BOOL	Open()	=	0;
		// �ر����ݿ�����
		virtual	BOOL	Close()	=	0;
		// �ж��Ƿ��
		virtual	BOOL	IsOpen() = 0;

	public:
		// ------------------------------------------------------------------------------------------------
		// ������ ���ⲿ�ӿڣ��ڲ���Ч����ӿ�
		// -----------------------------------------------------------------------------------------------

	};
	// ����洢�Ķ���
	typedef	vector<CConnection*>				Connection_Vector;
	// ���еĵ�����
	typedef vector<CConnection*>::iterator		Connection_Vector_Iterator;


}

#endif // CCONNECTION_DEF_H


