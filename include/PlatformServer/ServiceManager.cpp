#include "ServiceManager.h"

namespace DataLock{

	ServiceManager::ServiceManager(void)
	{
	}

	ServiceManager::~ServiceManager(void)
	{
	}

	void ServiceManager::regist_service(const string & name,INT32 id,const INT32 serial,
		INT32 number,const string & ip,unsigned short port)
	{
		// 查询是否已有同类服务注册
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->name == name && item->id == id && item->serial == serial )
			{
				// 更新信息
				item->ip     = ip;
				item->port   = port;
				item->num    = number;
				return ;
			}
		}

		Service srvInfor;

		srvInfor.name = name;
		srvInfor.id	  = id;
		srvInfor.ip   = ip;
		srvInfor.port = port;
		srvInfor.num  = number;
		srvInfor.serial = serial;

		_srvInfroList.push_back(srvInfor);
	}

	void ServiceManager::unregist_service(const string & name)
	{
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->name == name )
			{
				_srvInfroList.erase(item);
			}
		}
	}

	void ServiceManager::unregist_service(INT32 id)
	{
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->id == id )
			{
				_srvInfroList.erase(item);
			}
		}
	}

	void ServiceManager::update_service_client_linknumber(INT32 id,INT32 num)
	{
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->id == id )
			{
				item->num = num;
			}
		}
	}

	void ServiceManager::update_service_netinfor(INT32 id,const string & ip,unsigned short port)
	{
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->id == id )
			{
				item->ip   = ip;
				item->port = port;
			}
		}
	}

	INT32 ServiceManager::get_service_ancount()
	{
		return _srvInfroList.size();
	}

	Service* ServiceManager::get_service_infor(INT32 id)
	{
		std::list<Service>::iterator item;
		item = _srvInfroList.begin();
		for ( ; item != _srvInfroList.end(); ++item )
		{
			if( item->id == id )
			{
				return &(*item);
			}
		}

		return 0;
	}

}