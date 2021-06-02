#pragma once

#include <Ace/Basic_Types.h>
#include <protocol/MessagePack.pb.h>
#include "Type.h"

class ILogicHandler
{
public:
	virtual ~ILogicHandler(){};
//	virtual void handle(char* data,__int32 len,__int32 cmd,ACE_UINT32 channel_id) = 0;
	virtual void handle(const DataLock::MessagePack & message_pack,ACE_UINT32 channel_id) = 0;
};