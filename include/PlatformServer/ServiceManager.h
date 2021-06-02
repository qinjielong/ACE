#pragma once

#include <ace/Singleton.h>
#include <string>
#include <list>
#include "Type.h"

using namespace std;

namespace DataLock{

	struct Service 
	{
		string  name;	// 服务名称
		INT32   id;		// 服务ID(类型)
		INT32   serial; // 序号
		INT32   num;	// 负载的并发数
		string  ip;		// 服务IP
		unsigned short  port; // 服务端口号
	};

	class ServiceManager
	{
	public:
		ServiceManager(void);
		~ServiceManager(void);

	public:
		// 注册服务
		void regist_service(const string & name,INT32 id,const INT32 serial, INT32 number,const string & ip,unsigned short port);
		// 卸载服务
		void unregist_service(const string & name);
		void unregist_service(INT32 id);
		// 更新服务连接数
		void update_service_client_linknumber(INT32 id,INT32 num);
		// 更新服务网络连接IP及端口号
		void update_service_netinfor(INT32 id,const string & ip,unsigned short port);

		// 获取现有服务总数
		INT32 get_service_ancount();
		// 获取服务信息
		Service* get_service_infor(INT32 id);

	private:
		std::list<Service> _srvInfroList;	// 服务列表
	};

	typedef ACE_Singleton<ServiceManager,ACE_Thread_Mutex>  Sgl_ServiceManager;

}
