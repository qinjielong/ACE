//=============================================================================
/**
*  @file: Main.cpp
*
*  $Id:   Main.cpp 2014-6-3
*  
*  ƽ̨����������˵��
*  1. ׼����֤����ɿͻ���/http API�������Ľ��봦��
*  2. �������ߣ���ȡ���������ṩ�ķ������ͼ���Ӧ�ķ�����Ϣ��������IP/�˿ڼ����ͱ�ţ�
*  3. ��Ϣͬ����ͬ���û������Ϣ
*  4. Զ��ͨ�������ͻ������͹����Զ��ͨ��
*
*  @author Don kvin geng
*/
//=============================================================================

#include <iostream>
#include "Type.h"
#include "IDBAccessModule.h"
#include "CDBConnectionPool.h"
#include "Ace/ACE.h"
#include "Ace/OS_main.h""
#include "PlantformService.h"
#include "LogService.h"

using namespace std;
using namespace DBAccessModule;
using namespace DataLock;

const wchar_t* serverName = L"DataLock_Plant";  //namespace DataLock

int run_main(int argc,ACE_TCHAR *argv[])
{
	/*DataLock::PlantformService service ;

	service.Init(serverName);

	if ( argc > 1 )
	{
		if(stricmp(argv[1], "/install") == 0)
		{
			service.Install();
		}
		else if (stricmp(argv[1], "/uninstall") == 0)
		{
			service.Uninstall();
		}
		else if ( stricmp(argv[1],"/control") == 0 )
		{
			service.ServiceRun();
			service.ServiceStop();
		}
	}
	else
	{
		service.StartCtrlDispatcher();
	}*/
	
	DataLock::PlantformService PService;
	PService.ServiceRun();

	DataLock::LogService LogService;
	LogService.ServiceRun();

	return 0; 
}


int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here

	CDBConnectionPool *pConnPool = CDBConnectionPool::Instance(); 
	pConnPool->GetConnection("192.168.1.22","db_base","root","xslserver",MYSQL);
	CConnection*	pConnectionObj = NULL;  //������ // �������Ӷ���(CConnection*)pMySQLConnection;
	pConnectionObj = pConnPool->GetOneConnectionFromPool();
	if (NULL == pConnectionObj)
	{
		cout << "get connection from pool fail" << endl;
		return -1;
	}

	BOOL result = pConnectionObj->ExecuteSql("SELECT user_id FROM tb_user_infor ORDER BY user_id");
	if (result)
	{
		cout << "execute succeed" << endl;
	}

	//-����(IRecordSet*)this;
	IRecordSet *pIRecordSet = pConnectionObj->ExecuteQuery("SELECT user_id FROM tb_user_infor ORDER BY user_id");
	if (NULL == pIRecordSet)
	{
		cout << "get recordset fail" << endl;
		return -1;
	}
	INT val;
	pIRecordSet->GetCollect("user_id",val);
	cout << val << endl;

	return run_main(argc,argv);
}

