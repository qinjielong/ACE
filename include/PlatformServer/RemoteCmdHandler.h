#pragma once

#include "LogicHandler.h"
#include <vector>
#include "Type.h"

// �ͻ�����Ϣ������
// Զ���������
namespace DataLock{

	class RemoteCmdHandler  : public ILogicHandler
	{
	public:
		RemoteCmdHandler(void);
		~RemoteCmdHandler(void);
	public:
		virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id);

	private:
		typedef std::vector<std::string> UserIDArray;
	private:
		// �����û�ID
		void ResolveUserIDs(std::string userIDs,UserIDArray & array);
		// ͨ����Ϣ����
		void NotifyMessageDeal(const MessagePack & message_pack);
		// ���ͱ����Զ��������Ϣ���ͻ���
		void SendNotfiyMessageToClient(INT32 protocolID,INT32 cmd,unsigned long userID);
	};

}