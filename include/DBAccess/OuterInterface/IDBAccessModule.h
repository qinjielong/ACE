#ifndef IDBACCESSMODULE_DEF_H
#define IDBACCESSMODULE_DEF_H

/**************************************************************************************************
Description :	���ݿ����ģ��ӿ�
�� ���Ӷ�����	IConnection
�� ���ӳ���		IConnectionPool
�� ���ݼ���		IRecordSet
*************************************************************************************************/

// ϵͳ�ײ�ķ�װͷ�ļ�
#include <Type.h>
#include <Define.h>
#include <Common.h>

using namespace std;

namespace	DBAccessModule
{
	//----------------------------------------------------------------------------------
	#define ERROR_DBACCESS_START							5000							// ���ݿ�������ʿ�ʼλ��

	#define ERROR_DB_CONNECT_ERROR							(ERROR_DBACCESS_START + 1)		// �������ݿⷢ���쳣
	#define	ERROR_DB_CREATE_CONNECTION_ERROR				(ERROR_DBACCESS_START + 2)		// ����ADO���ӳ���
	#define ERROR_DB_WRONG_CONNECTION_STR					(ERROR_DBACCESS_START + 3)		// ���ݿ������ַ�������

	#define	ERROR_DB_TRANS_RET_ERROR						(ERROR_DBACCESS_START + 4)		// �������쳣����
	#define	ERROR_DB_TRANS_RET_SUCCESS						(ERROR_DBACCESS_START + 5)		// ����������

	//----------------------------------------------------------------------------------
	// ���ݿ�����
	enum EnumDatabaseType
	{
		ORACLE = 0,		// oracle 
		MYSQL,			// MySql 
		SQLite,			// SQLite
		SQLSERVER,		// SqlServer
		ACCESS,			// Access
		EXCEL			// Excel���ݿ�
	};

	// ���Ӷ����Ƿ���ʹ��
	enum EnumConnectionUseFlag
	{
		CONNECTION_IDLESSE	= 0,	// ���ӿ���
		CONNECTION_OCCUPY	= 1		// ����ռ��
	};

	/**************************************************************************************************
	����˵�������ݼ�����ӿڣ�ͨ���ö��󣬽������ݼ��Ĳ���

	�����б�
	<1>. Eof()
	<2>. MoveLast()
	<3>. MoveNext()
	<4>. GetCollect(const char* szFieldName,string&	szValue)
	<5>. PutCollect(const char* szFieldName,const string&	szValue)
	<6>. GetCollect(const char* szFieldName,INT&	iValue)
	<7>. PutCollect(const char* szFieldName,const INT&	iValue)
	<8>. GetCollect(const char* szFieldName,void*	   pValue,const INT iBuffSize,INT&	iDataLen)
	<9>. PutCollect(const char* szFieldName,const  void*	   pValue,const INT	iDataLen)
	<10>. PutDtCollect(const char* szFieldName,const INT iYear,const INT iMonth,const INT iDay,const INT iHour,const INT iMinute,const INT iSecond)
	<11>. AddNew()
	<12>. AddNew(const	char*	szTableName) 
	<13>. Update()
	<14>. ReleaseRecordSet()
	��	  ע�� 
	ͨ�������ݼ������Խ������ݿ�ķ��ʲ���[��ӣ�ɾ�����޸�]��
	*************************************************************************************************/
	class IConnection;
	class IRecordSet
	{
	public:
		IRecordSet(void){};

	public:
		virtual ~IRecordSet(void){};
	public:
		/**************************************************************************************************
		Function		: Eof()    
		DateTime		: 
		Description	: ��¼��β��
		Input			: NULL
		Output		: NULL
		Return		: TRUE:��¼��β�� ,FALSE:��β��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL	Eof() = 0;

		/**************************************************************************************************
		Function		: MoveLast()      
		DateTime		: 
		Description	: �������һ����¼
		Input			: NULL
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL	MoveLast() = 0;

		/**************************************************************************************************
		Function		: MoveNext()    
		DateTime		: 
		Description	: ������һ����¼
		Input			: NULL
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL	MoveNext() = 0;

		/**************************************************************************************************
		  Function		: AddNew(IConnection*	pConnection,const	char*	szTableName)
		  DateTime		: 
		  Description	: ��һ�������ϣ�ָ���ı������������
		  Input			: szTableName��������
		  Output		: NULL
		  Return		: TRUE:�ɹ�
		  Note			:
		  <1> AddNew(const	char*	szTableName)
		  <2> PutCollect()
		  <3> Update()
		**************************************************************************************************/
		virtual	BOOL	AddNew(const	char*	szTableName) = 0;

		/**************************************************************************************************
		Function		: AddNew()    
		DateTime		: 
		Description	: ���һ���µļ�¼
		Input			: NULL
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			:	��Ӽ�¼��
		<1> �Ȳ�ѯ���������ܲ���
		<2> AddNew()
		<3> PutCollect()
		<4> Update()
		**************************************************************************************************/
		virtual BOOL    AddNew() = 0;

		/**************************************************************************************************
		Function		: Update()    
		DateTime		: 
		Description	: ���¼�¼
		Input			: NULL
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ���¼�¼����ο�AddNew ����
		<1> PutCollect()
		<2> Update()
		**************************************************************************************************/
		virtual BOOL    Update() = 0;

	public:	
		/**************************************************************************************************
		Function		: GetCollect(const char* szFieldName,string&	strValue)     
		DateTime		: 
		Description	: ���������ƻ�ȡ��Ӧ�ֶε��ַ���ֵ
		Input			: szFieldName ��   ������
		Output		: strValue ��       ���ص��ַ���ֵ
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ��Ӧ����������Ϊ��string,float,long,unsigned long ,double
		**************************************************************************************************/
		virtual	BOOL	GetCollect(const char* szFieldName,string&	strValue) = 0;

		/**************************************************************************************************
		Function		: PutCollect(const char* szFieldName,const char*     szValue)    
		DateTime		: 
		Description	: �������������ö�Ӧ�ֶε��ַ���ֵ
		Input			: szFieldName ��   ������    ,szValue ��       Ҫ���õ�ֵ
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ��Ӧ����������Ϊ��string,float,long,unsigned long ,double
		**************************************************************************************************/
		virtual	BOOL	PutCollect(const char* szFieldName,const char*     szValue) = 0;

		/**************************************************************************************************
		Function		: GetCollect(const char* szFieldName,INT&	iValue)     
		DateTime		: 
		Description	: ���������ƻ�ȡ��Ӧ�ֶε�INTֵ
		Input			: szFieldName ��   ������
		Output		: iValue ��         ���ص���ֵ
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL	GetCollect(const char* szFieldName,INT&	iValue) = 0;

		/**************************************************************************************************
		Function		: PutCollect(const char* szFieldName,const INT	iValue)     
		DateTime		: 
		Description	: �������������ö�Ӧ�ֶε�INTֵ
		Input			: iValue ��         ���õ���ֵ
		szFieldName ��   ������
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL	PutCollect(const char* szFieldName,const INT	iValue) = 0;

		/**************************************************************************************************
		Function		: GetCollect(const char* szFieldName,void*	   pValue,const INT iBuffSize,INT&	iDataLen)    
		DateTime		: 	
		Description	: �������������ö�Ӧ�ֶε�void*ֵ
		Input			: szFieldName ��   ������
		iBuffSize:	   �����ȡֵ�Ļ����С
		pValue:	������÷���õ��ڴ��ַ
		Output		: pValue ��         ��ȡ������
		iDataLen:			��ȡ�����ݳ��ȣ��ֽڳ���
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ���ڲ��������ƴ�����ʱ��
		**************************************************************************************************/
		virtual	BOOL	GetCollect(const char* szFieldName,void*	   pValue,const INT iBuffSize,INT&	iDataLen) = 0;

		/**************************************************************************************************
		Function		: PutCollect(const char* szFieldName,const  void*	   pValue,const INT	iDataLen)   
		DateTime		: 	
		Description	: �������������ö�Ӧ�ֶε�void*ֵ
		Input			: szFieldName ��   ������
		iDataLen:			���õ����ݳ���	
		pValue:����
		Output		: NULL
		Return		: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ���ڲ��������ƴ�����ʱ��
		**************************************************************************************************/
		virtual	BOOL	PutCollect(const char* szFieldName,const  void*	   pValue,const INT	iDataLen) = 0;

		/**************************************************************************************************
		  Function		: PutDtCollect(const char* szFieldName,const char* szValue)
		  DateTime		: 	
		  Description	: �������������ö�Ӧʱ���ֶε�ֵ
		  Input			: szFieldName��ʱ�������ƣ�szValue��ʱ��ֵ��Ҫ���ʽ����[YYYY-MM-DD HH:mm:SS]�����磺2010-11-27 12:12:12
		  Output		: NULL
		  Return		: NULL
		  Note			:
		**************************************************************************************************/
		virtual	BOOL	PutDtCollect(const char* szFieldName,const INT iYear,const INT iMonth,const INT iDay,const INT iHour,const INT iMinute,const INT iSecond) = 0;

		/**************************************************************************************************
		Function		: ReleaseRecordSet    
		DateTime		: 	
		Description	: �ڲ��ͷ����ݼ�����
		Input			: NULL
		Output		: NULL
		Return		: TRUE:�ɹ���FALSE:ʧ��
		Note			: ʹ�����IRecordSet����󣬵���
		���øú������������Ͳ������ˣ������ڽ��е��ö������ط�����
		ֻ�ǽӿڶ������ôʹ�ã������Ƽ�ʹ��delete this
		���øú����󣬽�ָ�븳ֵΪ �� NULL
		**************************************************************************************************/
		virtual	BOOL	ReleaseRecordSet() = 0;

		/*
		 *********************************************
		 Function : GetColumnNumber
		 DateTime : 
		 Description :  ���ؼ�¼��������
		 Input :  
		 Output : 
		 Return : ������-1
		 Note :   
		 *********************************************
		 */
		virtual INT GetColumnNumber(void) = 0;

		/*
		 *********************************************
		 Function : GetCloumnName
		 DateTime : 
		 Description :  ��ȡ����
		 Input :  iColIndex ������ �� 0 ��ʼ
		 Output : oStrName ���ص�����
		 Return : TRUE/FALSE
		 Note :   
		 *********************************************
		 */
		virtual BOOL GetCloumnName(INT iColIndex, std::string &oStrName ) = 0;

	};


	/**************************************************************************************************
	����˵�����ṩ��ȡ���ݿ����Ӷ���Ľӿ�
	�����б�
	<1>. ExecuteSql( const char*  szSql)
	<2>. ExecuteSql(const char*  szSql,INT64& lRowId)
	<3>. ExecuteQuery(const char*  szSql)
	<4>. ReleaseConnection()
	<5>. BeginTrans()
	<6>. RollbackTrans()
	<7>. CommitTrans()
	<8>. ExecutePageQuery(const char*  szSql,const INT32	iStartRow,const INT32 iRowNum)
	<9>. GetEmptyRecordSet()
	<10>.ToTime(const char*	szDateTime)
	<11>.ToDate(const char*	szDateTime)
	<12>.ToDateTime(const char*	szDateTime)
	��	  ע�� ֵ�ṩһ��OuterInterface���ṩִ����ز����ķ���

	<ע��>. ���ڸ������ݿ�ӿڵĲ�ͬ����������sql��������ѭ����ԭ��
	<1>. �����׼SQL ����ʱ��Ҫ��ȫ����д������ע�ⲻ��ʹ�� * ,���� SELECT * FROM TABLE
	<2>. SQL ����в��ܴ��ڹؼ����ֶΣ����� SELECT USER_ID AS ID FROM TABLE��
	<3>. ���ݿ���Ƶ��ǣ��������ʹ�ùؼ��֣�������ORACLE���ݿ⡣
	<4>. ���ݿ�������б����ƣ��ֶ����ƣ������д

	*************************************************************************************************/
	class IConnection
	{
	protected:
		IConnection(void){};
		virtual ~IConnection(void){};

	public:
		/**************************************************************************************************
		  Function		: GetEmptyRecordSet()
		  DateTime		: 
		  Description	: �����Ӷ����ȡһ���յ����ݼ�
		  Input			: NULL
		  Output		: NULL
		  Return		: NULL
		  Note			: �������ݵ�ʱ����
		**************************************************************************************************/
		virtual	IRecordSet*		GetEmptyRecordSet()	= 0;

		/**************************************************************************************************
		Function		: ReleaseConnection()    
		DateTime		: 
		Description		: �ͷ����Ӷ���ӿڣ���ʹ����Ϻ�����Ӷ��󣬷Żص����ӳ���
		Input			: NULL
		Output			: NULL
		Return			: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: NULL
		**************************************************************************************************/
		virtual	BOOL			ReleaseConnection() = 0 ;

		/**************************************************************************************************
		Function		: ExecuteSql(const char*  szSql)     
		DateTime		: 
		Description		: ִ��Sql���
		Input			: strSql��            ��׼��sql���
		Output			: TRUE
		Return			: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ͨ����׼sql��䣬ִ��sql����[insert,update,delete]
		**************************************************************************************************/
		virtual	BOOL			ExecuteSql(const char*  szSql) = 0;

		/**************************************************************************************************
		Function		: ExecuteSql(const char*  szSql,const char* szTable,INT64& lRowId)  
		DateTime		: 
		Description		: ִ��Sql���
		Input			: strSql��            ��׼��sql���
		: szTable:			  �����ı���
		Output			: lRowId:             �����������Ӧ��Ӱ��������ID
		Return			: TRUE:�����ɹ���FALSE:����ʧ��
		Note			: ͨ����׼sql��䣬ִ��sql����[insert]
		**************************************************************************************************/
		virtual	BOOL			ExecuteSql(const char*  szSql,const char* szTable,INT64& lRowId) = 0;

		/**************************************************************************************************
		Function		: ExecuteQuery(const char*  szSql)    
		DateTime		: 
		Description		: ͨ�����Ӷ����sql��ѯ��䣬�������ݼ�����
		Input			: strSql��            ��׼��sql���
		Output			: NULL
		Return			: IRecordSet* ����ָ�룬������Ӧ�Ĳ�����
		Note			: ͨ����׼sql��䣬ִ��sql����[select]
		**************************************************************************************************/
		virtual	IRecordSet*		ExecuteQuery(const char*  szSql) = 0;


		/**************************************************************************************************
		Function		: ExecutePageQuery(const char*  szSql,const INT32	iStartRow,const INT32 iRowNum)
		DateTime		: 
		Description		: ͨ�����Ӷ����sql��ѯ��䣬�������ݼ�����
		Input			: strSql��            ��׼��sql���,SQL SERVER ֻ֧�ֵ�����ߵ�����ͼ�����ORACLE֧�ֶ�������ͼ����
						  ��Ҫ��ҳ��ѯ����ö����Ƚ�������ͼ����ͳһ��ѯ
						  iStartRow:		  ��ʼ��
						  iRowNum��			  ����Ŀ
		Output			: NULL
		Return			: IRecordSet* ����ָ�룬������Ӧ�Ĳ�����
		Note			: ͨ����׼sql��䣬ִ��sql����[select]
		**************************************************************************************************/
		virtual	IRecordSet*		ExecutePageQuery(const char*  szSql,const INT32	iStartRow,const INT32 iRowNum) = 0;	

		/**************************************************************************************************
		Function		: BeginTrans()    
		DateTime		: 
		Description		: ��ʼִ������
		Input			: NULL
		Output			: NULL
		Return			: TRUE:�ɹ���FALSE:ʧ��
		Note			:			
		**************************************************************************************************/
		virtual	UINT			BeginTrans() = 0;

		/**************************************************************************************************
		Function		: RollbackTrans()    
		DateTime		: 
		Description		: ����ع�����
		Input			: NULL
		Output			: NULL
		Return			: TRUE:�ɹ� �� FALSE:ʧ��
		Note			:
		**************************************************************************************************/
		virtual	BOOL			RollbackTrans() = 0;

		/**************************************************************************************************
		Function		: CommitTrans()    
		DateTime		: 
		Description		: �ύ����
		Input			: NULL
		Output			: NULL
		Return			: TRUE:�ɹ���FALSE:ʧ��
		Note			: 
		**************************************************************************************************/
		virtual	BOOL			CommitTrans() = 0;

		/**************************************************************************************************
		  Function		: ToTime,ToDate,ToDateTime
		  DateTime		: 
		  Description	: ���ַ���ת���ɱ�׼�����ݿ��ʱ���ַ���
		  Input			: NULL
							ToTime:HH:MI:SS
						    ToDate:YYYY-MM-DD
							ToDateTime:YYYY-MM-DD HH:MI:SS
		  Output		: 
							ToTime:HH:MI:SS
							ToDate:YYYY-MM-DD
							ToDateTime:YYYY-MM-DD HH:MI:SS
		  Return		: NULL
		  Note			: ��ͬ�����ݿ⣬��һ�����÷������磺sql server ����ת����ORACLE��Ҫת������
						  ע�⣺�ⲿʹ�ã����������[']��[']
		**************************************************************************************************/
		virtual	string			ToTime(const char*	szDateTime) = 0;
		virtual	string			ToDate(const char*	szDateTime) = 0;
		virtual	string			ToDateTime(const char*	szDateTime) = 0;
	};


	/**************************************************************************************************
	����˵�������ݿ����Ӷ����
	�����б�
	<1>. GetConnectionHandle(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)

	��	  ע�� 
	���ݿ����Ӷ����
	*************************************************************************************************/
	class IConnectionPool 
	{
	protected:
		IConnectionPool(void){};
	public:
		virtual ~IConnectionPool(void){};

	public:
		/**************************************************************************************************
		Function		: GetConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType)    
		DateTime		: 
		Description		: �������Ӷ���ӿ�,�����ӳ���ȡ��һ���Ѵ������ӣ��������ʹ�÷���NULL��
		Input			:     
		szServer		: ���ݿ��������ַ
		szDatabase		: ���ݿ�����(ODBC����)
		szUser			: ���ݿ��û���
		szPass			: ���ݿ�����
		iDbType			: ���ݿ�����[MySql��SQLite ��]
		Output			: NULL
		Return			: IConnection* ���Ӷ���ӿ�ָ��
		Note			: NULL
		**************************************************************************************************/
		virtual	IConnection*	GetConnection(const	char*   szServer,const	char*   szDatabase,const	char*   szUser,const	char*   szPass,const INT iDbType) = 0 ;

	};


	/**************************************************************************************************
	Function			: StartDBAccessModule    
	DateTime			: 
	Description			: �������ݿ�ģ��
	Input				: NULL
	Output				: NULL
	Return				: TRUE:�ɹ�  FALSE:ʧ��
	Note				: �ṩ�������ݿ�ģ����񣬸ú������ṩ�ⲿ������ʹ��
	��		  ֻ�ṩ�ײ���������������ӿ�
			  *************************************************************************************************/
	extern	BOOL	StartDBAccessModule();

	/**************************************************************************************************
	Function			: GetConnectionPoolInstance
	DateTime			: 
	Description			: ��ȡ���ӳض���
	Input				: NULL
	Output				: NULL
	Return				: IConnectionPool*ָ�����
	Note				: ���ص����ӳض������㵥��ʵ��
	ǰ��������StartDBAccessModule�ɹ����ײ��ǵ�������StartDBAccessModule �Ĺ���һ����
	*************************************************************************************************/
	extern 	IConnectionPool*	GetConnectionPoolInstance();


	/**************************************************************************************************
	Function			: StopDBAccessModule    
	DateTime			: 
	Description			: ֹͣ���ݿ�ģ����񣬲��ڲ��ͷ����е���Դ
	Input				: NULL
	Output				: NULL
	Return				: TRUE : �ɹ���FALSE:ʧ��
	Note				: �ú������ṩ�ⲿ������ʹ�ã�ֻ�ṩ�ײ�����˳����˳��ӿ�
	*************************************************************************************************/
	extern	BOOL	StopDBAccessModule();
}

#endif // IDBACCESSMODULE_DEF_H

