#include "LogHandler.h"
#include <protocol/Log.pb.h>
#include <ProtocolType.h>
#include "DBAccessManager.h"

namespace DataLock{

	LogHandler::LogHandler(void)
	{
	}

	LogHandler::~LogHandler(void)
	{
	}

	void LogHandler::handle( const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id )
	{
		// 终端系统日志
		if ( CMD_CLINET_SYS == message_pack.cmd() )
		{
			ClientSysDeal(message_pack);
		}
		// 终端用户日志
		else if (CMD_CLINET_USER == message_pack.cmd())
		{
			UserLogDeal(message_pack);
		}
	}

	void LogHandler::ClientSysDeal( const MessagePack & message_pack )
	{
		DataLock::Log::ClientLog* log = new DataLock::Log::ClientLog;

		//解析消息
		if ( !log->ParseFromArray(message_pack.body().c_str(),message_pack.len()) )
		{
			delete log;
			return ;
		}

		IConnection* connection = Sgl_DBAccessManager::instance()->get_connection();

		char insert_sql[512] = {0x00};

		sprintf(insert_sql,"INSERT tb_log_client_sys (device_code,device_name,type,level,time,content,status) \
						   VALUES ('%s','%s',%d,%d,NOW(),'%s',1)",
						   log->devicecode().c_str(),
						   log->devicename().c_str(),
						   log->type(),
						   log->leve(),
						   log->content().c_str()
						   );
		//插入数据库
		connection->ExecuteSql(insert_sql);

		connection->ReleaseConnection();

		delete log;
	}

	void LogHandler::UserLogDeal( const MessagePack & message_pack )
	{
		DataLock::Log::UserLog* log = new DataLock::Log::UserLog;

		if ( !log->ParseFromArray(message_pack.body().c_str(),message_pack.len()) )
		{
			delete log;
			return ;
		}

		IConnection* connection = Sgl_DBAccessManager::instance()->get_connection();

		char insert_sql[512] = {0x00};

		sprintf(insert_sql,"INSERT tb_log_client_user (user_id,user_name,type,level,time,content,status) \
						   VALUES (%d,'%s',%d,%d,NOW(),'%s',1)",
						   log->userid(),
						   log->username().c_str(),
						   log->type(),
						   log->leve(),
						   log->content().c_str()
						   );

		connection->ExecuteSql(insert_sql);

		connection->ReleaseConnection();

		delete log;
	}
}
