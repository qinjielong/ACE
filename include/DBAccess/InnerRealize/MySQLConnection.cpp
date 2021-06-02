#include "MySQLConnection.h"
#include "MySQLRecordSet.h"
#include "CDBConnectionPool.h"
#include "DLPUtility.h"

using namespace DBAccessModule;


CMySQLConnection::CMySQLConnection(void)
{
	// ���Ӿ��
	m_hDbc	=	SQL_NULL_HANDLE;
	// �������
	m_hEnv	=	SQL_NULL_HANDLE;	
	// ����״̬��־
	m_bConnectFlag = FALSE;
}

CMySQLConnection::~CMySQLConnection(void)
{
	if (IsOpen())
	{	
		// �Ͽ�����
		SQLDisconnect(m_hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);

		// ������Դ
		m_hEnv = SQL_NULL_HANDLE;
		m_hDbc = SQL_NULL_HANDLE;

		// ����״̬��־
		m_bConnectFlag = FALSE; 
	}
}


// �����ݿ�����
BOOL	CMySQLConnection::Open()	
{
	//#ifdef _WIN32
	// linux
	// ����ֵ����
	SQLRETURN	retcode;

	std::cout<< "opening mysql ..." <<endl;
	// ����Sql�������
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) ;
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		// ����SQL �����������
		retcode = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			// ����sql���Ӿ��
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{
				// �������ݿ�
				retcode = SQLConnect(m_hDbc, (SQLCHAR*)m_szDatabase, SQL_NTS,NULL/* (SQLCHAR*)m_szUser*/, SQL_NTS, 
					NULL/*(SQLCHAR*)m_szPass*/, SQL_NTS);

				// �ж������ӳɹ�
				if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO))
				{					
					std::cout<<"---> DBAccess Error��CMySQLConnection::Open() DB SQLConnect failed !"<<endl;
					// ���������Ϣ
					INT iErrno = 0;
					ErrorLogInfo(m_hEnv,m_hDbc,NULL, &iErrno);

					SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); 
					SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
					return FALSE;
				}
			} 
			else
			{
				std::cout<<"---> DBAccess Error��CMySQLConnection::Open() DB SQLAllocHandle failed !"<<endl;
				// ����Sql���Ӿ��ʧ��
				SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); 
				SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
				return FALSE;
			}
		}
		else
		{
			std::cout<<"---> DBAccess Error��CMySQLConnection::Open() DB SQLSetEnvAttr failed !"<<endl;
			// ����Sql �������ʧ��
			SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
			return FALSE;
		}
	}
	else
	{
		std::cout<<"---> DBAccess Error��CMySQLConnection::Open() DB SQLAllocHandle failed !"<<endl;
		// ����Sql�������ʧ��
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		return FALSE;
	}
	std::cout<< "opening mysql success" << endl;
	m_bConnectFlag = TRUE;

	return  TRUE;
}

BOOL	CMySQLConnection::Close()
{
	//#ifdef _WIN32
	// linux
	// ����Ǵ򿪵�
	if(m_bConnectFlag)
	{
		// �Ͽ����ݿ�����
		SQLDisconnect(m_hDbc);
		// �ͷ����ݿ����Ӿ��
		SQLFreeHandle(SQL_HANDLE_DBC,m_hDbc); 
		// �ͷ����ݿ⻷�����
		SQLFreeHandle(SQL_HANDLE_ENV,m_hEnv);

		// ���Ӿ��
		m_hDbc	=	SQL_NULL_HANDLE;
		// �������
		m_hEnv	=	SQL_NULL_HANDLE;	

		// ���ӱ�־
		m_bConnectFlag = FALSE;
	}
	return	TRUE;

}

BOOL	CMySQLConnection::IsOpen()
{
	//#ifdef _WIN32
	// linux
	return	m_bConnectFlag;

}

// ��ʼִ������
UINT	CMySQLConnection::BeginTrans()
{
	// linux
	// ��ʼ����
	SQLRETURN	retcode;
	// ��ʼ������

	// ����Ϊ�ֶ��ύģʽ, SQL_AUTOCOMMIT_OFF
	retcode	=	SQLSetConnectAttr (m_hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_OFF, SQL_IS_POINTER);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		return	ERROR_DB_TRANS_RET_SUCCESS;
	}
	return	ERROR_DB_TRANS_RET_ERROR;
}

// ����Ļع�����
BOOL	CMySQLConnection::RollbackTrans()
{
	SQLRETURN	retcode;
	// ��ʼ�ع�����
	retcode	=	SQLTransact(m_hEnv,m_hDbc,SQL_ROLLBACK);
	if(retcode == SQL_SUCCESS_WITH_INFO || retcode == SQL_SUCCESS)
	{
		// ����Ϊ�Զ��ύģʽ
		retcode	=	SQLSetConnectAttr (m_hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_IS_POINTER);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			return	ERROR_DB_TRANS_RET_SUCCESS;
		}
	}
	return	ERROR_DB_TRANS_RET_ERROR;
}

// ������ύ
BOOL	CMySQLConnection::CommitTrans()
{
	SQLRETURN	retcode;
	// ��ʼ������
	retcode	=	SQLTransact(m_hEnv,m_hDbc,SQL_COMMIT);
	if(retcode == SQL_SUCCESS_WITH_INFO || retcode ==  SQL_SUCCESS)
	{
		// ����Ϊ�Զ��ύģʽ
		retcode	=	SQLSetConnectAttr (m_hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_ON, SQL_IS_POINTER);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			return	ERROR_DB_TRANS_RET_SUCCESS;
		}
	}
	return	ERROR_DB_TRANS_RET_ERROR;
}

// ִ��Sql���
BOOL			CMySQLConnection::ExecuteSql(const char*  szSql)
{
	// linux
	try
	{
		if (!IsOpen())
		{
			Open();
			// ���ش���
			return FALSE;
		}
		// Sql ��ѯ�����
		SQLHSTMT	hstmt;
		// ����ֵ����
		SQLRETURN	retcode;

		// ����SQL�������ÿ��ִ��SQL��䶼�����������������ִ����ɺ��ͷ�
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hstmt);	
		// �ж�����Sql������Ƿ�ɹ����ɹ���ִ��Sql���
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			// ִ�гɹ�	// ִ��Sql���
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)szSql, SQL_NTS) ;
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO || retcode == SQL_NO_DATA) 
			{
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
				return	TRUE;
			}
			else
			{				
				//----------------------------------------------------------------------------------
				// ���������Ϣ
				INT iErrno = 0;
				ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
				//----------------------------------------------------------------------------------
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
				//MY_ERR_THROW();
				return	FALSE;
			}
		}
		else
		{	
			//----------------------------------------------------------------------------------
			// ���������Ϣ
			INT iErrno = 0;
			ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
			//----------------------------------------------------------------------------------

			// ����Sql ���ִ�о�������ͷ�Sql��ѯ�����
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			//MY_ERR_THROW();
			return FALSE;	
		}	
	}
	catch(...)
	{
		// ���ϴ���
		GetConnectionErrorAndDeal();
	}

	return FALSE;

}

// ���ܲο��ӿ���
BOOL			CMySQLConnection::ExecuteSql(const char*  szSql,const char* szTable,INT64& lRowId)
{
	// linux
	try
	{
		if (!IsOpen())
		{
			Open();
			// ���ش���
			return FALSE;
		}

		// ������ʶ
		BOOL		bSucFlag = FALSE;

		// Sql ��ѯ�����
		SQLHSTMT	hstmt;
		SQLHSTMT	hstmtQuery;
		// ����ֵ����
		SQLRETURN	retcode;

		// ����SQL�������ÿ��ִ��SQL��䶼�����������������ִ����ɺ��ͷ�
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hstmt);	
		// �ж�����Sql������Ƿ�ɹ����ɹ���ִ��Sql���
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{	
			// ִ��Sql���
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)szSql, SQL_NTS) ;
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{			
				// �Ƿ����
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);	

				// ����SQL�������ÿ��ִ��SQL��䶼�����������������ִ����ɺ��ͷ�
				retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hstmtQuery);	
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
				{
					SQLFreeHandle(SQL_HANDLE_STMT,	hstmt);	
					SQLFreeHandle(SQL_HANDLE_STMT,	hstmtQuery);                    
					return FALSE;
				}
				// ��ȡ��Ӱ����еĶ�ӦID
				char	szInsertIdSql[coniSqlStrLen] = {0};
				memset(szInsertIdSql,0x0,coniSqlStrLen);
				// Sql �����÷�
				// SELECT  NVL(MAX(ID),1) AS ID FROM VID_TB_TEST_2
				sprintf(szInsertIdSql,"SELECT  IFNULL(MAX(ID),1) AS  %s  FROM %s",INSERT_ID_FIELED_NAME,szTable);

				// ָ��Ҫʹ�õ��α겢������
				retcode = SQLSetStmtAttr(hstmtQuery, SQL_ATTR_CONCURRENCY,(SQLPOINTER) SQL_CONCUR_ROWVER, 0);
				// ���ù������Ϊ�������,
				retcode = SQLSetStmtAttr(hstmtQuery, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC /* SQL_CURSOR_KEYSET_DRIVEN*/, 0);

				// ִ��sql���
				retcode = SQLExecDirect(hstmtQuery, (SQLCHAR*)szInsertIdSql, SQL_NTS);
				if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) 
				{
					//----------------------------------------------------------------------------------
					// ���������Ϣ 
					INT iErrno = 0;
					ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
					//----------------------------------------------------------------------------------
					SQLFreeHandle(SQL_HANDLE_STMT, hstmtQuery);	
					//MY_ERR_THROW();
					return FALSE;
				}
				else
				{				
					// ��ȡ����
					SQLRETURN	iRetFlag = 0;
					SQLCHAR			szData[FIELD_DATA_LEN] = {0};
					memset(szData,0x0,FIELD_DATA_LEN);
					SQLLEN		iActualDataSize = 0;
					// ��һ��,������1��ʼ
					SQLSMALLINT	iIdCollumn = 1;					
					// ����ƶ�����һ����¼
					SQLFetchScroll(hstmtQuery, SQL_FETCH_FIRST, 0);

					// ��ȡֵ����
					iRetFlag = SQLGetData(hstmtQuery,iIdCollumn,SQL_C_CHAR,szData,FIELD_DATA_LEN,&iActualDataSize);
					// �жϻ�ȡ�����Ƿ�ɹ�
					if (SQL_SUCCEEDED(iRetFlag)) 
					{
						// ��ȡ����ID
						lRowId = atol((char*)szData);
					}
					else
					{
						//----------------------------------------------------------------------------------
						// ���������Ϣ
						INT iErrno = 0;
						ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
						//----------------------------------------------------------------------------------
						// ��ȡ����ʧ��
						SQLFreeHandle(SQL_HANDLE_STMT, hstmtQuery);	
						//MY_ERR_THROW();
						return	FALSE;
					}
					SQLFreeHandle(SQL_HANDLE_STMT, hstmtQuery);	
					return  TRUE;		
				}

			}
			else
			{
				//----------------------------------------------------------------------------------
				// ���������Ϣ
				INT iErrno = 0;
				ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
				//----------------------------------------------------------------------------------

				// ִ��ʧ��
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);	
				//MY_ERR_THROW();
				return FALSE;
			}			
		}
		else
		{	
			//----------------------------------------------------------------------------------
			// ���������Ϣ [11/30/2010 13:57 Modify by Liujs]
			INT iErrno = 0;
			ErrorLogInfo(m_hEnv,m_hDbc,hstmt, &iErrno);
			//----------------------------------------------------------------------------------

			// ����Sql ���ִ�о�������ͷ�Sql��ѯ�����
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			//MY_ERR_THROW();
			return FALSE;	
		}	
	}
	catch(...)
	{
		// ���ϴ���
		GetConnectionErrorAndDeal();
	}
	// �޷�ʵ��
	lRowId = -1;
	return	FALSE;

}

// ��ҳ��ѯ
IRecordSet*		CMySQLConnection::ExecutePageQuery(const char *szSql, const INT32 iStartRow, const INT32 iRowNum)
{
	if (iStartRow < coniZero)
	{
		return NULL;
	}
	if (iRowNum <= coniZero)
	{
		return	NULL;
	}

	// ��������ַ������ϳ�������ַ�����ʽ
	// SELECT ID FROM (SELECT ROWNUM R ,ID FROM TB_DEVINFO EMP WHERE ROWNUM < 200 ) WHERE R>100
	// ���Ӽ������ѯ���ݾͿ�����
	string	strFullSql = "";
	if (GetFullPageQuerySql(szSql,iStartRow,iRowNum,strFullSql))
	{
		// ����һ�����󣬷���β��ʹ��
		CMySQLRecordSet*		pcsMySQLRecordSet = NULL;
		pcsMySQLRecordSet = new CMySQLRecordSet();
		// �������Ӷ���ָ��
		pcsMySQLRecordSet->SetConnection(this);

		// ��ѯ���ݼ�
		if (pcsMySQLRecordSet->QuerySql(strFullSql.c_str())!=NULL)
		{
			return	pcsMySQLRecordSet;
		}
		else
		{
			delete	pcsMySQLRecordSet;
			pcsMySQLRecordSet = NULL;
			return NULL;
		}
	}
	return	NULL;
}

// ʱ���ַ���ת����ʱ�����ӿ�
string	CMySQLConnection::ToTime(const char*	szDateTime)
{
	char	szDt[64] = {0};
	sprintf(szDt, "'%s'", szDateTime);
	return	string(szDt);
}
string	CMySQLConnection::ToDate(const char*	szDateTime)
{
	char	szDt[64] = {0};
	sprintf(szDt, "'%s'", szDateTime);
	return	string(szDt);
}
string	CMySQLConnection::ToDateTime(const char*	szDateTime)
{
	char	szDt[64] = {0};
	sprintf(szDt, "'%s'", szDateTime);
	return	string(szDt);
}

// ���ҹؼ��֣��ɹ���TRUE��ʧ�ܣ�FALSE �����عؼ���ǰ����ַ���
BOOL	CMySQLConnection::Find_keyWord_GetString(const char* szSql,const char* szKeyWord,string& strBefore,string& strAfter)
{
	strBefore	=	"";
	strAfter	=	"";

	// char*ת��Ϊstring
	string	strSql		= string(szSql);
	string	strKeyWord	= string(szKeyWord);

	// ת��Ϊ��д
	//transform(strSql.begin(),strSql.end(),strSql.begin(),ToUpper());
	//transform(strKeyWord.begin(),strKeyWord.end(),strKeyWord.begin(),ToUpper());

	// szKeyWord������szSql�е�λ��
	string::size_type	nFromPos = strSql.find(strKeyWord.c_str(),0);

	// û���ҵ�����FALSE
	if (string::npos == nFromPos)
	{
		return FALSE;
	}

	// ����ҵ�������ȡ����
	try
	{
		// ��ȡstrKeyWord�ĳ���
		string::size_type	nLenKeyWord = strKeyWord.length();

		// ��ȡstrSql�ĳ���
		string::size_type	nLenSql		= strSql.length();

		// ��ȡstrSql�г���strKeyWord֮ǰ���ַ�
		strBefore	= strSql.substr(0,nFromPos);

		// ��ȡstrSql�г���strKeyWord֮����ַ�
		strAfter	= strSql.substr(nFromPos+nLenKeyWord,nLenSql-nFromPos-nLenKeyWord);

		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}

	return	TRUE;
}

// ��ȡ FROMǰ����ַ���
// SELECT ROWNUM R,ID,DEV_ID,DEV_NAME FROM TB_DEVINFO EMP 
// ---> SELECT ROWNUM R,ID,DEV_ID,DEV_NAME 
BOOL		CMySQLConnection::GetBeforeFromSqlText(const char* szSql,string& strBeforeFromSql)
{
	try
	{
		string	strSql  = string(szSql);
		// ���������һ��from������ַ���

		// ת��Ϊ��д
		//transform(strSql.begin(),strSql.end(),strSql.begin(),ToUpper()); 

		// FROM �ַ����ֵ�λ��
		string::size_type	nFromPos = strSql.find(constrFromKeyWord.c_str());

		// û���ҵ�
		if (nFromPos == string::npos)
		{
			return	FALSE;
		}

		// ��ȡ from ��ǰ���ַ���
		strBeforeFromSql  =	strSql.substr(0,nFromPos);

	}
	catch(...)
	{
		return	FALSE;
	}

	return	TRUE;
}


// ��� ROWNUM R , ��SELECT ����
// SELECT ID,DEV_ID,DEV_NAME 
// SELECT ROWNUM R,ID,DEV_ID,DEV_NAME 
BOOL		CMySQLConnection::InsertRowinfo2SqlText(const char* szSql,string& strSql)
{
	try
	{
		// ��������Ϣ���ַ���
		std::ostringstream strInfo;
		strInfo.str("");

		strSql  = string(szSql);
		string	strTemp = "";
		// ���������һ��SELECT ������ַ���

		// ת��Ϊ��д
		//transform(strSql.begin(),strSql.end(),strSql.begin(),ToUpper());

		// FROM �ַ����ֵ�λ��
		string::size_type	nSelectPos = strSql.find(constrSelectKeyword.c_str());

		// û���ҵ�
		if (nSelectPos == string::npos)
		{
			return	FALSE;
		}

		// ��ȡSELECT ������ַ���
		strSql	=	strSql.substr(nSelectPos + 6,strSql.size() - nSelectPos - 6);

		// DISTINCT������szSql�е�λ��
		string::size_type	nDISTINCTPos;
		nDISTINCTPos = strSql.find(constrDISTINCTKeyWord.c_str());

		// û���ҵ�DISTINCT
		if (nDISTINCTPos == string::npos)
		{
			// �ϳ��µ��ַ���
			strInfo << "SELECT ";
			strInfo << " ROWNUM R";
			strInfo << " , ";
			strInfo	<< strSql;
		}
		else
		{
			// ��ȡDISTINCT������ַ���
			strSql	=	strSql.substr(nDISTINCTPos + 8,strSql.size() - nDISTINCTPos - 8);

			// �ϳ��µ��ַ���
			strInfo << "SELECT DISTINCT";
			strInfo << " ROWNUM R";
			strInfo << " , ";
			strInfo	<< strSql;
		}

		// ��ȡ�ַ���
		strSql	=	strInfo.str();

		strInfo.str("");
	}
	catch(...)
	{
		return	FALSE;
	}

	return	TRUE;
}

// �����ַ�������
// ���룺SELECT ID,DEV_ID,DEV_NAME FROM TB_DEVINFO EMP 
// �����SELECT  * FROM (SELECT ROWNUM R,TB_DLP_SELECT.* FROM((SELECT ID,DEV_ID,DEV_NAME FROM TB_DEVINFO EMP  )) TB_DLP_SELECT ) WHERE  ROWNUM  < 51 AND R >= 1
BOOL		CMySQLConnection::GetFullPageQuerySql(const char* szSql,const INT32	iStartRow,const INT32 iRowNum,string& strFullSql)
{
	//�޸�ʵ�ַ�ʽ��ֱ�ӽ��ӿڴ�������SQL���з�װ����,hf�޸���2011.03.23
	string strSql(szSql);
	std::ostringstream	strFullSqlInfo;

	strFullSqlInfo << strSql;
	strFullSqlInfo << " LIMIT    ";
	strFullSqlInfo << iStartRow -1;
	strFullSqlInfo <<",   ";
	strFullSqlInfo <<iRowNum;

	strFullSql	=	strFullSqlInfo.str();

	return	TRUE;
}

// ִ�б�׼SQL���
IRecordSet*		CMySQLConnection::ExecuteQuery(const char*  szSql)
{
	// ����һ�����󣬷���β��ʹ��
	CMySQLRecordSet*		pcsCMySQLRecordSet = NULL;
	pcsCMySQLRecordSet = new CMySQLRecordSet();
	// �������Ӷ���ָ��
	pcsCMySQLRecordSet->SetConnection(this);

	// ��ѯ���ݼ�
	if (pcsCMySQLRecordSet->QuerySql(szSql)!=NULL)
	{
		return	pcsCMySQLRecordSet;
	}
	else
	{
		delete	pcsCMySQLRecordSet;
		pcsCMySQLRecordSet = NULL;
		return NULL;
	}

}

// ����һ���յ����ݼ���Addnew���ݵ�ʱ���õ���
IRecordSet*		CMySQLConnection::GetEmptyRecordSet()
{
	// ����һ�����󣬷���β��ʹ��
	CMySQLRecordSet*		pcsCMySQLRecordSet = NULL;
	pcsCMySQLRecordSet = new CMySQLRecordSet();
	// �������Ӷ���ָ��
	pcsCMySQLRecordSet->SetConnection(this);

	// ��������� ReleaseRecordSet�����ͷ�
	return	pcsCMySQLRecordSet;
}

// �ͷ����Ӷ���ӿڣ���ʹ����Ϻ�����Ӷ��󣬷��ص����ӳ���
BOOL			CMySQLConnection::ReleaseConnection() 
{
	// ��ȡ����
	if (CDBConnectionPool::Instance()!=NULL)
	{
		CDBConnectionPool::Instance()->ReleaseConnection((CConnection*)this);
	}
	return TRUE;

}

//
BOOL	CMySQLConnection::GetConnectionErrorAndDeal()
{
	// linux
	Open();
	return	TRUE;
}