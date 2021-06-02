//=============================================================================
/**
*  @file: Base_Service_Handler.h
*
*  $Id:   Base_Service_Handler.h 2013-10-12
*  
*  基础处理器
*  1. 组包
*  2. 路由客户端/服务发送的接入消息，分配给各自对应的处理器处理；
*  注：处理器和客户端的配置比为1:1，即一个客户端对应一个中心处理器
*  @author Don kvin geng
*/
//=============================================================================
#pragma once

#include "Type.h"
#include <Ace/Message_Queue.h>
#include <Ace/Asynch_IO.h>
#include <Ace/Singleton.h>
#include <Ace/Thread_Mutex.h>
#include "protocol/MessagePack.pb.h"
#include "NetPacket.h"
#include <Ace/INET_Addr.h>

class Base_Service_Handler : public ACE_Service_Handler
{
public:
	Base_Service_Handler();
	virtual ~Base_Service_Handler();
	// 服务端连接事件
	virtual void open(ACE_HANDLE h, ACE_Message_Block&);
	// 此处解网络底层封包
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
	// 此处封装网络底层封包
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
	// 连接地址处理
	virtual void addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address);
	// 发送数据
	void send(char* data,ACE_INT32 len);
	// 获取客户端地址
	ACE_INET_Addr& get_peer_addr();

protected:
	bool initiate_read_stream();
	// 解析消息
	//virtual void resolve_message(const dlp::MessagePack & msg) = 0;
	// 处理消息
	void handler_message(const ACE_Message_Block & msgBlock);

private:
	ACE_Asynch_Read_Stream  _reader;		// 读
	ACE_Asynch_Write_Stream _writer;		// 写
	ACE_INET_Addr			_peer_addr;	    // 连接的地址
	char*					_buffer;		// 包缓冲区
	UINT32					_bufszie;		// 包缓冲区大小
	bool					_waite;			// 等待下个包标记
	UINT32					_copysize;		// 已复制数据大小
};
