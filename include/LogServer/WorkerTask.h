//=============================================================================
/**
*  @file: WorkerTask.h
*
*  $Id:   WorkerTask.h 2013-10-13
*  
*  工作者业务处理线程
*  1. 从TCP接收消息缓存中获取消息，并将该消息分配给相应的消息处理器
*  2. 注册消息处理器类型；
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

	//-ACE_Task 就相当于一个拥有消息队列的线程
	//-发送任务用putq()将消息插入到另一任务的消息队列中，接收任务通过使用getq()将消息提取出来
	//- 在主动对象被启用后，各个新线程在 svc() 方法中启动
	class UDP_Worker_Task : public ACE_Task<ACE_MT_SYNCH>
	{
	public:
		UDP_Worker_Task(void);
		~UDP_Worker_Task(void);

	public:
		int open();
		void close();

	public:
		// 注册消息处理器
		bool regist_message_handler(INT32 type,ILogicHandler* handler);
		// 注销消息处理器
		void unregist_message_handler(ILogicHandler* handler);

	protected:
		// 线程函数
		virtual int svc();

	private:
		// 路由消息
		void route_message_pack(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		typedef std::map<INT32,ILogicHandler*> Logic_Handler_Map;

		Logic_Handler_Map handler_map_;	// 消息处理器表
	};

} // end namespace

