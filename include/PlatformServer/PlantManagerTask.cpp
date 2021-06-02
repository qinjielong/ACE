#include "PlantManagerTask.h"
#include "DBAccessManager.h"
#include "ChannelManager.h"
#include "PlatformServerManager.h"
#include <Ace/OS.h>
#include <Xml/Markup.h>
//#include <lexical_cast.h>

#include <iostream>
#include <sstream>

#if (defined(_UNICODE) && defined(_WIN32) )
#include <Helper/StringHelper.h>
#endif

namespace DataLock{

	//UKeyEpass PlantManagerTask::_uKey;

	PlantManagerTask::PlantManagerTask(void)
	{
	}

	PlantManagerTask::~PlantManagerTask(void)
	{
	}

	int PlantManagerTask::open()
	{
		return this->activate();
	}

	void PlantManagerTask::close()
	{
		ACE_Message_Block *pmb = new( ACE_Allocator::instance()->malloc( sizeof(ACE_Message_Block) ) ) 
			ACE_Message_Block(0, ACE_Message_Block::MB_STOP );

		this->putq(pmb);
	}

	//-������̼߳����Ҫִ�еĺ���
	int PlantManagerTask::svc()
	{
		while(true)
		{
			ACE_Message_Block *pmb = NULL;

			ACE_Time_Value tvTmp = ACE_OS::gettimeofday ();
			tvTmp += ACE_Time_Value(20);
			this->getq(pmb,&tvTmp);

			if ( NULL != pmb )
			{
				if( pmb->msg_type() == ACE_Message_Block::MB_STOP )
				{
					printf("msg MB_STOP type.\n");
					pmb->release();
					break;
				}
			}
			
			UpdateUserState();
			UpdateSystemUseTime();

			// ��ͣ20��
		//	ACE_OS::sleep(ACE_Time_Value(0, 20000000));
		}
		return 0;
	}

	void PlantManagerTask::UpdateUserState()
	{
		// ��ʱ20����û���Ϊ����
		char sql[512] = {0x00};

		sprintf(sql,"UPDATE tb_temp_user SET state=0 \
					 WHERE ( TO_SECONDS(NOW()) - TO_SECONDS(login_time) ) > 20\0"
				);

		IConnection* connect = Sgl_DBAccessManager::instance()->get_connection();

		if(connect)
		{
			connect->ExecuteSql(sql);
		}
	}

	void PlantManagerTask::CloseNet()
	{
		Sgl_PlatformServerManager::instance()->close_net();
	}

	void PlantManagerTask::UpdateSystemUseTime()
	{

		// ������Ȩ��Ϣ��ʹ��ʱ��
		string sql = "SELECT NOW() now_time,AES_DECRYPT(usetime,'xsl_datalock2014') use_time, AES_DECRYPT(lic,'xsl_datalock2014') lic \
					  FROM tb_sys_licence \
					  WHERE status=1 AND product_id='DataLock_Platform' \0";

		IConnection* connect = Sgl_DBAccessManager::instance()->get_connection();

		if(!connect)
			return ;

		IRecordSet* record = connect->ExecuteQuery(sql.c_str());

		if(!record)
			return ;

		if( record->Eof() )
			return ;

		string lic;		// ��Ȩ��Ϣ
		string useTime; // ʹ��ʱ��
		string nowTime; // ��ǰʱ��
		string vality;	// ��Ч��

		string name;

		record->GetCollect("lic",lic);
		record->GetCollect("use_time",useTime);
		record->GetCollect("now_time",nowTime);

		CMarkup xml;

#if (defined(_UNICODE) && defined(_WIN32) )
		wstring wlic = DataLock::StringHelper::s2ws(lic);
		if(!xml.SetDoc(wlic.c_str()))
			return ;
#else
		if(!xml.SetDoc(lic.c_str()))
			return ;
#endif
		
		if(!xml.FindElem("SkyCanopy"))
			return ;

		xml.IntoElem();

		if(!xml.FindElem("Licence"))
			return ;

#if (defined(_UNICODE) && defined(_WIN32) )
		wstring wVality;
		wstring wMachineCode;
		wVality     = xml.GetAttrib(_T("Validity"));
		vality      = DataLock::StringHelper::ws2s(wVality);
		wMachineCode = xml.GetAttrib(_T("MachineCode"));
#else
		vality     = xml.GetAttrib("Validity");
#endif

		vality.append(" 00:00:00");

		// ��ʹ��Key��֤

		// ����Key����Key�γ���ֹͣ����
		/*if ( _uKey.Connect() )
		{
			// �������Key���к�
			if ( _uKey.GetSerialNumber().compare(wMachineCode) )
			{
				CloseNet();
			}
		}
		else
		{
			CloseNet();
		}
		*/

		// ���ڻ��ߵ�ǰϵͳʱ��С��ʹ��ʱ�䣬��Ͽ����пͻ�������
		if ( 1 != CompareTime(vality,useTime) || 
			 1 != CompareTime(nowTime,useTime)
			)
		{
			CloseNet();
		}
		else
		{
			// ����ʹ��ʱ��
			string updateSql = "UPDATE tb_sys_licence \
								SET usetime=AES_ENCRYPT(NOW(),'xsl_datalock2014') \
								WHERE status=1 AND product_id='DataLock_Platform'";

			connect->ExecuteSql(updateSql.c_str());
		}
		
		record->ReleaseRecordSet();
		connect->ReleaseConnection();
	}

	int PlantManagerTask::CompareTime( const string & ft,const string & st )
	{
		stringstream fstrm(ft);
		stringstream sstrm(st);

		char dump;
		int fy,fm,fd,fh,fmi,fs;
		int sy,sm,sd,sh,smi,ss;

		fstrm>>fy>>dump>>fm>>dump>>fd>>fh>>dump>>fmi>>dump>>fs;
		sstrm>>sy>>dump>>sm>>dump>>sd>>sh>>dump>>smi>>dump>>ss;

		if(fy < sy)
		{
			return -1;
		}
		else if(fy > sy)
		{
			return 1;
		}

		if(fm < sm)
		{
			return -1;
		}
		else if(fm > sm)
		{
			return 1;
		}

		if(fd < sd)
		{
			return -1;
		}
		else if(fd > sd)
		{
			return 1;
		}

		if(fh < sh)
		{
			return -1;
		}
		else if(fh > sh)
		{
			return 1;
		}

		if(fmi < smi)
		{
			return -1;
		}
		else if(fmi > smi)
		{
			return 1;
		}

		if(fs < ss)
		{
			return -1;
		}
		else if(fs > ss)
		{
			return 1;
		}

		return 0;
	}
}