#include "Base_Service_Handler.h"
#include <Ace/OS.h>
#include <Ace/SOCK_SEQPACK_Association.h>
#include "LogicHandler.h"

Base_Service_Handler::Base_Service_Handler()
{
	_waite    = false;
	_copysize = 0;
}

Base_Service_Handler::~Base_Service_Handler()
{
	if (this->handle() != ACE_INVALID_HANDLE)
		ACE_OS::closesocket (this->handle());
}

void Base_Service_Handler::open(ACE_HANDLE h, ACE_Message_Block&) 
{
	this->handle(h);

	// �򿪶�д��
	if (this->_reader.open(*this) != 0 )
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("Base_Service_Handler open")));
		delete this;
		return;
	}
	if (this->_writer.open(*this) != 0 )
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("Base_Service_Handler open")));
		delete this;
		return;
	}

	//��ȡ�ͻ������ӵ�ַ�Ͷ˿�
	size_t addr_size = 1; 
	ACE_SOCK_SEQPACK_Association ass = ACE_SOCK_SEQPACK_Association(h); 	
	ass.get_local_addrs(&_peer_addr,addr_size);

	if(!this->initiate_read_stream())
	{
		delete this;
	}
}

void Base_Service_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)  
{
	// �������յ�����Ϣ�鲢�ͷ�
	ACE_Message_Block &mb = result.message_block();
	if(!result.success() || result.bytes_transferred() == 0)
	{
		mb.release();
		delete this;
		return;
	}

	handler_message(mb);

	mb.release();

	// �����ȴ��ͻ�������
	if(!this->initiate_read_stream())
	{
		delete this;
	}
}

void Base_Service_Handler::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
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

void Base_Service_Handler::addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address)
{
	_peer_addr = remote_address;
}

// ��������
void Base_Service_Handler::send(char* data,ACE_INT32 len)
{
	//
}
// ��ȡ�ͻ��˵�ַ
ACE_INET_Addr& Base_Service_Handler:: get_peer_addr()
{
	return _peer_addr;
}

bool Base_Service_Handler::initiate_read_stream()
{
	// �ȴ��ͻ��˷�������
	if( this->handle_ == ACE_INVALID_HANDLE )
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("Base_Service_Handler handl_ invalid!\n")));
		return false;
	}

	ACE_Message_Block *mb = 0;
	ACE_NEW_NORETURN(mb,ACE_Message_Block(1024,-1));
	if( this->_reader.read(*mb,mb->size()) == -1 )
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("Base_Service_Handler read error\n")));
		mb->release();
		return false;
	}

	return true;
}

void Base_Service_Handler::handler_message(const ACE_Message_Block & msg)
{
	if(!_waite)
	{
		// ������Ϣ���ͽ��д���
		NetPacket* pack = (NetPacket*)msg.rd_ptr();

		pack->head.ntoh();
		_bufszie  = pack->head.len;
		_copysize = msg.length() - sizeof(NetPacketHead);
		_buffer   = new char[_bufszie];
		::memset(_buffer,0,_bufszie);
		::memcpy(_buffer,pack->body,_copysize);
		if( pack->head.len > _copysize )
		{
			_waite   = true;
			return ;
		}
	}
	else
	{
		// ���
		::memcpy(_buffer+_copysize,msg.rd_ptr(),msg.length());
		_copysize = _copysize + msg.length();
		
	}

	if( _bufszie == _copysize )
	{
		/*dlp::MessagePack* message = new dlp::MessagePack;
		if( message->ParseFromArray(_buffer,_bufszie))
		{
			resolve_message(*message);
		}
		else
		{
			ACE_ERROR((LM_ERROR,ACE_TEXT("%p\n"),ACE_TEXT("message pack is wrong!\n")));
		}

		delete message;
		delete _buffer;*/
		_buffer  = 0;
		_bufszie = 0;
		_waite = false;
	}
}
