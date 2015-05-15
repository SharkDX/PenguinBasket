#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketPlayerConnected : public Packet
{
public:
	PacketPlayerConnected(int clientID, std::string name);
	DataBuffer GetData();

	short m_clientID;
	std::string m_name;
};