#include "TCPChannel.h"
#include "ChannelManager.h"
#include <Ace/OS.h>
#include <Ace/SOCK_SEQPACK_Association.h>
//-duplicateǳ�������ᶯ̬����һ��ACE_Message_Block������������б����Ŷ����ݿ�����ü���

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

	//-�µ�socket���������
	//-ÿ���ͻ������ӵ��������ͻ���ô˺���(��̬)    
	void TCPChannel::open(ACE_HANDLE h, ACE_Message_Block&) 
	{
		std::cout << "tcp channel open" << endl;
		this->handle(h);

		// �򿪶�д �������
		if (this->_reader.open(*this) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel open")));
			delete this;
			return;
		}
		// ����д��
		if (this->_writer.open(*this) != 0 )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel open")));
			delete this;
			return;
		}

		// ��ȡ�ͻ������ӵ�ַ�Ͷ˿�
		ACE_SOCK_SEQPACK_Association ass = ACE_SOCK_SEQPACK_Association(h); 	
		ass.get_remote_addr(_peer_addr);

		//-������� ����I/O����
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

		// ����ͨ��������
		_id = Sgl_ChannelManager::instance()->add_channel(this);
	}

	//-������������ã��ͱ����������Ѿ������ˣ�
	//�����Ѿ�����̨�ˡ�Proactor����̨MM���ã������������ˣ����ݾ���Result��
	void TCPChannel::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)  
	{
		// �������յ�����Ϣ�鲢�ͷ�(����)
		ACE_Message_Block &mb = result.message_block();
		if(!result.success() || result.bytes_transferred() == 0)
		{
			// �Ͽ�ʱ�ᴥ��(��������)
			mb.release();
			delete this;
			return;
		}

		// ���(�ȴ��¸������) 
		if(_waite)  
		{
			ACE_Message_Block* new_mb = 0;
			
			ACE_NEW_NORETURN(new_mb,ACE_Message_Block(_mb->length() + mb.length()));
			new_mb->copy(_mb->rd_ptr(),_mb->length()); //��'\0'��Ϊ�ַ�������
			new_mb->copy(mb.rd_ptr(),mb.length());
			_mb->release();
			_mb = new_mb;
		}
		else
		{
			ACE_NEW_NORETURN(_mb,ACE_Message_Block(mb.length()));
			_mb->copy(mb.rd_ptr(),mb.length());
		}

		ACE_INT32  size   = 0;	// ʣ�����ݴ�С

		size = _mb->length();

		// ��Ϣ�����ж��NetPacket���ݰ�
		for (;;)
		{
			INT32 length = size - sizeof(NetPacketHead);
			if( 0 <= length ) // �ﵽ��ͷ��С
			{
				NetPacket* pack = (NetPacket*)_mb->rd_ptr();

				pack->head.ntoh();

				if( length >= pack->head.len ) // �ﵽ�����С
				{
					ACE_Message_Block* new_mb = new ACE_Message_Block(pack->head.len+sizeof(ACE_UINT32));

					// ���ͨ��ID
					new_mb->copy((char*)(&_id),sizeof(ACE_UINT32));
					new_mb->copy(pack->body,pack->head.len);

					// ����Ϣ������ջ���
					TCPChannel::putq_rev(new_mb);

					// �ƶ���ָ��
					_mb->rd_ptr(pack->head.len+sizeof(NetPacketHead));

					size = size - sizeof(NetPacketHead) - pack->head.len;

					// ��ʣ������
					if ( 0 == size )
					{
						_waite = false;
						break;
					}
				}
				else // �����С����,�ȴ��¸���Ϣ
				{
					pack->head.hton();
					_waite = true;
					break;
				}
			}
			else // ���ݲ��� 
			{
				_waite = true;
				break;
			}
		}

		if( 0 < size) // ����ʣ�����ݵ���Ϣ����
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
		
		// �����ȴ��ͻ�������
		if(!this->initiate_read_stream())
		{
			delete this;
		}
	}

	void TCPChannel::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
	{
		// �ͷŷ��͵���Ϣ��
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


	//-����д��WS��Proactor����һ��Write�¼��Ϳ����ˣ����ͳɹ���Handle_write_handle�ᱻ����
	void TCPChannel::send(ACE_Message_Block& mb)
	{
		if( -1 == _writer.write(mb,mb.length()) ) //-�����ϵͳ��������
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel send error!\n")));
		}
	}

	/*
	UINT32	  ver;		// �汾��
	UINT32	  seq;		// ��ˮ��
	UINT32    count;	// �ð�����[udpЭ����ʹ��]
	UINT32    num;		// �����[udpЭ����ʹ��]
	UINT32	  len;		// ���峤��
	*/
	//ClientAccessHandler���������send(body,len);
	void TCPChannel::send(const char* data,ACE_UINT32 length)
	{
		// �������ݰ���С
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

		//  ��д��ɺ��ͷ� handle_write_stream���ͷ�
		//	mb->release();
	}

	ACE_INET_Addr& TCPChannel::get_peer_addr()
	{
		return _peer_addr;
	}

	// ��ȡͨ��ID
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

	// �Ͽ�ͨ��
	void TCPChannel::disconnect()
	{
		// ��ͨ����������ɾ��
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

	//ÿ�����queue��enqueue��dequeue�����Ŀ�����������һ����С��������
	int TCPChannel::putq_rev(ACE_Message_Block * mb)
	{
		return message_queue.enqueue_tail(mb);
	}

	int TCPChannel::getq_rev(ACE_Message_Block *&mb)
	{
		return message_queue.dequeue_head(mb);
	}	

	//-��δ����������̨MM��������Ҫ�հ������պ��˽��ң�
	//Ҳ����˵����δ���99%�Ŀ����Ƕ��������ݵģ�ֻ����Proactorע������¼���
	//����ĵȴ�����ȡ������Proactor���������ˣ��ͻص�Handle_Read_Stream����
	bool TCPChannel::initiate_read_stream()
	{
		// �ȴ��ͻ��˷�������
		if( this->handle_ == ACE_INVALID_HANDLE )
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel handl_ invalid!\n")));
			return false;
		}

		ACE_Message_Block *mb = 0;  //����һ����Ϣ�顣�����Ϣ�齫���ڴ��׽������첽�� 

		ACE_NEW_NORETURN(mb,ACE_Message_Block(1024,-1));

		if( this->_reader.read(*mb,mb->size()) == -1 )  //-Ȼ��ʹ�ö���ע��һ������Ϣ�Ϳ�����
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("TCPChannel read error\n")));
			mb->release();
			return false;
		}

		return true;
	}

}	// end namespace
