#pragma once

#include "LogicHandler.h"
#include "ServiceManager.h"
#include <map>
#include <ACE/Thread_Mutex.h>


// �ͻ�����Ϣ������
namespace DataLock{

	class ClientAccessHandler : public ILogicHandler
	{
	public:
		ClientAccessHandler(void);
		virtual ~ClientAccessHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		// <���ͣ�����������>
		typedef std::map<int,string> ServiceInforMap;
	private:
		// ��ѯ��ط�����Ϣ
		void FindServicesToMap(INT32 id,ServiceInforMap & map);
		// ��ȡ����������Ϣ
		void GetServiceToMap(INT32 id,ServiceInforMap & map);

	public:
		//ServiceInforMap _map;	// ������Ϣ��
		static ACE_Thread_Mutex mutex_;
	};
}

