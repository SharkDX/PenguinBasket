#include "PacketPlayerDisconnected.h"

PacketPlayerDisconnected::PacketPlayerDisconnected(int clientID) :
	Packet(PACKET_PLAYER_CONNECTED, DeliveryMethod::ReliableOrdered)
{
	this->m_clientID = clientID;
}

DataBuffer PacketPlayerDisconnected::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteShort(m_clientID);
	return data;
}