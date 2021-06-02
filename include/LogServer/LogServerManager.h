#pragma once

#include <Ace/Log_Msg.h>
#include <Ace/Proactor.h>
#include <Ace/Asynch_Acceptor.h>
#include <vector>
#include "UDPChannel.h"
#include "WorkerTask.h"

namespace DataLock{

#define ACE_LOCAL_IP "0.0.0.0"

	// ����������
	class LogServerManager
	{
	public:
		LogServerManager(void);
		~LogServerManager(void);

	public:
		// ����������
		bool start();
		// ����������
		bool restart();
		// �رշ�����
		bool stop();

	private:
		// ��������
		int open_net();
		// �ر�����
		int close_net();
		// ��ʼ������
		bool initialize_config();
		// �������ݿ�
		bool db_start();
		// �ر����ݿ�����
		void db_stop();
		// ����ҵ�����߳�
		void start_worker_task();
		// �ر�ҵ�����߳�
		void stop_worker_task();

	private:
		// ��ȡCPU����
		int  get_processor_number();

	private:
	//	typedef ACE_Asynch_Acceptor<UDPChannel>	 ACCEPTOR;
		typedef std::vector<UDP_Worker_Task*>	 Task_Array;
		typedef std::vector<ILogicHandler*>		 Logic_Handler_Array;

		unsigned int port_;			// ��������ַ
	//	ACCEPTOR	  acceptor_;		// ������

		Task_Array			task_array_;	// ����
		Logic_Handler_Array handler_array_;	// ������
	};

	typedef ACE_Singleton<LogServerManager,ACE_Thread_Mutex> Sgl_LogServerManager;

} // end namespace

