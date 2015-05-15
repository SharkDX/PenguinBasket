#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketHandshake : public Packet
{
public:
	PacketHandshake(unsigned int protocolVersion, short clientId);
	DataBuffer GetData();

	unsigned int m_protocolVersion;
	short m_clientId;
};