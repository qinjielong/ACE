
#ifndef DBACCESSDATADEF_DEF_H
#define DBACCESSDATADEF_DEF_H

// ϵͳ��ͷ�ļ�
#include <algorithm>
#include <iostream>
#include "Type.h"

using	namespace std;
/**************************************************************************************************

Description 	: ���ݿ�ģ�����غ꣬��������ģ�飬����ģ��
**************************************************************************************************/
#if 0		//#ifdef _WIN32

// ���� ado �� -----------------------------------------------------------
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#include <icrsint.h>

// ����sprintf �Ⱥ�����VS 2008 ƽ̨�µĸ澯
#pragma warning(disable:4996)

// ODBC��API �ӿڣ�ͷ�ļ�
/*
#include <sql.h> 
#include <sqlext.h>
#include <sqltypes.h>	
#include <odbcinst.h>
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"OdbcCP32.Lib")
*/

#else
// linux    ���ݿ�����ļ�
#include <sql.h>					// ODBC �������ݿⷽʽ
#include <sqlext.h>					// ODBC ��ʽ
#include <sqltypes.h>				// ODBC ���ݿ�����

// ����������ԣ�linuxҪɾ������windowsʱ����
#ifdef _WIN32
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"OdbcCP32.Lib")  //-windows���Դ�
#else
// linux
#endif

#endif

namespace	DBAccessModule
{
	// ���ӳ����������(С)ֵ��С
	const	INT					coniConnectionPoolMaxMum    = 20;
	//-ͨ��ֻ������������
	const	INT					coniConnectionBatchNum		= 2;

	// ����ID��С��ʼ��ֵ
	const	INT					coniDefaultConnectionAutoId = 1; 

	// �������ݳ��ȣ��洢���ݿ��������ַ�����ݿ����ơ��û��������������ĳ���
	const	UINT				coniDataLen					= 256;
	// ��ֵ0
	const	UINT				coniZero					= 0;
	// sql ��䳤�ȣ�
	const	UINT				coniSqlStrLen				= 1024;

	// ����ֵ�����ֶ�����
	#define						INSERT_ID_FIELED_NAME		"ID"


	// ȱʡ��sting�����ַ���
	const	string				constrDefaultString			= "";


#if 0	// #ifdef _WIN32
		
#else

	// ���Ͷ���
	#define						SQLRETURN					SQLLEN						// ����ֵ���Ͷ��� INT

	// SQL ��������
	#define						SQL_ROW_NUMBER_ZORE			0							// û�м�¼
	// �е�����������
	#define						ERROR_SQL_COLUMN_INDEX		-1							// ������ֶ�����

	// һ�����ݵĳ���(�����ݿ��л�ȡ��ʱ���õ�)
	#define 					FIELD_DATA_LEN				8192

	// ���Ƴ���
	#define						MAX_FNAME_LEN				256  

	// �༭ģʽ
	enum EnumEditMode
	{
		EM_UnKnown,				// 0 δ֪
		EM_Edit,				// 1 �༭
		EM_AddNew,				// 2 �¼�
		EM_NoUpdate				// 3 ���ܸ��£�����Ӷ��ű��ѯ���ݵ�ʱ�򣬾ͻ��õ�
	};

	// ���ݿ����ֶε����ƺ������ṹ
	typedef	struct StruColumnItem
	{
		// ������
		string		strColumn;
		// ������
		INT			iColumnIndex;
	}StruColumnItem,*StruColumnItemPtr;

	// �洢�����ƺ������Ľṹ�б�
	typedef		vector<StruColumnItem>						ColumnItemVector;
	typedef		ColumnItemVector::iterator					ColumnItemVectorIterator;

	// ���ұ����ƵĹؼ���  COracleRecordSet �� CSqlServrRecordSet���ݼ����õ�
	const		string			constrFromKeyWord			= "FROM";
	const		string			constrWhereKeyWord			= " WHERE ";
	const		string			constrSelectKeyword			= "SELECT";
	const		string			constrOrderKeyword			= "ORDER";
	const		string			constrGroupKeyWord			= "GROUP";
	const		string			constrBYKeyWord				= "BY";
	const		string			constrDISTINCTKeyWord		= "DISTINCT";


	// ���ݼ��򿪳�ʱʱ��
	const		INT				coniOpenTimeOut				= 3;

	// �α�����,���ڸ��µ�ʱ���ã�����ɾ����ʱ��
	#define		CURRENT_CURSOR_NAME							"CURRENT_CURSOR_NAME"

	// �α����Ƴ���%
	#define		MAX_CURSOR_NAME_LEN							64

	// �α�����ȡֵ��Χ
	const		INT32			coniMaxCursorIndex			= 0x0FFFFFFF;			// �α��������ֵ
	const		INT32			coniMinCursorIndex			= 0x00000001;			// �α�������Сֵ


	/**************************************************************************************************
	Function		: ErrorLogInfo(SQLHENV		hEnv,SQLHDBC	hDbc,SQLHSTMT	hStmt)
	DateTime		: 2010/11/30 13:49	
	Description		: ���������Ϣ�ĺ���
	Input			: hEnv:���ݿ⻷�������hDbc�����ݿ����Ӿ����hStmt�����ݿ��ѯ�����
	Output			: NULL
	Return			: TRUE
	Note			:
	**************************************************************************************************/
	BOOL	ErrorLogInfo(SQLHENV		hEnv,SQLHDBC	hDbc,SQLHSTMT	hStmt, INT *piErrno = NULL);

#endif



}

#endif // DBACCESSDATADEF_DEF_H
