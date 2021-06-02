#pragma once

#include <Ace/Log_Msg.h>
#include <Ace/Proactor.h>
#include <Ace/Asynch_Acceptor.h>
#include <vector>
#include "UDPChannel.h"
#include "WorkerTask.h"

namespace DataLock{

#define ACE_LOCAL_IP "0.0.0.0"

	// 服务器管理
	class LogServerManager
	{
	public:
		LogServerManager(void);
		~LogServerManager(void);

	public:
		// 开启服务器
		bool start();
		// 重启服务器
		bool restart();
		// 关闭服务器
		bool stop();

	private:
		// 开启网络
		int open_net();
		// 关闭网络
		int close_net();
		// 初始化配置
		bool initialize_config();
		// 连接数据库
		bool db_start();
		// 关闭数据库连接
		void db_stop();
		// 开启业务处理线程
		void start_worker_task();
		// 关闭业务处理线程
		void stop_worker_task();

	private:
		// 获取CPU核数
		int  get_processor_number();

	private:
	//	typedef ACE_Asynch_Acceptor<UDPChannel>	 ACCEPTOR;
		typedef std::vector<UDP_Worker_Task*>	 Task_Array;
		typedef std::vector<ILogicHandler*>		 Logic_Handler_Array;

		unsigned int port_;			// 服务器地址
	//	ACCEPTOR	  acceptor_;		// 接收器

		Task_Array			task_array_;	// 任务
		Logic_Handler_Array handler_array_;	// 处理器
	};

	typedef ACE_Singleton<LogServerManager,ACE_Thread_Mutex> Sgl_LogServerManager;

} // end namespace

