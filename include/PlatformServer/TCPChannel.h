//=============================================================================
/**
*  @file: TCPChannel.h
*
*  $Id:   TCPChannel.h 2013-10-12
*  
*  TCP处理器
*  1. 组包
*  2. 将完成的数据包放置于消息缓存中，供消息处理线程读取；
*  @author Don kvin geng
*/
//=============================================================================
#pragma once

#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Thread_Mutex.h>
#include "protocol/MessagePack.pb.h"
#include "NetPacket.h"
#include <Ace/INET_Addr.h>
#include <Ace/Null_Mutex.h>
#include <Ace/Null_Condition.h>

namespace DataLock{
	
	//-针对你应用从ACE_Service_Handler派生
	//-ACE_Service_Handle主要就是定义了一些回调函数
	class TCPChannel : public ACE_Service_Handler
	{
	public:
		TCPChannel();
		~TCPChannel();

		//-当连接创建完成后，上面讲的Open方法会被调用，我们看看Open方法中都有些什么代码：
		// 服务端连接事件  //-当有客户端连接上来，连接建立成功后Proactor会调用这个方法
		virtual void open(ACE_HANDLE h, ACE_Message_Block&);
		// 此处解网络底层封包 //-当用户要读的数据读好了后，调用这个方法
		virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
		// 此处封装网络底层封包 //-当用户要写的数据在网卡上发送成功后，Proactor会回调这个方法
		virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
		
		// 连接地址处理
		virtual void addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address);
		// 发送数据
		void send(ACE_Message_Block& mb);
		// 发送数据
		void send(const char* data,ACE_UINT32 length);
		// 获取远程IP地址
		ACE_INET_Addr& get_peer_addr();
		// 获取通道ID
		ACE_UINT32	   get_id();
		// 设置扩展标识
		void set_flag(ACE_UINT32 flag);
		// 获取扩展标识
		ACE_UINT32 get_flag();
		// 断开通道
		void disconnect();

	public:
		// 设置接收消息缓存队列大小
		static void set_rev_message_queue_size(ACE_UINT32 size);
		// 设置发送消息缓存队列大小
		//static void set_send_meesage_queue_size(ACE_UINT32 size);
		
		// 添加消息到公共消息缓存队列
		static int  putq_rev(ACE_Message_Block * mb);
		// 从消息缓存队列中获取消息并从队列中删除原消息
		static int  getq_rev(ACE_Message_Block *&mb);

	private:
		bool initiate_read_stream();
		// 解析消息
	//	void resolve_message(const DataLock::MessagePack & message_pack);
		// 处理消息
	//	void handler_message(const ACE_Message_Block & message_block);

	private:
		static ACE_Message_Queue<ACE_MT_SYNCH> message_queue;	// 接收消息队列缓存
		static ACE_UINT32					   queue_size;	    // 接收消息队列缓存大小
	private:
		ACE_Asynch_Read_Stream  _reader;		// 读
		ACE_Asynch_Write_Stream _writer;		// 写
		ACE_INET_Addr			_peer_addr;	    // 连接的地址
		bool					_waite;			// 等待下个包标记
	//	ACE_UINT32				_copysize;		// 已复制数据大小
		ACE_UINT32				_id;			// 通道ID
		ACE_UINT32				_flag;			// 扩展标识
//		char*					_buffer;		// 数据缓存
//		ACE_UINT32				_size;			// 缓存数据大小
		ACE_Message_Block*      _mb;			// 读数据缓存[组包和拆粘包时使用]

	};
} // end namespace



