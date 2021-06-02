
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

#include <Ace/ACE.h>
#include <Ace/OS_main.h>
#include "LogService.h"

const wchar_t* serverName = L"DataLock_Log";

int run_main(int argc,ACE_TCHAR *argv[])
{
	DataLock::LogService service ;

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
	}
	
	return 0;
}

int ACE_TMAIN(int argc,ACE_TCHAR *argv[])
{
	return run_main(argc,argv);
}

