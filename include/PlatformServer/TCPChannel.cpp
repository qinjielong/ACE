#include "TCPChannel.h"
#include "ChannelManager.h"
#include <Ace/OS.h>
#include <Ace/SOCK_SEQPACK_Association.h>
//-duplicate浅拷贝，会动态分配一个ACE_Message_Block对象，这个对象中保留着对数据块的引用计数

#ifdef _DEBUG
	#include <Ace/Date_Time.h>
#endif

namespace DataLock{

	// ACE_NULL_SYNCH
	ACE_Message_Queue<ACE_MT_SYNCH> TCPChannel::message_queue;

	TCPChannel::TCPChannel()
	{
		_waite = false;
		_mb    = 0;
		_id	   = 0;
		_flag  = 0;
	}

	TCPChannel::~TCPChannel()
	{
		disconnect();
	}

	//-新的socket句柄产生了
	//-每当客户端连接到服务器就会调用此函数(多态)    
	void TCPChannel::open(ACE_HANDLE h, ACE_Message_Block&) 
	{
		std::cout << "tcp channel open" << endl;
		this->handle(h);

		// 打开读写 构造读流
		if (this->_reader.open(*this) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel open")));
			delete this;
			return;
		}
		// 构造写流
		if (this->_writer.open(*this) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel open")));
			delete this;
			return;
		}

		// 获取客户端连接地址和端口
		ACE_SOCK_SEQPACK_Association ass = ACE_SOCK_SEQPACK_Association(h); 	
		ass.get_remote_addr(_peer_addr);

		//-发起读流 发起I/O操作
		if(!this->initiate_read_stream())
		{
			delete this;
			return ;
		}

#ifdef _DEBUG
		ACE_TCHAR ip[25] = {0x00};
		ACE_TCHAR time[21] = {0x00};
		ACE_Date_Time tvTime(ACE_OS::gettimeofday()); 

		sprintf(time,"%d-%d-%d %2d:%2d:%2d",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());
		_peer_addr.addr_to_string(ip,sizeof(ip));

		printf("%s : channel open ip=%s\n",time,ip);
#endif

		// 加入通道管理器
		_id = Sgl_ChannelManager::instance()->add_channel(this);
	}

	//-这个函数被调用，就表明有数据已经读好了，
	//包裹已经在总台了。Proactor比总台MM还好，给你送上门了，数据就在Result里
	void TCPChannel::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)  
	{
		// 解析接收到的消息块并释放(引用)
		ACE_Message_Block &mb = result.message_block();
		if(!result.success() || result.bytes_transferred() == 0)
		{
			// 断开时会触发(析构函数)
			mb.release();
			delete this;
			return;
		}

		// 组包(等待下个包标记) 
		if(_waite)  
		{
			ACE_Message_Block* new_mb = 0;
			
			ACE_NEW_NORETURN(new_mb,ACE_Message_Block(_mb->length() + mb.length()));
			new_mb->copy(_mb->rd_ptr(),_mb->length()); //以'\0'作为字符串结束
			new_mb->copy(mb.rd_ptr(),mb.length());
			_mb->release();
			_mb = new_mb;
		}
		else
		{
			ACE_NEW_NORETURN(_mb,ACE_Message_Block(mb.length()));
			_mb->copy(mb.rd_ptr(),mb.length());
		}

		ACE_INT32  size   = 0;	// 剩余数据大小

		size = _mb->length();

		// 消息块中有多个NetPacket数据包
		for (;;)
		{
			INT32 length = size - sizeof(NetPacketHead);
			if( 0 <= length ) // 达到包头大小
			{
				NetPacket* pack = (NetPacket*)_mb->rd_ptr();

				pack->head.ntoh();

				if( length >= pack->head.len ) // 达到包体大小
				{
					ACE_Message_Block* new_mb = new ACE_Message_Block(pack->head.len+sizeof(ACE_UINT32));

					// 添加通道ID
					new_mb->copy((char*)(&_id),sizeof(ACE_UINT32));
					new_mb->copy(pack->body,pack->head.len);

					// 将消息放入接收缓存
					TCPChannel::putq_rev(new_mb);

					// 移动读指针
					_mb->rd_ptr(pack->head.len+sizeof(NetPacketHead));

					size = size - sizeof(NetPacketHead) - pack->head.len;

					// 无剩余数据
					if ( 0 == size )
					{
						_waite = false;
						break;
					}
				}
				else // 包体大小不足,等待下个消息
				{
					pack->head.hton();
					_waite = true;
					break;
				}
			}
			else // 数据不足 
			{
				_waite = true;
				break;
			}
		}

		if( 0 < size) // 拷贝剩余数据到消息缓存
		{
			ACE_Message_Block *new_mb = 0;
	 
			ACE_NEW_NORETURN(new_mb,ACE_Message_Block(size));
			new_mb->copy(_mb->rd_ptr(),size);
			_mb->release();
			_mb = new_mb;
		}
		else
		{
			_mb->release();
			_mb = 0;
		}

		mb.release();
		
		// 继续等待客户端数据
		if(!this->initiate_read_stream())
		{
			delete this;
		}
	}

	void TCPChannel::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
	{
		// 释放发送的消息块
		ACE_Message_Block &mb = result.message_block();
		if(!result.success() || result.bytes_transferred() == 0)
		{
			mb.release();
			delete this;
			return;
		}

		mb.release();

		return ;
	}

	void TCPChannel::addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address)
	{
		_peer_addr = remote_address;
	}


	//-调用写流WS向Proactor发送一个Write事件就可以了，发送成功后，Handle_write_handle会被调用
	void TCPChannel::send(ACE_Message_Block& mb)
	{
		if( -1 == _writer.write(mb,mb.length()) ) //-向操作系统发送数据
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel send error!\n")));
		}
	}

	/*
	UINT32	  ver;		// 版本号
	UINT32	  seq;		// 流水号
	UINT32    count;	// 该包总数[udp协议中使用]
	UINT32    num;		// 包编号[udp协议中使用]
	UINT32	  len;		// 包体长度
	*/
	//ClientAccessHandler调用这里的send(body,len);
	void TCPChannel::send(const char* data,ACE_UINT32 length)
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
	//	ACE_Message_Block* mb = new ACE_Message_Block(packe_size);
		mb->copy((char*)(&head),head_size);
		mb->copy(data,length);

		send(*mb);

		//  在写完成后释放 handle_write_stream会释放
		//	mb->release();
	}

	ACE_INET_Addr& TCPChannel::get_peer_addr()
	{
		return _peer_addr;
	}

	// 获取通道ID
	ACE_UINT32	TCPChannel::get_id()
	{
		return _id;
	}


	void TCPChannel::set_flag( ACE_UINT32 flag )
	{
		_flag = flag;
	}

	ACE_UINT32 TCPChannel::get_flag()
	{
		return _flag;
	}

	// 断开通道
	void TCPChannel::disconnect()
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

	//每次针对queue的enqueue和dequeue操作的开销都保持在一个很小的量级上
	int TCPChannel::putq_rev(ACE_Message_Block * mb)
	{
		return message_queue.enqueue_tail(mb);
	}

	int TCPChannel::getq_rev(ACE_Message_Block *&mb)
	{
		return message_queue.dequeue_head(mb);
	}	

	//-这段代码就是向总台MM交待：我要收包裹，收好了叫我！
	//也就是说，这段代码99%的可能是读不出数据的，只是向Proactor注册读的事件，
	//具体的等待、读取操作由Proactor读，读到了，就回调Handle_Read_Stream方法
	bool TCPChannel::initiate_read_stream()
	{
		// 等待客户端发送数据
		if( this->handle_ == ACE_INVALID_HANDLE )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel handl_ invalid!\n")));
			return false;
		}

		ACE_Message_Block *mb = 0;  //创建一个消息块。这个消息块将用于从套接字中异步读 

		ACE_NEW_NORETURN(mb,ACE_Message_Block(1024,-1));

		if( this->_reader.read(*mb,mb->size()) == -1 )  //-然后使用读流注册一个读消息就可以了
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel read error\n")));
			mb->release();
			return false;
		}

		return true;
	}

}	// end namespace
