#include "PlatformServerManager.h"
#include "ClientAccessHandler.h"
#include "ServiceAccessHandler.h"
#include "RemoteCmdHandler.h"
#include "ChannelManager.h"
#include "ProtocolType.h"
#include "DBAccessManager.h"
#include "Xml/Markup.h"
//#include <lexical_cast.h>


#ifdef _WIN32
#include <Helper/OSHelper.h>
#endif

namespace DataLock{

	PlatformServerManager::PlatformServerManager(void)
	{
		max_number_ = 0;
	}

	PlatformServerManager::~PlatformServerManager(void)
	{
	}

	bool PlatformServerManager::start()
	{

		ACE_DEBUG((LM_DEBUG,"Server running...\n"));

		// ��ʼ������
		if(!initialize_config())
		{
			ACE_ERROR((LM_ERROR,"initialize_config error !\n"));
			return false;
		}

		// �������ݿ�
		if( !db_start())
		{
			ACE_ERROR((LM_ERROR,"db_start error !\n"));
			return false;
		}

		// ���������߳�
		Sgl_PlantManagerTask::instance()->open(); //activate ACE_TASK

		// ���������������߳�
		start_worker_task();

		int re = open_net();
		if ( 1 == re || -1 == re )
		{
			return false;
		}

		return true;
	}

	bool PlatformServerManager::restart()
	{
		stop();
		return start();
	}

	bool PlatformServerManager::stop()
	{
		// ֹͣ�����������߳�
		stop_worker_task();
		// �رչ����߳�
		Sgl_PlantManagerTask::instance()->close();
		Sgl_PlantManagerTask::instance()->wait();
		// �ر����ݿ�
		db_stop();
		// �ر�����
		if( -1 != close_net() )
			return true;

		return false;
	}

	bool PlatformServerManager::initialize_config()
	{
		//wstring fileName;
		string fileName;
		unsigned int port = 5000;

#ifdef _WIN32
		fileName = OSHelper::GetInstancePath() + L"config\\skycanopy.xml";
#endif
		CMarkup xml;

		xml.Load(fileName);

		xml.FindElem("Config");
		xml.IntoElem();
		xml.FindElem("ServiceConfig");
		xml.IntoElem();
		
		while ( xml.FindElem("Service") )
		{
			if ("DataLock_Plant" == xml.GetAttrib("Name") )
			{
				//port = lexical_cast<unsigned int>(xml.GetAttrib(L"Port"));
				break;
			}
		}

		addr_.set(port,ACE_LOCAL_IP);
		return true;
	}

	//-��ʼ����
	int PlatformServerManager::open_net()
	{
		std::cout << "open net now " << endl;
		if( -1 == acceptor_.open(addr_) )
		{
			ACE_ERROR((LM_ERROR,"Acceptor open fail!\n"));
			return -1;
		}
		//-ֻ�ܱ���ֻ��һ���̴߳���ӵײ��յ��������������������շ�û�й�ϵ��
		//-�����������շ�����ɶ˿ڶ�Ӧ���̳߳���ɵġ�
		//-��ʼ�ȴ��ͻ��˵����� 
		//-proactor_run_event_loop����ѭ�������������ڲ�����handle_events������ת��Impl->handle_events
		return ACE_Proactor::instance()->proactor_run_event_loop();
	}

	int PlatformServerManager::close_net()
	{
		//-Acceptor/Handler��Ҫ�� end_event_loop֮ǰ�����
		Sgl_ChannelManager::instance()->release_all_channel();
		return ACE_Proactor::instance()->proactor_end_event_loop();
	}

	// ����ҵ�����߳�(���CPU����)
	void PlatformServerManager::start_worker_task()
	{
		// �����������߳�,�߳���ΪCPU����
		int num = 0;

		num = this->get_processor_number();

		for( int i=0; i<num; i++ )
		{
			// ����ҵ��������ע��
			// ע�������ɸ���Ϊ����������̬����
			TCP_Worker_Task* task = new TCP_Worker_Task;

			ClientAccessHandler*  client_access_handler  = new ClientAccessHandler;
			ServiceAccessHandler* service_access_handler = new ServiceAccessHandler;
			RemoteCmdHandler*     remote_cmd_handler =     new RemoteCmdHandler;

			task->regist_message_handler(PROTOCOL_CLIENT_JION,client_access_handler);
			task->regist_message_handler(PROTOCOL_SERVICE_JION,service_access_handler);

			// �ֽ׶�ֻ�����û���Ϣ���¡����Ը��¡�Ȩ�޸��¡�Զ��ж������
			// ����ҵ��Э�鲻�����Ҷ�����Զ�������������
			task->regist_message_handler(PROTOCOL_CLIENT_USER,remote_cmd_handler);
			task->regist_message_handler(PROTOCOL_POLICY,remote_cmd_handler);
			task->regist_message_handler(PROTOCOL_AUTHORITY,remote_cmd_handler);
			task->regist_message_handler(PROTOCOL_CLIENT_REMOTE_CMD,remote_cmd_handler);

			task->open();

			handler_array_.push_back(client_access_handler);
			handler_array_.push_back(service_access_handler);
			
			task_array_.push_back(task);
		}
	}

	// �ر�ҵ�����߳�
	void PlatformServerManager::stop_worker_task()
	{
		// ������� 
		Task_Array::iterator task_item = task_array_.begin();
		for( ; task_item != task_array_.end(); ++task_item )
		{
			(*task_item)->close();
			(*task_item)->wait();
			delete *task_item;
		}
		task_array_.clear();

		// ���������
		Logic_Handler_Array::iterator handler_item = handler_array_.begin();
		for( ; handler_item != handler_array_.end(); ++handler_item )
		{
			delete *handler_item;
		}
		handler_array_.clear();
	}

	int PlatformServerManager::get_processor_number()
	{
		int		num = 2;
#ifdef _WIN32
		SYSTEM_INFO info;
		::GetSystemInfo(&info);
		num = info.dwNumberOfProcessors;
#endif
		return	num;
	}

	bool PlatformServerManager::db_start()
	{
		Sgl_DBAccessManager::instance()->init("192.168.1.22","db_base","root","xslserver");
		return Sgl_DBAccessManager::instance()->start();
	}

	void PlatformServerManager::db_stop()
	{
		Sgl_DBAccessManager::instance()->stop();
	}

	void PlatformServerManager::set_clinet_max_number(int number)
	{
		max_number_ = number;
	}

	int PlatformServerManager::get_client_max_number()
	{
		return max_number_;
	}
} // end namespace dlp
