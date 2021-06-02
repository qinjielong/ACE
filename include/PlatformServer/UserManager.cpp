#include "UserManager.h"
#include "DBAccessManager.h"
//#include <lexical_cast.h>

using namespace DBAccessModule;

namespace DataLock{

	UserManager::UserManager(void)
	{
	}

	UserManager::~UserManager(void)
	{
	}

	void UserManager::UpdateUserInfor( const ClientUserInfor & userInfor )
	{
		char sql[512] = {0x00};

		sprintf(sql,"SELECT * FROM tb_temp_user WHERE code='%s'\0",userInfor.machinecode().c_str());

		IConnection* connection = Sgl_DBAccessManager::instance()->get_connection();

		if( 0 == connection )
		{
			return ;
		}

		//��ü�¼��
		IRecordSet* record_set = connection->ExecuteQuery(sql);

		if ( !record_set )
		{
			return ;
		}

		// ������������¼(��¼��������֤���鵽)(�α��Ƿ񵽼�¼β)
		if ( TRUE == record_set->Eof() )
		{
			char insert_sql[512] = {0x00};

			sprintf(insert_sql,"INSERT tb_temp_user (code,user_id,session,state,part_id,login_time,client_version,ip,status) \
								VALUES ('%s',%d,'%s',%d,%d,NOW(),'%s','%s',1)",
					userInfor.machinecode().c_str(),
					userInfor.userid(),
					userInfor.session().c_str(),
					userInfor.state(),
					userInfor.partid(),
				//	userInfor.longintime().c_str(),
					userInfor.version().c_str(),
					userInfor.ip().c_str()
					);

			connection->ExecuteSql(insert_sql);

			char sql2[128] = {0x00};

			//������һ����
			sprintf(sql2,"SELECT * FROM tb_device WHERE code='%s'\0",userInfor.machinecode().c_str());

			IRecordSet* record_set2 = connection->ExecuteQuery(sql2);

			if ( !record_set2 )
			{
				return ;
			}

			if ( TRUE == record_set2->Eof() )
			{
				char insert_sql2[512] = {0x00};
				sprintf(insert_sql2,"INSERT tb_device (name,code,type, status) \
									 VALUES ('%s','%s',1, 1) ",
						userInfor.machinename().c_str(),
						userInfor.machinecode().c_str()
					);
				connection->ExecuteSql(insert_sql2);
			}

			record_set2->ReleaseRecordSet();
		}
		// ���������
		else
		{
			char update_sql[512] = {0x00};

			sprintf(update_sql,"UPDATE tb_temp_user \
							    SET user_id=%d,session='%s',state=%d,part_id=%d,login_time=NOW(),client_version='%s',ip='%s' \
								WHERE code='%s'" ,
					userInfor.userid(),
					userInfor.session().c_str(),
					userInfor.state(),
					userInfor.partid(),
			//		userInfor.longintime().c_str(),
					userInfor.version().c_str(),
					userInfor.ip().c_str(),
					userInfor.machinecode().c_str()
				    );
			connection->ExecuteSql(update_sql);
		}

		record_set->ReleaseRecordSet();
		connection->ReleaseConnection();
		
	}

	void UserManager::GetUserInfor(string machine_code, ClientUserInfor & userInfor )
	{
		IConnection* connection = Sgl_DBAccessManager::instance()->get_connection();

		if( 0 == connection )
		{
			return ;
		}

		string sql = "SELECT * FROM tb_temp_user WHERE code=";
		//sql.append(lexical_cast<string>(userInfor.userid()));

		IRecordSet* record_set = connection->ExecuteQuery(sql.c_str());

		// ������
		if ( !record_set->Eof() )
		{
			return ;
		}

		int   user_id = 0;
		int   part_id = 0;
		int   state   = 0;
		string session;

		record_set->GetCollect("user_id",user_id);
		record_set->GetCollect("state",state);
		record_set->GetCollect("session",session);
	//	record_set->GetCollect("part_id",part_id);

		userInfor.set_userid(user_id);
		userInfor.set_state(state);
		userInfor.set_session(session);
		
		record_set->ReleaseRecordSet();
	}
}