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

	// ����������
	class PlatformServerManager
	{
	public:
		PlatformServerManager(void);
		~PlatformServerManager(void);

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
		// ���ÿͻ������������
		void set_clinet_max_number(int number);
		// ��ȡ�ͻ������������
		int get_client_max_number();

		friend class PlantManagerTask;
		friend class ClientAccessHandler;

	private:
		//-template <class T>
		//void Smemory<T>::mput(T x)
		//{��}
		//��ʾ����һ��Smemory�ĳ�Ա������������Ϊmput���β�x��������T�������޷���ֵ
		//T��һ���βΣ�ͬ���͵�ʵ��ֵ���ṩ�����βΣ�ָ����ÿ����ͬ���͵�ֵ������һ������
		//-�������Ǹղ�д���࣬��Ϊ���̳��˻ص��ӿڣ���ʵ�������ѵĴ��룬
		//-ģ����ACE_Asynch_Acceptor���ں��ʵ�ʱ��ص���Щ������
		typedef ACE_Asynch_Acceptor<TCPChannel>	 ACCEPTOR; //-����һ��������
		typedef std::vector<TCP_Worker_Task*>	 Task_Array;
		typedef std::vector<ILogicHandler*>		 Logic_Handler_Array;

		ACE_INET_Addr addr_;			// ��������ַ
		ACCEPTOR	  acceptor_;		// ������

		Task_Array			task_array_;	// ����ϼ�
		Logic_Handler_Array handler_array_;	// �������ϼ�
		int					max_number_;	// �ն������
	};

	//����ģʽ
	typedef ACE_Singleton<PlatformServerManager,ACE_Thread_Mutex> Sgl_PlatformServerManager;

} // end namespace

