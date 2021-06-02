#pragma once

#include "Ace/Log_Msg.h"
#include "Ace/Proactor.h"
#include "ace/init_ACE.h"
#include "Ace/Asynch_Acceptor.h"
#include <vector>
#include "TCPChannel.h"
#include "TCPWorkerTask.h"
#include "PlantManagerTask.h"
#include "ClientAccessHandler.h"

namespace DataLock{

#define ACE_LOCAL_IP "0.0.0.0"

	// 服务器管理
	class PlatformServerManager
	{
	public:
		PlatformServerManager(void);
		~PlatformServerManager(void);

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
		// 设置客户端最大连接数
		void set_clinet_max_number(int number);
		// 获取客户端最大连接数
		int get_client_max_number();

		friend class PlantManagerTask;
		friend class ClientAccessHandler;

	private:
		//-template <class T>
		//void Smemory<T>::mput(T x)
		//{…}
		//表示定义一个Smemory的成员函数，函数名为mput，形参x的类型是T，函数无返回值
		//T是一个形参，同类型的实参值被提供给该形参，指定的每个不同类型的值都创建一个新类
		//-就是我们刚才写的类，因为他继承了回调接口，并实现了自已的代码，
		//-模板中ACE_Asynch_Acceptor会在合适的时候回调这些方法。
		typedef ACE_Asynch_Acceptor<TCPChannel>	 ACCEPTOR; //-创建一个监听器
		typedef std::vector<TCP_Worker_Task*>	 Task_Array;
		typedef std::vector<ILogicHandler*>		 Logic_Handler_Array;

		ACE_INET_Addr addr_;			// 服务器地址
		ACCEPTOR	  acceptor_;		// 接收器

		Task_Array			task_array_;	// 任务合集
		Logic_Handler_Array handler_array_;	// 处理器合集
		int					max_number_;	// 终端最大数
	};

	//单例模式
	typedef ACE_Singleton<PlatformServerManager,ACE_Thread_Mutex> Sgl_PlatformServerManager;

} // end namespace

