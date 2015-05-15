#include "PacketPing.h"

PacketPing::PacketPing()
	: Packet(PACKET_PING, DeliveryMethod::ReliableOrdered)
{

}

DataBuffer PacketPing::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	return data;
}