#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketPlayerDisconnected : public Packet
{
public:
	PacketPlayerDisconnected(int clientID);
	DataBuffer GetData();

	short m_clientID;
};