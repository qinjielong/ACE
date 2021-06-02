#include "MySQLRecordSet.h"
#include "MySQLConnection.h"

using namespace DBAccessModule;

// �α�����
INT		CMySQLRecordSet::m_iCursorIndex = 0;

CMySQLRecordSet::CMySQLRecordSet(void)
{
	// linux
	m_hSqlStmt = SQL_NULL_HSTMT;
	m_bIsOpen = FALSE;
	// �༭ģʽ
	m_iEditMode = EM_UnKnown;
	// ������
	m_strTableName = constrDefaultString ;

	// �����õ�
	m_hUpdateSqlStmt = SQL_NULL_HSTMT;
	// �Ƿ��Ǽ�¼��β��
	m_bEof = TRUE;

	m_pConnection = NULL;
}

CMySQLRecordSet::~CMySQLRecordSet(void)
{
	if (IsOpen())
	{
		Close();
	}
	// linux
	if (m_bIsOpen)
	{
		m_bIsOpen = FALSE;
		SQLFreeHandle(SQL_HANDLE_STMT,m_hSqlStmt);
		m_hSqlStmt = SQL_NULL_HSTMT;
	}
	// �����õ�
	if (m_hUpdateSqlStmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,m_hUpdateSqlStmt);
		m_hUpdateSqlStmt = SQL_NULL_HSTMT;
	}
	m_iEditMode = EM_UnKnown;
	m_bEof = TRUE;
}

// ��ѯ�α��Ƿ����ڼ�¼��β
BOOL	CMySQLRecordSet::Eof()
{
	// linux 
	m_iEditMode = EM_UnKnown;
	return m_bEof;

}

// �������һ����¼
BOOL	CMySQLRecordSet::MoveLast()
{
	// linux
	// ��¼�α��ƶ������һ��λ��
	SQLRETURN	retCode = 0;
	if(m_hSqlStmt != SQL_NULL_HSTMT)
	{
		// �Ƶ����һ����¼
		retCode = SQLFetchScroll(m_hSqlStmt,SQL_FETCH_LAST,0);
		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			// ����������к����־
			InitCollumnIndexID();

			m_bEof = FALSE;
			return	TRUE;
		}
		else
		{
			m_bEof = TRUE;
			return FALSE;
		}
	}
	else
	{
		return	FALSE;
	}
}

// ���ݼ��ƶ�����һ����¼
BOOL	CMySQLRecordSet::MoveNext()
{
	// linux
	SQLRETURN	retCode = 0;
	if (m_hSqlStmt != SQL_NULL_HSTMT)
	{
		// �ƶ�����һ����¼
		retCode = SQLFetchScroll(m_hSqlStmt, SQL_FETCH_NEXT, 0);
		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			// ����������к����־
			InitCollumnIndexID();

			m_bEof = FALSE;
			return	TRUE;
		}
		else
		{
			m_bEof = TRUE;
			return	FALSE;
		}
	}
	else
	{
		// �Ѿ������һ����¼�ˣ�������������ƶ�����
		return	FALSE;
	}

}

// ���һ���ռ�¼
BOOL    CMySQLRecordSet::AddNew()
{
	// linux
	m_iEditMode = EM_AddNew;

	// ����������к����־
	InitCollumnIndexID();
	ClearFieldInfoList();

	return	TRUE;

}

// ��һ�������ϣ�ָ���ı������������
BOOL	CMySQLRecordSet::AddNew(const	char*	szTableName)
{
	// ��ʼ������
	Init();

	// ���ò����־
	m_iEditMode = EM_AddNew;

	// ����������к����־
	InitCollumnIndexID();
	ClearFieldInfoList();

	// ���ñ���
	m_strTableName	= string(szTableName);

	return	TRUE;
}

// �������ݼ�
BOOL    CMySQLRecordSet::Update()
{
	// linux
	// �����ַ���
	if (BuildInsertUpdateSqlTxt())
	{
		// ���ֵ
		if (AppendValueToSqlTxt())
		{
			// ִ�в���
			if (ExecuteInsertUpdate())
			{
				// ����������к����־
				InitCollumnIndexID();
				// ����б�
				ClearFieldInfoList();

				return TRUE;
			}
		}
	}
	// ����б�
	ClearFieldInfoList();
	// ����������к����־
	InitCollumnIndexID();
	// �������ʧ��,��ȡ������
	CancelUpdate();
	return	FALSE;

}

// ���������ֶ����ƻ�ȡ�ַ���ֵ
BOOL	CMySQLRecordSet::GetCollect(const char* szFieldName,string&	szValue)
{
	// linux
	try
	{	
		// ��ǰ�ֶε��ֽڳ���
		SQLLEN		iActualDataSize = 0;
		SQLCHAR			szData[FIELD_DATA_LEN] = {0};
		memset(szData,0x0,FIELD_DATA_LEN);
		SQLRETURN		iRetFlag = 0;

		// ��ȡ������
		SQLSMALLINT		iColumnIndex = GetColumnIndexByName(szFieldName);
		if(iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return FALSE;
		}
		// ��ȡ����
		iRetFlag = SQLGetData(m_hSqlStmt,iColumnIndex,SQL_C_CHAR,szData,FIELD_DATA_LEN,&iActualDataSize);
		// �жϻ�ȡ�����Ƿ�ɹ�
		if (SQL_SUCCEEDED(iRetFlag)) 
		{
			// ������ȴ�С���ԣ����ش���
			if (iActualDataSize > FIELD_DATA_LEN)
			{
				return	FALSE;
			}
			else
			{
				// ��ȡ�����ݳ���С�ڴ���ʵ�ʵ����ݳ���
				// ��ȡ���ݳɹ�
				szValue	=	string((char*)szData);
			}
			return	TRUE;

		}
		else
		{
			// ��ȡ����ʧ��
			return	FALSE;
		}

	}
	catch(...)
	{
		return	FALSE;
	}
	return FALSE;
}

// �����������ö�Ӧ�е�ֵ
BOOL	CMySQLRecordSet::PutCollect(const char* szFieldName,const char*     szValue)
{
	try
	{
		// ��ȡ��Ӧ��������
		INT		iColumnIndex = GenerateCollumnIndexID();/* GetColumnIndexByName(szFieldName);*/
		if (iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return	FALSE;
		}

		// �����ֶ���Ϣ
		CDBFieldInfo*	pcsDbFieldInfoItem = NULL;
		pcsDbFieldInfoItem = new CDBFieldInfo();

		// ����������
		pcsDbFieldInfoItem->SetColumnIndex(iColumnIndex);
		// ����������
		pcsDbFieldInfoItem->SetFieldName(szFieldName);
		// �����ֶ�����
		pcsDbFieldInfoItem->SetFieldType(FT_String);
		// ����ֵ
		pcsDbFieldInfoItem->SetStringValue(szValue);

		// ��ӵ��б���
		if(AddFieldItem2FieldInfoList(pcsDbFieldInfoItem))
		{
			// ���ñ༭ģʽ
			if (m_iEditMode == EM_UnKnown )
			{
				m_iEditMode = EM_Edit;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return	FALSE;
	}
	return	FALSE;
}

// �������������ö�Ӧʱ���ֶε�ֵ
BOOL	CMySQLRecordSet::PutDtCollect(const char* szFieldName,const INT iYear,const INT iMonth,const INT iDay,const INT iHour,const INT iMinute,const INT iSecond)
{
	try
	{
		// ��ȡ��Ӧ��������
		INT		iColumnIndex = GenerateCollumnIndexID();
		if (iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return	FALSE;
		}

		// �����ֶ���Ϣ
		CDBFieldInfo*	pcsDbFieldInfoItem = NULL;
		pcsDbFieldInfoItem = new CDBFieldInfo();
		// ����������
		pcsDbFieldInfoItem->SetColumnIndex(iColumnIndex);
		// ����������
		pcsDbFieldInfoItem->SetFieldName(szFieldName);
		// �����ֶ�����
		pcsDbFieldInfoItem->SetFieldType(FT_DateTime);
		// ����ֵ
		pcsDbFieldInfoItem->SetDateTimeValue(iYear,iMonth,iDay,iHour,iMinute,iSecond);

		// ��ӵ��б���
		if(AddFieldItem2FieldInfoList(pcsDbFieldInfoItem))
		{
			// ���ñ༭ģʽ
			if (m_iEditMode == EM_UnKnown )
			{
				m_iEditMode = EM_Edit;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return	FALSE;
	}
	return	FALSE;
}

// �ͷ����ݼ�����
BOOL	CMySQLRecordSet::ReleaseRecordSet()
{
	delete	this;
	return  TRUE;
}

INT CMySQLRecordSet::GetColumnNumber(void)
{
	if(m_hSqlStmt == SQL_NULL_HSTMT)
	{
		return	-1;
	}
	return m_ColumnItemList.size();	

}


BOOL CMySQLRecordSet::GetCloumnName(INT iColIndex, std::string &oStrName )
{
	oStrName.clear();
	if(m_hSqlStmt == SQL_NULL_HSTMT || iColIndex<0 || iColIndex>=(int) m_ColumnItemList.size() )
	{
		return	FALSE;
	}
	oStrName = m_ColumnItemList[iColIndex].strColumn;
	return TRUE;
}

// ���������ƻ�ȡ��Ӧ�ֶε�ֵ������INT �����ַ���
BOOL	CMySQLRecordSet::GetCollect(const char* szFieldName,INT&	iValue)
{
	// linux
	try
	{	
		// ��ǰ�ֶε��ֽڳ���
		SQLLEN		iActualDataSize = 0;
		SQLCHAR			szData[FIELD_DATA_LEN] = {0};
		memset(szData,0x0,FIELD_DATA_LEN);
		SQLRETURN		iRetFlag = 0;

		// ��ȡ������
		SQLSMALLINT		iColumnIndex = GetColumnIndexByName(szFieldName);
		if(iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return FALSE;
		}
		// ��ȡ����
		iRetFlag = SQLGetData(m_hSqlStmt,iColumnIndex,SQL_C_CHAR,szData,FIELD_DATA_LEN,&iActualDataSize);
		// �жϻ�ȡ�����Ƿ�ɹ�
		if (SQL_SUCCEEDED(iRetFlag)) 
		{
			// ������ȴ�С���ԣ����ش���
			if (iActualDataSize > FIELD_DATA_LEN)
			{
				return	FALSE;
			}
			else
			{
				// ��ȡ�����ݳ���С�ڴ���ʵ�ʵ����ݳ���
				// ��ȡ���ݳɹ�
				iValue = atol((char*)szData);
			}

			return	TRUE;

		}
		else
		{
			// ��ȡ����ʧ��
			return	FALSE;
		}

	}
	catch(...)
	{
		return	FALSE;
	}
	return TRUE;
}

// ���������ƻ�ȡ��Ӧ�ֶε�ֵ������INT �����ַ���
BOOL	CMySQLRecordSet::PutCollect(const char* szFieldName,const INT	iValue)
{
	try
	{
		// ��ȡ��Ӧ��������
		INT		iColumnIndex = GenerateCollumnIndexID();/*GetColumnIndexByName(szFieldName);*/
		if (iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return	FALSE;
		}

		// �����ֶ���Ϣ
		CDBFieldInfo*	pcsDbFieldInfoItem = NULL;
		pcsDbFieldInfoItem = new CDBFieldInfo();
		// ����������
		pcsDbFieldInfoItem->SetColumnIndex(iColumnIndex);
		// ����������
		pcsDbFieldInfoItem->SetFieldName(szFieldName);
		// �����ֶ�����
		pcsDbFieldInfoItem->SetFieldType(FT_Integer);
		// ����ֵ
		pcsDbFieldInfoItem->SetIntegerValue(iValue);

		// ��ӵ��б���
		if(AddFieldItem2FieldInfoList(pcsDbFieldInfoItem))
		{
			// ���ñ༭ģʽ
			if (m_iEditMode == EM_UnKnown )
			{
				m_iEditMode = EM_Edit;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return	FALSE;
	}
	return	FALSE;
}


// �����ֶ�����������Ӧ��ֵ
BOOL	CMySQLRecordSet::GetCollect(const char* szFieldName,void*	   pValue,const INT iBuffSize,INT&	iDataLen)
{
	// linux

	// �������ݴ洢��
	SQLCHAR*		pszData = NULL;
	try
	{	
		// ��ǰ�ֶε��ֽڳ���
		SQLLEN		iActualDataSize = 0;

		// �����ڴ�
		pszData = new SQLCHAR[iBuffSize + 1];
		memset(pszData,0x0,iBuffSize+1);
		SQLRETURN		iRetFlag = 0;

		// ��ȡ������
		SQLSMALLINT		iColumnIndex = GetColumnIndexByName(szFieldName);
		if(iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return FALSE;
		}
		// ��ȡ����
		iRetFlag = SQLGetData(m_hSqlStmt,iColumnIndex,SQL_C_BINARY,pszData,iBuffSize,&iActualDataSize);
		// �жϻ�ȡ�����Ƿ�ɹ�
		if (SQL_SUCCEEDED(iRetFlag)) 
		{
			if (iBuffSize < iActualDataSize)
			{
				// ������С���������ݳ��ȣ�ֻ���ش�С
				iDataLen = iActualDataSize;
			}
			else if (iActualDataSize <=0)
			{
				iDataLen = coniZero;
			}
			else
			{
				// �������ݣ����ҷ��ش�С
				memcpy(pValue,pszData,iActualDataSize);
				iDataLen = iActualDataSize;
			}

		}
		else
		{
			if (pszData != NULL)
			{
				delete[] pszData;
				pszData = NULL;
			}
			// ��ȡ����ʧ��
			return	FALSE;
		}

		if (pszData != NULL)
		{
			delete[] pszData;
			pszData = NULL;
		}

	}
	catch(...)
	{
		if (pszData != NULL)
		{
			delete[] pszData;
			pszData = NULL;
		}
		return	FALSE;
	}
	return TRUE;
}

// �����ֶ�����������Ӧ��ֵ
BOOL	CMySQLRecordSet::PutCollect(const char* szFieldName,const  void*	   pValue,const INT	iDataLen)
{
	try
	{
		// ��ȡ��Ӧ��������
		INT		iColumnIndex = GenerateCollumnIndexID();/*GetColumnIndexByName(szFieldName);*/
		if (iColumnIndex == ERROR_SQL_COLUMN_INDEX)
		{
			return	FALSE;
		}

		// �����ֶ���Ϣ
		CDBFieldInfo*	pcsDbFieldInfoItem = NULL;
		pcsDbFieldInfoItem = new CDBFieldInfo();
		// ����������
		pcsDbFieldInfoItem->SetColumnIndex(iColumnIndex);
		// ����������
		pcsDbFieldInfoItem->SetFieldName(szFieldName);
		// �����ֶ�����
		pcsDbFieldInfoItem->SetFieldType(FT_Binary);
		// ���ö��������ݵĳ���
		pcsDbFieldInfoItem->SetBinaryValueLen(iDataLen);
		// ���ö���������ֵ(���ݵ�ֵ�ʹ�С)
		pcsDbFieldInfoItem->SetBinaryValue(pValue,iDataLen);

		// ��ӵ��б���
		if(AddFieldItem2FieldInfoList(pcsDbFieldInfoItem))
		{
			// ���ñ༭ģʽ
			if (m_iEditMode == EM_UnKnown )
			{
				m_iEditMode = EM_Edit;
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return	FALSE;
	}
	return TRUE;

}

// ִ��Sql��䷵�����ݼ�
IRecordSet*		CMySQLRecordSet::QuerySql(const char*	szSql)
{	
	//	ASSERT(m_pRecordset != NULL)
	// ִ�в����������ݼ�����
	if (Open(szSql))
	{
		return (IRecordSet*)this;
	}
	else
	{
		return	NULL;
	}

}

// ��ȡ���ݼ��ж��Ƿ���Խ��б༭
BOOL		CMySQLRecordSet::GetEditMode()
{
	return	FALSE; 
}


// �������Ӷ���
BOOL			CMySQLRecordSet::SetConnection(CMySQLConnection* pConnection)
{
	if (pConnection != NULL)
	{
		m_pConnection	=	pConnection;
		return	TRUE;
	}
	return FALSE;

}

// ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���
BOOL	CMySQLRecordSet::CancelUpdate()
{
	//linux
	ClearFieldInfoList();	

	// ���ñ༭ģʽΪUnKnown
	m_iEditMode = EM_UnKnown;

	// �ͷŸ��¾��
	if (m_hUpdateSqlStmt!=SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,m_hUpdateSqlStmt);
		m_hUpdateSqlStmt = SQL_NULL_HSTMT;
	}
	// �쳣����
	return FALSE;
}


// 
BOOL	CMySQLRecordSet::IsOpen()
{
	// linux
	return	m_bIsOpen;

}


// �ر����ݼ�
BOOL	CMySQLRecordSet::Close()
{
	//linux
	if (IsOpen())
	{
		m_bIsOpen = FALSE;
		SQLFreeHandle(SQL_HANDLE_STMT,m_hSqlStmt);
		m_hSqlStmt = SQL_NULL_HSTMT;
	}
	return	TRUE;

}


// �����ݼ�
BOOL	CMySQLRecordSet::Open(const char*  szSql)
{
	//linux
	// �����ݼ�����
	try
	{
		// �ر����ݼ�����
		if (IsOpen()) 
		{
			Close();
		}

		// �ж�׼�������Ƿ�ɹ�
		if (Init()) 
		{	
			// ִ��sql���
			SQLINTEGER	iRetCode = -1;
			iRetCode	=	SQLExecDirect(m_hSqlStmt, (SQLCHAR*)szSql, SQL_NTS);
			if ( iRetCode == SQL_ERROR)
			{
				//----------------------------------------------------------------------------------
				// ���������Ϣ
				ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
				//----------------------------------------------------------------------------------
				//MY_ERR_THROW();
				Close();
				return FALSE;
			}
			else
			{
				m_strTableName = constrDefaultString ;
				// �Ѿ�ִ�гɹ��������ݼ���
				m_bIsOpen = TRUE;
				// ��ȡ���������Ϣ�����б�
				GetColumnList();
				// ����ƶ�����һ����¼
				iRetCode = SQLFetchScroll(m_hSqlStmt, SQL_FETCH_FIRST, 0);
				// �ж��Ƿ��м�¼����(�Ƿ��м�¼��)
				if((iRetCode != SQL_SUCCESS) && (iRetCode != SQL_SUCCESS_WITH_INFO))
				{
					m_bEof = TRUE;
				}
				else
				{
					m_bEof = FALSE;
				}
				// ��Sql�ַ�������ȡ��������
				if (!GetTableNameFromSqlText(szSql,m_strTableName))
				{
					// ����Ҳ��������ƣ��Ͳ��ܹ����£���ӵȲ���
					m_iEditMode = EM_NoUpdate;
				}

				// ����������к����־
				InitCollumnIndexID();

				return  TRUE;		
			}
		}
		else
		{	
			return FALSE;	
		}

	}
	catch (...)
	{
		// ����Ͽ������ˣ�Ҫ������������
		m_pConnection->GetConnectionErrorAndDeal();

		m_bIsOpen = FALSE;
		return FALSE;
	}
	return TRUE;
}

// ��ʼ��STMT��������Ϣ
BOOL		CMySQLRecordSet::Init()
{
	try
	{ 
		// ���жϾ���Ƿ����
		if ( (m_pConnection->GetOdbcSqlHDbc() == SQL_NULL_HANDLE ) || (!m_pConnection->IsOpen()))
		{
			return FALSE;
		}

		SQLRETURN	retCode = 0;
		//����SQL�����
		retCode = SQLAllocHandle( SQL_HANDLE_STMT,m_pConnection->GetOdbcSqlHDbc(),&m_hSqlStmt);

		// �ж��Ƿ�����ɹ�
		if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) 
		{	
			//ָ��Ҫʹ�õ��α겢������
			retCode = SQLSetStmtAttr(m_hSqlStmt, SQL_ATTR_CONCURRENCY,(SQLPOINTER) SQL_CONCUR_ROWVER, 0);
			if ( (retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO)) 
			{
				//----------------------------------------------------------------------------------
				// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
				ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hSqlStmt);
				//----------------------------------------------------------------------------------
				Close(); 
				return FALSE;
			}


			//���ù������Ϊ�������,
			//��������ܹ���⵽�е�ɾ�����޸ģ������޷���⵽��⵽�е���Ӻͽ����˳��仯��
			//��Ϊ�ڹ�괴��ʱ�ʹ������������������������м�¼��˳���Ѿ����̶���
			//��һ��;�̬���һ�������Լ���������˵��һ�ֽ��ھ�̬���Ͷ�̬���֮��Ĺ�����͡�
			retCode = SQLSetStmtAttr(m_hSqlStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC /* SQL_CURSOR_KEYSET_DRIVEN*/, 0);
			if ( (retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO)) 
			{
				//----------------------------------------------------------------------------------
				// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
				ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hSqlStmt);
				//----------------------------------------------------------------------------------
				Close(); 
				return FALSE;
			}

			//-----------------------------MySQL��֧���α�����-------------------------
			//// �����α�����
			//char		szCursorName[MAX_CURSOR_NAME_LEN] = {0x0};
			//sprintf(szCursorName,"%s_%d",CURRENT_CURSOR_NAME,GetCursorIndex());
			//retCode = SQLSetCursorName(m_hSqlStmt,reinterpret_cast<SQLCHAR*>(szCursorName), SQL_NTS);
			//if ( (retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO)) 
			//{
			//	//----------------------------------------------------------------------------------
			//	// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
			//	
			//	int ierror;
			//	ierror= GetLastError();
			//	ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hSqlStmt);
			//	//----------------------------------------------------------------------------------
			//	Close(); 
			//	return FALSE;
			//}

		}
		else
		{
			Close();
			return FALSE;
		}
	}
	catch (...)
	{
		Close();
		return FALSE;
	}

	return TRUE;
}

// ��ȡ�ֶ������б�
BOOL		CMySQLRecordSet::GetColumnList()
{
	if (m_hSqlStmt == SQL_NULL_HSTMT)
	{
		return FALSE;
	}
	m_ColumnItemList.clear();

	// ����
	SQLSMALLINT		iColumnNum = 0;
	// ��ȡ������
	SQLNumResultCols(m_hSqlStmt, &iColumnNum);
	SQLCHAR			szColumnName[MAX_FNAME_LEN] = {0};


	int nType = SQL_C_DEFAULT;
	// ��Щ���ݣ�����������У����ǲ��ù���
	SQLSMALLINT		nSwCol=0, nSwType=0, nSwScale=0, nSwNull=0; 
	SQLULEN		pcbColDef=0;
	SQLRETURN		iRet=0;

	// ��ȡ����
	for(INT	i = 1;i<= iColumnNum ;i++)
	{
		// ����
		memset(szColumnName,0x0,MAX_FNAME_LEN);
		// ��ȡ�е������Ϣ
		iRet = SQLDescribeCol(m_hSqlStmt, i,(SQLTCHAR*)szColumnName,MAX_FNAME_LEN,&nSwCol, &nSwType, &pcbColDef,&nSwScale, &nSwNull); 
		if (iRet == SQL_SUCCESS || iRet == SQL_SUCCESS_WITH_INFO)
		{
			// ��ȡ��������Ϣ
			StruColumnItem		stColumnItem;
			stColumnItem.strColumn = string((char*)szColumnName);
			stColumnItem.iColumnIndex = i;

			// �������
			m_ColumnItemList.push_back(stColumnItem);
		}
		else
		{
			// �ر�
			Close();
			return FALSE;
		}
	}

	return	TRUE;
}

// ����ֶ��б�
BOOL		CMySQLRecordSet::ClearColumnList()
{
	m_ColumnItemList.clear();
	return	TRUE;

}


// �����ֶ����ƣ���ȡ�е�����
INT		  CMySQLRecordSet::GetColumnIndexByName(const char* szFieldName)
{
	if(m_hSqlStmt == SQL_NULL_HSTMT)
	{
		return	ERROR_SQL_COLUMN_INDEX;
	}

	INT		iColumnIndex = ERROR_SQL_COLUMN_INDEX;
	// �����ַ���
	string	strColumnName = string(szFieldName);
	// ��ȡ�����ƶ�Ӧ����������
	for (ColumnItemVector::size_type i = 0;i< m_ColumnItemList.size();i++)
	{
		if(strColumnName == m_ColumnItemList[i].strColumn)
		{
			iColumnIndex = m_ColumnItemList[i].iColumnIndex;
			break;
		}
	}
	// �����е�����
	return	iColumnIndex;

}

// ��������ֶ��б�m_FieldInfoList �� AddNew �� Update��ʱ���õģ�
BOOL		CMySQLRecordSet::ClearFieldInfoList()
{
	// ����ֶ��б�
	for (DBFieldInfoVector::size_type i = 0;i<m_FieldInfoList.size();i++)
	{
		// ����ڴ�
		CDBFieldInfo*	pDbFieldInfo = NULL;
		pDbFieldInfo = m_FieldInfoList[i];
		if (pDbFieldInfo!=NULL)
		{
			pDbFieldInfo->Clear();
			delete	pDbFieldInfo;
			pDbFieldInfo = NULL;
		}
	}
	m_FieldInfoList.clear();

	return TRUE;

}

// ���ֶ��б�������ֶ���Ϣ
BOOL		CMySQLRecordSet::AddFieldItem2FieldInfoList(CDBFieldInfo*	pcsDbFieldInfoItem)
{	
	// ���ж������Ƿ����
	for (DBFieldInfoVector::size_type i = 0 ;i <m_FieldInfoList.size();i++)
	{
		if (m_FieldInfoList[i]->GetFieldName() == pcsDbFieldInfoItem->GetFieldName())
		{
			// �Ѿ����ھͷ���
			return	FALSE;
		}
	}

	// ��ӵ��б���
	m_FieldInfoList.push_back(pcsDbFieldInfoItem);
	return	TRUE;

}

// ���ݲ�ѯ���ݵ�sql�������ȡ���еı�����(��Ҫ�ǶԱ��ѯ�󣬽��в������ݺ͸������ݵ�ʱ����)
BOOL		CMySQLRecordSet::GetTableNameFromSqlText(const char* szSql,string& strTableName)
{
	try
	{
		string	strSql  = string(szSql);
		// ���������һ��from������ַ���


		// ת��Ϊ��д
		// transform (strSql.begin(),strSql.end(), strSql.begin(),toupper); 
		transform(strSql.begin(),strSql.end(),strSql.begin(),ToUpper());


		// FROM �ַ����ֵ�λ��
		string::size_type	nFromPos = strSql.find(constrFromKeyWord.c_str());

		// û���ҵ�
		if (nFromPos == string::npos)
		{
			return	FALSE;
		}

		// ȥ��FROM��ǰ���ַ���
		strSql	=	strSql.substr(nFromPos+4,strSql.size() - nFromPos -4);

		// ɾ������ǰ��Ŀո��ַ���
		strSql.erase(0,strSql.find_first_not_of(" "));
		// ȥ������Ŀո�
		strSql.erase(strSql.find_last_not_of(" ") + 1,strSql.size()-1);

		// ��ʼ���ұ����� "   TABLE "����" TABLE"��
		// �����ƽ���λ��
		string::size_type	nTableTailPos  = strSql.find_first_of(" ");
		if (nTableTailPos == string::npos)
		{
			// ��������û�пո�
			strTableName = strSql;
		}
		else
		{
			// ��ȡ����
			strTableName	=	strSql.substr(0,nTableTailPos);
		}

	}
	catch(...)
	{
		return	FALSE;
	}

	return	TRUE;
}


// ����Sql�����в���
BOOL		CMySQLRecordSet::BuildInsertUpdateSqlTxt()
{
	// ȷ����Ԫ�ش���
	if (m_FieldInfoList.empty())
	{
		return FALSE;
	}

	if (m_hUpdateSqlStmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,m_hUpdateSqlStmt);
		m_hUpdateSqlStmt = SQL_NULL_HSTMT;
	}

	// ���ؾ��
	SQLRETURN retcode;
	// ����Sql�����
	retcode = SQLAllocHandle(SQL_HANDLE_STMT,m_pConnection->GetOdbcSqlHDbc(),&m_hUpdateSqlStmt);	
	// �ж��Ƿ�����ɹ�
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{	
		// ����ַ���
		m_strSqlText = constrDefaultString;

		// �ж�����
		if (m_iEditMode == EM_AddNew)
		{
			// INSERT ����
			m_strSqlText = "INSERT INTO ";
			m_strSqlText += m_strTableName;
			m_strSqlText += " ( ";
			// ������Ƽ���
			for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)
			{
				m_strSqlText += m_FieldInfoList[i]->GetFieldName();
				if (i!=m_FieldInfoList.size() - 1)
				{
					m_strSqlText += ",";
				}
				else
				{
					m_strSqlText += " ";
				}
			}

			m_strSqlText += " ) VALUES (";

			// ����ֵ
			for (DBFieldInfoVector::size_type j=0;j<m_FieldInfoList.size();j++)
			{
				m_strSqlText += "?";
				if (j!=m_FieldInfoList.size() - 1)
				{
					m_strSqlText += ",";
				}
				else
				{
					m_strSqlText += ")";
				}
			}

			// ���磺 INSERT INTO TB_TEST(ID,NAME,KK) VALUES(?,?,?)

			// ׼��Sql ���
			SQLRETURN	retCode = 0;
			retcode = SQLPrepare(m_hUpdateSqlStmt,(SQLCHAR*)m_strSqlText.c_str(),SQL_NTS);
			if (retcode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				return TRUE;
			}
			//----------------------------------------------------------------------------------
			// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
			ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
			//----------------------------------------------------------------------------------

		}
		else if (m_iEditMode == EM_Edit)
		{
			// UPDATE ����
			m_strSqlText = "UPDATE ";
			m_strSqlText += m_strTableName;
			m_strSqlText += " Set ";

			// ��������
			// ������Ƽ���
			for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)
			{
				m_strSqlText += m_FieldInfoList[i]->GetFieldName();
				m_strSqlText += " = ? ";
				if (i!=m_FieldInfoList.size() - 1)
				{
					m_strSqlText += ",";
				}
				else
				{
					m_strSqlText += " ";
				}
			}

			// ������������,ͨ���α�������
			// m_strSqlText += " WHERE CURRENT OF CURRENT_CURSOR_NAME";

			m_strSqlText += " WHERE CURRENT OF ";
			SQLRETURN	nRetCode = 0;
			unsigned char	szCursorName[MAX_CURSOR_NAME_LEN+1] = {0};
			memset(szCursorName,0x0,MAX_CURSOR_NAME_LEN+1);
			SQLSMALLINT			iLenght = 0;
			nRetCode	=	SQLGetCursorName(m_hSqlStmt,szCursorName,MAX_CURSOR_NAME_LEN,&iLenght);
			if (nRetCode == SQL_SUCCESS || nRetCode == SQL_SUCCESS_WITH_INFO)
			{
				string	strTemp = (char*)szCursorName;
				m_strSqlText	+= strTemp;
			}
			else
			{
				//----------------------------------------------------------------------------------
				// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
				ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
				//----------------------------------------------------------------------------------
				return FALSE;
			}

			// ���磺 UPDATE TB_TEST SET NAME = ? ,PASS= ? WHERE KK = 'E'

			// ׼��Sql ���
			SQLRETURN	retCode = 0;
			retcode = SQLPrepare(m_hUpdateSqlStmt,(SQLCHAR*)m_strSqlText.c_str(),SQL_NTS);
			if (retcode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				return TRUE;
			}
			//----------------------------------------------------------------------------------
			// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
			ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
			//----------------------------------------------------------------------------------
		}
		else
		{	
			// ֻ�������ºͲ���ģ����״̬��Ϊ����
			SQLFreeHandle(SQL_HANDLE_STMT,m_hUpdateSqlStmt);
			m_hUpdateSqlStmt = SQL_NULL_HSTMT;

			return FALSE;
		}

	}
	else
	{	
		// ����Sql�����ʧ��
		SQLFreeHandle(SQL_HANDLE_STMT, m_hSqlStmt);
		return FALSE;	
	}

	return	FALSE;

}


// ��Sql���������Ӷ�Ӧ��ֵ
BOOL		CMySQLRecordSet::AppendValueToSqlTxt()
{
	// ȷ����Ԫ�ش���
	if (m_FieldInfoList.empty() || m_strSqlText.empty())
	{
		return FALSE;
	}

	// SQL ����ֵ
	SQLRETURN	retCode;

	// �ж϶����Ƿ����
	if (m_hUpdateSqlStmt != SQL_NULL_HSTMT) 
	{	
		// �ж�����
		if (m_iEditMode == EM_AddNew)
		{
			// ���磺 INSERT INTO TB_TEST(ID,NAME,KK) VALUES(?,?,?)

			// ���ֵ�İ����
			for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)
			{
				switch (m_FieldInfoList[i]->GetFieldType())
				{
				case	FT_Integer:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(),
							SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER, 0, 
							0,(SQLINTEGER*)&(m_FieldInfoList[i]->m_iRefValue),0,
							(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������
						m_FieldInfoList[i]->m_iRefValue = m_FieldInfoList[i]->GetIntegerValue();
						m_FieldInfoList[i]->m_icpValue = SQL_NTS;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}
					} 
					break;
				case	FT_String:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(), 
							SQL_PARAM_INPUT,SQL_C_CHAR,SQL_VARCHAR,	FIELD_DATA_LEN,0,
							(SQLCHAR*)(m_FieldInfoList[i]->m_szRefValue),0,
							(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������
						strcpy((m_FieldInfoList[i]->m_szRefValue),m_FieldInfoList[i]->GetStringValue().c_str());
						// ��������
						m_FieldInfoList[i]->m_icpValue = SQL_NTS;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} 
					break;
				case	FT_DateTime:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt,m_FieldInfoList[i]->GetColumnIndex(),
							SQL_PARAM_INPUT,SQL_C_TIMESTAMP,SQL_DATETIME,
							sizeof(TIMESTAMP_STRUCT),0,&m_FieldInfoList[i]->m_stDateTime,
							sizeof(TIMESTAMP_STRUCT),(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue)); 

						// ��������
						m_FieldInfoList[i]->m_icpValue = 3;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					}
					break;
				case FT_Binary:
					{
						// �����ƴ���� FT_Binary

						// �ر�˵����ORACLE �� SQL SERVER �Ķ����Ʋ���������һ������ķ�����һ��		
						// ����˳
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(), 
							SQL_PARAM_INPUT,SQL_C_BINARY,SQL_LONGVARBINARY,
							0,0,
							(SQLPOINTER)m_FieldInfoList[i]->m_pValue,
							m_FieldInfoList[i]->GetBinaryValueLen(),(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������					
						/*
						The length of the parameter value stored in *ParameterValuePtr. This is ignored except for character or binary C data.
						SQL_NTS. The parameter value is a null-terminated string.
						*/
						m_FieldInfoList[i]->m_icpValue = m_FieldInfoList[i]->GetBinaryValueLen();

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} 
					break;
				default:
					break;
				}

			} // end for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)

		} // end  if (m_iEditMode == EM_AddNew)
		else if (m_iEditMode == EM_Edit)
		{
			// UPDATE ����
			// ���磺 UPDATE TB_TEST SET NAME = ? ,PASS= ? WHERE KK = 'E'

			// ���ֵ�İ����
			for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)
			{
				switch (m_FieldInfoList[i]->GetFieldType())
				{
				case FT_Integer:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(),
							SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER, 0, 
							0,(SQLINTEGER*)&(m_FieldInfoList[i]->m_iRefValue),0,
							(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������
						m_FieldInfoList[i]->m_iRefValue = m_FieldInfoList[i]->GetIntegerValue();
						m_FieldInfoList[i]->m_icpValue = SQL_NTS;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} 
					break;
				case FT_String:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(), 
							SQL_PARAM_INPUT,SQL_C_CHAR,SQL_VARCHAR,	FIELD_DATA_LEN,0,
							(SQLCHAR*)(m_FieldInfoList[i]->m_szRefValue),0,
							(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������
						strcpy((m_FieldInfoList[i]->m_szRefValue),m_FieldInfoList[i]->GetStringValue().c_str());
						// ��������
						m_FieldInfoList[i]->m_icpValue = (INT)SQL_NTS;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} 
					break;
				case FT_DateTime:
					{
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt,m_FieldInfoList[i]->GetColumnIndex(),
							SQL_PARAM_INPUT,SQL_C_TIMESTAMP,SQL_DATETIME,
							sizeof(TIMESTAMP_STRUCT), 0,&m_FieldInfoList[i]->m_stDateTime, 
							sizeof(TIMESTAMP_STRUCT),(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue)); 

						// ��������
						m_FieldInfoList[i]->m_icpValue = 3;

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} 
					break;
				case FT_Binary:
					{
						// �����ƴ���� FT_Binary

						// �ر�˵����ORACLE �� SQL SERVER �Ķ����Ʋ���������һ������ķ�����һ��		
						// ����˳
						// ������
						retCode = SQLBindParameter(m_hUpdateSqlStmt, m_FieldInfoList[i]->GetColumnIndex(), 
							SQL_PARAM_INPUT,SQL_C_BINARY,SQL_LONGVARBINARY,
							0,0,
							(SQLPOINTER)m_FieldInfoList[i]->m_pValue,
							m_FieldInfoList[i]->GetBinaryValueLen(),(SQLLEN*)&(m_FieldInfoList[i]->m_icpValue));

						// ��������					
						/*
						The length of the parameter value stored in *ParameterValuePtr. This is ignored except for character or binary C data.
						SQL_NTS. The parameter value is a null-terminated string.
						*/
						m_FieldInfoList[i]->m_icpValue = m_FieldInfoList[i]->GetBinaryValueLen();

						// �жϰ��Ƿ�ɹ�
						if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
						{
							//----------------------------------------------------------------------------------
							// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
							ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
							//----------------------------------------------------------------------------------
							return FALSE;
						}

					} // end  FT_Binary
					break;

				default:
					break;
				}

			} // end for (DBFieldInfoVector::size_type i=0;i<m_FieldInfoList.size();i++)

		} //  end else if (m_iEditMode == EM_Edit)
		else
		{	
			// ֻ�������ºͲ���ģ����״̬��Ϊ����
			SQLFreeHandle(SQL_HANDLE_STMT,m_hUpdateSqlStmt);
			m_hUpdateSqlStmt = SQL_NULL_HSTMT;

			return FALSE;
		}

	}	//  end if (m_hUpdateSqlStmt != SQL_NULL_HSTMT) 
	else
	{	
		return FALSE;	
	}

	return TRUE;

}

// ִ��SQL ������߸��²���(INSERT �� UPDATE)
BOOL		CMySQLRecordSet::ExecuteInsertUpdate()
{
	// ִ�в���
	if (m_hUpdateSqlStmt != SQL_NULL_HSTMT)
	{
		// SQL ����ֵ
		SQLRETURN	retCode;

		// ִ��Sql����
		retCode = SQLExecute(m_hUpdateSqlStmt);
		if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
		{
			//----------------------------------------------------------------------------------
			// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
			ErrorLogInfo(m_pConnection->GetOdbcSqlHEven(),m_pConnection->GetOdbcSqlHDbc(),m_hUpdateSqlStmt);
			//----------------------------------------------------------------------------------
			return	FALSE;
		}
	}
	else
	{
		return	FALSE;
	}

	return TRUE;

}
