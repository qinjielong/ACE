#include "UDPChannel.h"
#include "ChannelManager.h"
#include <Ace/OS.h>
#include <Ace/SOCK_SEQPACK_Association.h>

#ifdef _DEBUG
	#include <Ace/Date_Time.h>
#endif

namespace DataLock{

	// ACE_MT_SYNCH
	ACE_Message_Queue<ACE_MT_SYNCH> UDPChannel::message_queue;

	UDPChannel::UDPChannel()
	{
		_waite = false;
		_mb    = 0;
		_id	   = 0;
		_flag  = 0;
	}

	UDPChannel::~UDPChannel()
	{
		disconnect();
	}

	int UDPChannel::open(const ACE_INET_Addr &localAddr) 
	{
		// 打开读写
		this->_sockDgram.open(localAddr);
		if (this->_reader.open(*this,_sockDgram.get_handle(),0,ACE_Proactor::instance()) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("UDPChannel open error")));
			delete this;
			return -1;
		}
		if (this->_writer.open(*this,_sockDgram.get_handle(),0,ACE_Proactor::instance()) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("UDPChannel open error")));
			delete this;
			return -1;
		}

		if ( !initiate_read_dgram() )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("UDPChannel open error")));
			delete this;
			return -1;
		}

		// 加入通道管理器
		//_id = Sgl_ChannelManager::instance()->add_channel(this);

		return 0;
	}

	void UDPChannel::handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result)  
	{
		// 解析接收到的消息块并释放
		ACE_Message_Block *mb = result.message_block();
		if(!result.success() || result.bytes_transferred() == 0)
		{
			// 断开时会触发
			mb->release();
			delete this;
			return;
		}

		NetPacket* pack = (NetPacket*)mb->rd_ptr();

		pack->head.ntoh();

		if( 0x01000001 == pack->head.ver ) // 包版本是否正确（做标记使用）
		{
			if ( (pack->head.len + sizeof(NetPacketHead)) == mb->length())
			{
				ACE_Message_Block* new_mb = new ACE_Message_Block(pack->head.len+sizeof(ACE_UINT32));

				// 添加通道ID
				new_mb->copy((char*)(&_id),sizeof(ACE_UINT32));
				new_mb->copy(pack->body,pack->head.len);
				// 将消息放入接收缓存
				UDPChannel::putq_rev(new_mb);
			}
		}

		initiate_read_dgram();
		
		mb->release();
	}

	void UDPChannel::handle_write_dgram(const ACE_Asynch_Write_Dgram::Result &result)
	{
		// 释放发送的消息块
		ACE_Message_Block* mb = result.message_block();
		if(!result.success() || result.bytes_transferred() == 0)
		{
			mb->release();
			delete this;
			return;
		}
		mb->release();

		return ;
	}

	void UDPChannel::addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address)
	{
		_peer_addr = remote_address;
	}


	void UDPChannel::send(ACE_Message_Block& mb)
	{
		size_t length = mb.length();
		if( -1 == _writer.send(&mb,length,0,_peer_addr) )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("UDPChannel send error!\n")));
		}
	}

	void UDPChannel::send(const char* data,ACE_UINT32 length)
	{
		// 计算数据包大小
		int head_size  = sizeof(NetPacketHead);
		int packe_size = head_size + length;

		NetPacketHead head;

		head.ver   = 0x01000000;
		head.seq   = 0x00000000;
		head.count = 0;
		head.num   = 0;
		head.len   = length;
		head.hton();

		ACE_Message_Block *mb = 0;

		ACE_NEW_NORETURN(mb,ACE_Message_Block(packe_size));
		mb->copy((char*)(&head),head_size);
		mb->copy(data,length);

		send(*mb);
	}

	ACE_INET_Addr& UDPChannel::get_peer_addr()
	{
		return _peer_addr;
	}

	// 获取通道ID
	ACE_UINT32	UDPChannel::get_id()
	{
		return _id;
	}


	void UDPChannel::set_flag( ACE_UINT32 flag )
	{
		_flag = flag;
	}

	ACE_UINT32 UDPChannel::get_flag()
	{
		return _flag;
	}

	int UDPChannel::putq_rev(ACE_Message_Block * mb)
	{
		return message_queue.enqueue_tail(mb);
	}

	int UDPChannel::getq_rev(ACE_Message_Block *&mb)
	{
		return message_queue.dequeue_head(mb);
	}	

	bool UDPChannel::initiate_read_dgram()
	{
		// 等待客户端发送数据
		if( this->_sockDgram.get_handle() == ACE_INVALID_HANDLE )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("UDPChannel handl_ invalid!\n")));
			return false;
		}

		ACE_Message_Block* mb = new ACE_Message_Block(4028);

		size_t number_of_bytes_recvd = 0;

		if ( 0 != _reader.recv (mb,number_of_bytes_recvd,0,PF_INET,0) )
		{
			return false;
		}

		return true;
	}

	void UDPChannel::disconnect()
	{
		// 从通道管理器中删除
		if (this->handle() != ACE_INVALID_HANDLE)
		{
			Sgl_ChannelManager::instance()->delet_channel(_id);
			ACE_OS::shutdown(this->handle(),SD_BOTH);
			ACE_OS::closesocket (this->handle());
#ifdef _DEBUG
			ACE_TCHAR ip[25] = {0x00};
			ACE_TCHAR time[21] = {0x00};
			ACE_Date_Time tvTime(ACE_OS::gettimeofday()); 

			sprintf(time,"%d-%d-%d %2d:%2d:%2d",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());
			_peer_addr.addr_to_string(ip,sizeof(ip));
			printf("%s : channel close ip=%s\n",time,ip);
#endif
		}
	}
}	// end namespace
