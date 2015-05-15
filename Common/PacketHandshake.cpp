#include "PacketHandshake.h"

PacketHandshake::PacketHandshake(unsigned int protocolVersion, short clientId) :
	Packet(PACKET_HAND_SHAKE, DeliveryMethod::ReliableOrdered)
{
	m_protocolVersion = protocolVersion;
	m_clientId = clientId;
}

DataBuffer PacketHandshake::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteInt(m_protocolVersion);
	data.WriteShort(m_clientId);
	return data;
}