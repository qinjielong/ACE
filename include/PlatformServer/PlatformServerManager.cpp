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

		// 初始化配置
		if(!initialize_config())
		{
			ACE_ERROR((LM_ERROR,"initialize_config error !\n"));
			return false;
		}

		// 连接数据库
		if( !db_start())
		{
			ACE_ERROR((LM_ERROR,"db_start error !\n"));
			return false;
		}

		// 开启管理线程
		Sgl_PlantManagerTask::instance()->open(); //activate ACE_TASK

		// 开启工作任务处理线程
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
		// 停止工作任务处理线程
		stop_worker_task();
		// 关闭管理线程
		Sgl_PlantManagerTask::instance()->close();
		Sgl_PlantManagerTask::instance()->wait();
		// 关闭数据库
		db_stop();
		// 关闭网络
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

	//-开始侦听
	int PlatformServerManager::open_net()
	{
		std::cout << "open net now " << endl;
		if( -1 == acceptor_.open(addr_) )
		{
			ACE_ERROR((LM_ERROR,"Acceptor open fail!\n"));
			return -1;
		}
		//-只能表明只有一个线程处理从底层收到的网络包，和网络包的收发没有关系。
		//-真正的网络收发是完成端口对应的线程池完成的。
		//-开始等待客户端的连接 
		//-proactor_run_event_loop负责循环处理工作，它内部调用handle_events，进而转到Impl->handle_events
		return ACE_Proactor::instance()->proactor_run_event_loop();
	}

	int PlatformServerManager::close_net()
	{
		//-Acceptor/Handler，要在 end_event_loop之前被清除
		Sgl_ChannelManager::instance()->release_all_channel();
		return ACE_Proactor::instance()->proactor_end_event_loop();
	}

	// 开启业务处理线程(获得CPU数量)
	void PlatformServerManager::start_worker_task()
	{
		// 创建工作者线程,线程数为CPU核数
		int num = 0;

		num = this->get_processor_number();

		for( int i=0; i<num; i++ )
		{
			// 创建业务处理器并注册
			// 注：后续可更改为根据类名动态创建
			TCP_Worker_Task* task = new TCP_Worker_Task;

			ClientAccessHandler*  client_access_handler  = new ClientAccessHandler;
			ServiceAccessHandler* service_access_handler = new ServiceAccessHandler;
			RemoteCmdHandler*     remote_cmd_handler =     new RemoteCmdHandler;

			task->regist_message_handler(PROTOCOL_CLIENT_JION,client_access_handler);
			task->regist_message_handler(PROTOCOL_SERVICE_JION,service_access_handler);

			// 现阶段只处理用户信息更新、策略更新、权限更新、远程卸载命令
			// 其他业务协议不处理，且都交由远程命令处理器处理
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

	// 关闭业务处理线程
	void PlatformServerManager::stop_worker_task()
	{
		// 清除任务 
		Task_Array::iterator task_item = task_array_.begin();
		for( ; task_item != task_array_.end(); ++task_item )
		{
			(*task_item)->close();
			(*task_item)->wait();
			delete *task_item;
		}
		task_array_.clear();

		// 清除处理器
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
