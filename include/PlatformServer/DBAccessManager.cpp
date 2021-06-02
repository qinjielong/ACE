#include "DBAccessManager.h"

#ifdef _DEBUG
#pragma comment(lib,"DBAccess_d.lib")
#else
#pragma comment(lib,"DBAccess.lib")
#endif

namespace DataLock{

	DBAccessManager::DBAccessManager(void)
		:_connection(0)
	{

	}

	DBAccessManager::~DBAccessManager(void)
	{

	}

	void DBAccessManager::init( string link_name,string db_name,string user_name,string password )
	{
		_link_name = link_name;
		_db_name   = db_name;
		_user_name = user_name;
		_password  = password;
	}

	bool DBAccessManager::start()
	{
		// 连接平台基础数据库 db_base
		// 启动数据库模块
		if(!DBAccessModule::StartDBAccessModule())
		{
			return false;
		}

		// 从连接池中获取连接
		_connectpool = DBAccessModule::GetConnectionPoolInstance();
		if ( 0 == _connectpool)
		{
			return false;
		}

		// 连接数据库获取连接
		_connection = _connectpool->GetConnection(_link_name.c_str(),_db_name.c_str(),_user_name.c_str(),_password.c_str(),DBAccessModule::MYSQL);
		if ( 0 == _connection)
		{
			return false;
		}

		return true;
	}

	bool DBAccessManager::stop()
	{
		if(!DBAccessModule::StopDBAccessModule())
		{
			return false;
		}

		return true;
	}

	IConnection* DBAccessManager::get_connection()
	{
		if ( _connection )
		{
			_connection->ReleaseConnection();
		}
		// 连接数据库获取连接
		_connection = _connectpool->GetConnection(_link_name.c_str(),_db_name.c_str(),_user_name.c_str(),_password.c_str(),DBAccessModule::MYSQL);
		if ( 0 == _connection)
		{
			return false;
		}
		
		return _connection;
	}
}