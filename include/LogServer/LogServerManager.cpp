#include "LogServerManager.h"
#include <ProtocolType.h>
#include "DBAccessManager.h"
#include "LogHandler.h"
#include "ChannelManager.h"
#include <Xml/Markup.h>
//#include <lexical_cast.h>

#ifdef _WIN32
#include <Helper/OSHelper.h>
#endif

namespace DataLock{

	LogServerManager::LogServerManager(void)
	{
	}

	LogServerManager::~LogServerManager(void)
	{
	}

	bool LogServerManager::start()
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

		// 开启工作任务处理线程
		start_worker_task();

		int re = open_net() ;
		if ( 1 == re || -1 == re )
		{
			return false;
		}

		return true;
	}

	bool LogServerManager::restart()
	{
		stop();
		return start();
	}

	bool LogServerManager::stop()
	{
		// 停止工作任务处理线程
		stop_worker_task();
		// 关闭数据库
		db_stop();
		// 关闭网络
		if( -1 != close_net() )
			return true;

		return false;
	}

	bool LogServerManager::initialize_config()
	{
		//wstring fileName;
		string fileName;
		port_ = 5001;

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
				//port_ = lexical_cast<unsigned int>(xml.GetAttrib(L"Port"));
				break;
			}
		}
		return true;
	}

	int LogServerManager::open_net()
	{
		// 采用一个UDP通道负责接受发送处理
		UDPChannel* channel = new UDPChannel;

		channel->open(ACE_INET_Addr(port_));

		return ACE_Proactor::instance()->proactor_run_event_loop();
	}

	int LogServerManager::close_net()
	{
		Sgl_ChannelManager::instance()->release_all_channel();
		return ACE_Proactor::instance()->proactor_end_event_loop();
	}

	// 开启业务处理线程
	void LogServerManager::start_worker_task()
	{
		// 创建工作者线程,线程数为CPU核数
		int num = 0;

		num = this->get_processor_number();

		for( int i=0; i<num; i++ )
		{
			// 创建业务处理器并注册
			// 注：后续可更改为根据类名动态创建
			UDP_Worker_Task* task = new UDP_Worker_Task;
			LogHandler* client_log_handler = new LogHandler;

			task->regist_message_handler(PROTOCOL_LOG,client_log_handler);

			task->open();

			handler_array_.push_back(client_log_handler);
			
			task_array_.push_back(task);
		}
	}
	// 关闭业务处理线程
	void LogServerManager::stop_worker_task()
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

	int LogServerManager::get_processor_number()
	{
		int		num = 2;
#ifdef _WIN32
		SYSTEM_INFO info;
		::GetSystemInfo(&info);
		num = info.dwNumberOfProcessors;
#endif
		return	num;
	}

	bool LogServerManager::db_start()
	{
		Sgl_DBAccessManager::instance()->init("192.168.1.22","db_base","root","xslserver");
		return Sgl_DBAccessManager::instance()->start();
	}

	void LogServerManager::db_stop()
	{
		Sgl_DBAccessManager::instance()->stop();
	}
	
} // end namespace dlp

