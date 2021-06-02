#pragma once

#include <string.h>
#include "IDBAccessModule.h"
#include <Ace/Singleton.h>

using namespace std;
using namespace DBAccessModule;

namespace DataLock{

	class DBAccessManager
	{
	public:
		DBAccessManager(void);
		~DBAccessManager(void);

	public:
		// 初始化配置
		void init(string link_name,string db_name,string user_name,string  password);
		// 开启数据库
		bool start();
		// 停止数据库
		bool stop();
		// 获取数据库连接对象
		IConnection* get_connection();

	public:
		IConnection* _connection;	// 数据库连接对象
		DBAccessModule::IConnectionPool* _connectpool;
		string		 _link_name;	// 连接名
		string		 _db_name;		// 数据库名
		string		 _user_name;	// 用户名
		string		 _password;		// 密码
	};

	typedef ACE_Singleton<DBAccessManager,ACE_Thread_Mutex> Sgl_DBAccessManager;

}
