#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketChat : public Packet
{
public:
	PacketChat(short clientID, std::string message);
	DataBuffer GetData();

	short m_clientID;
	std::string m_message;
};