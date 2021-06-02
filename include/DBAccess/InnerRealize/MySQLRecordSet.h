#pragma once

#include "CRecordSet.h"
#include "MySQLConnection.h"
#include "DbAccessDataDef.h"
#include "CDBFieldInfo.h"

/**************************************************************************************************

Description 	: MySQL���ݼ�����
**************************************************************************************************/
namespace	DBAccessModule
{
	class CMySQLRecordSet : public CRecordSet
	{
	public:
		CMySQLRecordSet(void);
		~CMySQLRecordSet(void);

	public:
		// ---------------------------------------------------------------------------------
		// OuterInterface����
		// ---------------------------------------------------------------------------------
		// �ƶ�����
		// ��¼��β��
		BOOL	Eof();
		// �������һ����¼
		BOOL	MoveLast();
		// ��һ����¼
		BOOL	MoveNext();
		// ��Ӽ�¼
		BOOL    AddNew();
		// ���¼�¼
		BOOL    Update();
		// ��Ӽ�¼
		BOOL	AddNew(const	char*	szTableName);


	public:
		// ---------------------------------------------------------------------------------
		// OuterInterface����
		// ---------------------------------------------------------------------------------
		// ֵ�Ļ�ȡ������
		// ���������ƻ�ȡ��Ӧ�ֶε�ֵ������string �����ַ���(INT64,long long��float,double�ȶ�����string����)
		BOOL	GetCollect(const char* szFieldName,string&	strValue);
		// �����������ö�Ӧ�е�ֵ
		BOOL	PutCollect(const char* szFieldName,const char*     szValue);

		// ���������ƻ�ȡ��Ӧ�ֶε�ֵ������INT �����ַ���
		BOOL	GetCollect(const char* szFieldName,INT&	iValue);
		// �����������ö�Ӧ�е�ֵ
		BOOL	PutCollect(const char* szFieldName,const INT	iValue);

		// ���������ƻ�ȡ��Ӧ�ֶε�ֵ������viod* �����ַ���������
		BOOL	GetCollect(const char* szFieldName,void*	   pValue,const INT iBuffSize,INT&	iDataLen);
		// �����������ö�Ӧ�е�ֵ
		BOOL	PutCollect(const char* szFieldName,const  void*	   pValue,const INT	iDataLen);
		// �������������ö�Ӧʱ���ֶε�ֵ
		BOOL	PutDtCollect(const char* szFieldName,const INT iYear,const INT iMonth,const INT iDay,const INT iHour,const INT iMinute,const INT iSecond);
		// �ͷ��������
		BOOL	ReleaseRecordSet();

		INT GetColumnNumber(void);


		BOOL GetCloumnName(INT iColIndex, std::string &oStrName );

	public:    
		// ---------------------------------------------------------------------------------
		// �ڲ��ӿ�
		// ---------------------------------------------------------------------------------
		// ��ѯ�ӿ�
		IRecordSet*		QuerySql(const char*  szSql);

	public:
		//----------------------------------------------------------------------------------
		// �ڲ�����
		//----------------------------------------------------------------------------------
		// �������Ӷ���
		BOOL			SetConnection(CMySQLConnection* pConnection);


	protected:
		// ---------------------------------------------------------------------------------
		// �ڲ�����
		// ---------------------------------------------------------------------------------
		// ��ȡ���ݼ��ж��Ƿ���Խ��б༭
		BOOL	GetEditMode();
		// ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���.
		BOOL	CancelUpdate();
		// �ж��Ƿ��
		BOOL	IsOpen();
		// �ر����ݼ�
		BOOL	Close();
		// �����ݼ�
		BOOL	Open(const char*  szSql);
		// #ifdef _WIN32
#if 0
		// �ֶμ�
		FieldsPtr GetFields();
		// ȡ�ֶμ�
		FieldPtr  GetField(const char* szFieldName);
#else
		//linux

		// --------------------------------------------------------------------------------------------
		// ��ʼ��STMT��������Ϣ
		BOOL		Init();

		// --------------------------------------------------------------------------------------------
		// ��ȡֵ
		// ͨ���ֶ����ƻ�ȡ��Ӧ���е�����
		INT			GetColumnIndexByName(const char* szFieldName);
		// ��ȡ�ֶ������б�
		BOOL		GetColumnList();
		// ����ֶ������б�
		BOOL		ClearColumnList();

		//---------------------------------------------------------------------------------------------
		// ����ֵ
		// ���ݲ�ѯ���ݵ�sql�������ȡ���еı�����
		BOOL		GetTableNameFromSqlText(const char* szSql,string& strTableName);
		// ��������ֶ��б�m_FieldInfoList �� AddNew �� Update��ʱ���õģ�
		BOOL		ClearFieldInfoList();
		// ���ֶ��б�������ֶ���Ϣ
		BOOL		AddFieldItem2FieldInfoList(CDBFieldInfo*	pcsDbFieldInfoItem);

	public:
		// ����ִ�е�Sql��䣨INSERT �� UPDATE ����䣩
		BOOL		BuildInsertUpdateSqlTxt();
		// ��Sql���������Ӷ�Ӧ��ֵ
		BOOL		AppendValueToSqlTxt();
		// ִ��SQL ������߸��²���(INSERT �� UPDATE)
		BOOL		ExecuteInsertUpdate();
#endif


	protected:	
		// Oracle���Ӷ���
		CMySQLConnection*	    m_pConnection;
		// #ifdef _WIN32
#if 0
		// ADO���ݼ�
		_RecordsetPtr			m_pRecordset;
#else
		//linux

		// Sql stmt ����������ڲ�ѯ���ݵ�ʱ���ã�
		SQLHSTMT				m_hSqlStmt;
		// ���ݼ��Ƿ��
		BOOL					m_bIsOpen;
		// �ж��Ƿ��Ǽ�¼��β��
		BOOL					m_bEof;
		// ������Ӧ������ID,��Insert �� Update����ʱ���õ�
		INT						m_iCollumIndexID;


	public:
		// �������������
		inline	void	InitCollumnIndexID()
		{
			m_iCollumIndexID = coniZero;
		}
		// ���������Ĳ���
		inline		INT	GenerateCollumnIndexID()
		{
			return	++m_iCollumIndexID;
		}

	protected:
		// �ֶ������б�(��ȡֵ��ʱ���õ���)
		ColumnItemVector		m_ColumnItemList;

	protected:
		// ����͸������ݵ�ʱ���õ�

		// ����͸��µ�ʱ���õ�
		SQLHSTMT				m_hUpdateSqlStmt;
		// �༭ģʽ				
		INT						m_iEditMode;
		// �����ı����ƣ�����͸��µ�ʱ���õ�����ѯ��ʱ���SQL ����н����Զ��Ľ���������
		string					m_strTableName;
		// ������SQL�ַ���
		string					m_strSqlText;
		// �����ѯ��ȥ��һ�����ݵ�ID
		INT						m_iCurrenID;
		// �������ֶ���Ϣ��ϣ�AddNew �� Update��ʱ���õģ�
		DBFieldInfoVector		m_FieldInfoList;
		// �α�����[coniMinCursorIndex,coniMaxCursorIndex]
		static		INT			m_iCursorIndex;

		// �α���������
		INT32		GetCursorIndex()
		{
			if (m_iCursorIndex >= coniMaxCursorIndex)
			{
				m_iCursorIndex = coniMinCursorIndex;
			}
			return	m_iCursorIndex++;
		}

#endif


	};
}