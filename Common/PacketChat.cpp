#include "PacketChat.h"

PacketChat::PacketChat(short clientID, std::string message)
	: Packet(PACKET_CHAT_MESSAGE, DeliveryMethod::ReliableOrdered)
{
	m_clientID = clientID;
	m_message = message;
}

DataBuffer PacketChat::GetData()
{
	DataBuffer dataBuffer;
	dataBuffer.WriteChar(m_header);
	dataBuffer.WriteShort(m_clientID);
	dataBuffer.WriteString(m_message);
	return dataBuffer;
}