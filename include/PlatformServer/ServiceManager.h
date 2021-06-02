#pragma once

#include <ace/Singleton.h>
#include <string>
#include <list>
#include "Type.h"

using namespace std;

namespace DataLock{

	struct Service 
	{
		string  name;	// ��������
		INT32   id;		// ����ID(����)
		INT32   serial; // ���
		INT32   num;	// ���صĲ�����
		string  ip;		// ����IP
		unsigned short  port; // ����˿ں�
	};

	class ServiceManager
	{
	public:
		ServiceManager(void);
		~ServiceManager(void);

	public:
		// ע�����
		void regist_service(const string & name,INT32 id,const INT32 serial, INT32 number,const string & ip,unsigned short port);
		// ж�ط���
		void unregist_service(const string & name);
		void unregist_service(INT32 id);
		// ���·���������
		void update_service_client_linknumber(INT32 id,INT32 num);
		// ���·�����������IP���˿ں�
		void update_service_netinfor(INT32 id,const string & ip,unsigned short port);

		// ��ȡ���з�������
		INT32 get_service_ancount();
		// ��ȡ������Ϣ
		Service* get_service_infor(INT32 id);

	private:
		std::list<Service> _srvInfroList;	// �����б�
	};

	typedef ACE_Singleton<ServiceManager,ACE_Thread_Mutex>  Sgl_ServiceManager;

}
