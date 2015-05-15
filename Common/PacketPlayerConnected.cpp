#include "PacketPlayerConnected.h"

PacketPlayerConnected::PacketPlayerConnected(int clientID, std::string name) :
	Packet(PACKET_PLAYER_CONNECTED, DeliveryMethod::ReliableOrdered)
{
	this->m_clientID = clientID;
	this->m_name = name;
}

DataBuffer PacketPlayerConnected::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteShort(m_clientID);
	data.WriteString(m_name);
	return data;
}