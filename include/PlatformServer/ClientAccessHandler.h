#pragma once

#include "LogicHandler.h"
#include "ServiceManager.h"
#include <map>
#include <ACE/Thread_Mutex.h>


// 客户端消息处理器
namespace DataLock{

	class ClientAccessHandler : public ILogicHandler
	{
	public:
		ClientAccessHandler(void);
		virtual ~ClientAccessHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		// <类型，服务连接名>
		typedef std::map<int,string> ServiceInforMap;
	private:
		// 查询相关服务信息
		void FindServicesToMap(INT32 id,ServiceInforMap & map);
		// 获取单个服务信息
		void GetServiceToMap(INT32 id,ServiceInforMap & map);

	public:
		//ServiceInforMap _map;	// 服务信息表
		static ACE_Thread_Mutex mutex_;
	};
}

