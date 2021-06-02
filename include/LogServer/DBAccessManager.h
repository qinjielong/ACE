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
		// ��ʼ������
		void init(string link_name,string db_name,string user_name,string  password);
		// �������ݿ�
		bool start();
		// ֹͣ���ݿ�
		bool stop();
		// ��ȡ���ݿ����Ӷ���
		IConnection* get_connection();

	public:
		IConnection* _connection;	// ���ݿ����Ӷ���
		DBAccessModule::IConnectionPool* _connectpool;
		string		 _link_name;	// ������
		string		 _db_name;		// ���ݿ���
		string		 _user_name;	// �û���
		string		 _password;		// ����
	};

	typedef ACE_Singleton<DBAccessManager,ACE_Thread_Mutex> Sgl_DBAccessManager;

}
