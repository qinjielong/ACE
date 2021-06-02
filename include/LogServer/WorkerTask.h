//=============================================================================
/**
*  @file: WorkerTask.h
*
*  $Id:   WorkerTask.h 2013-10-13
*  
*  ������ҵ�����߳�
*  1. ��TCP������Ϣ�����л�ȡ��Ϣ����������Ϣ�������Ӧ����Ϣ������
*  2. ע����Ϣ���������ͣ�
*
*  @author Don kvin geng
*/
//=============================================================================

#pragma once

#include <Ace/Task.h>
#include <protocol/MessagePack.pb.h>
#include <map>
#include <LogicHandler.h>

namespace DataLock{

	//-ACE_Task ���൱��һ��ӵ����Ϣ���е��߳�
	//-����������putq()����Ϣ���뵽��һ�������Ϣ�����У���������ͨ��ʹ��getq()����Ϣ��ȡ����
	//- �������������ú󣬸������߳��� svc() ����������
	class UDP_Worker_Task : public ACE_Task<ACE_MT_SYNCH>
	{
	public:
		UDP_Worker_Task(void);
		~UDP_Worker_Task(void);

	public:
		int open();
		void close();

	public:
		// ע����Ϣ������
		bool regist_message_handler(INT32 type,ILogicHandler* handler);
		// ע����Ϣ������
		void unregist_message_handler(ILogicHandler* handler);

	protected:
		// �̺߳���
		virtual int svc();

	private:
		// ·����Ϣ
		void route_message_pack(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		typedef std::map<INT32,ILogicHandler*> Logic_Handler_Map;

		Logic_Handler_Map handler_map_;	// ��Ϣ��������
	};

} // end namespace

