#pragma once

#include <string>
#include <protocol/ClientUser.pb.h>
#include <Ace/Singleton.h>

using namespace std;

namespace DataLock{

	class UserInfor;

	class UserManager
	{
	public:
		UserManager(void);
		~UserManager(void);
	public:
		// ͬ���û���Ϣ
		void UpdateUserInfor(const ClientUserInfor & userInfor);
		// ��ȡ�û���Ϣ
		void GetUserInfor(string machine_code,ClientUserInfor & userInfor);
	};

	typedef ACE_Singleton<UserManager,ACE_Thread_Mutex> Sgl_UserManager;
}