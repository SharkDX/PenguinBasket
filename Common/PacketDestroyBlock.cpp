#include "PacketDestroyBlock.h"

PacketDestroyBlock::PacketDestroyBlock(short chunkPosition, short positionX, short positionY)
	: Packet(PACKET_DESTROY_BLOCK, DeliveryMethod::ReliableOrdered)
{
	m_chunkPosition = chunkPosition;
	m_positionX = positionX;
	m_positionY = positionY;
}

DataBuffer PacketDestroyBlock::GetData()
{
	DataBuffer dataBuffer;
	dataBuffer.WriteChar(m_header);
	dataBuffer.WriteShort(m_chunkPosition);
	dataBuffer.WriteShort(m_positionX);
	dataBuffer.WriteShort(m_positionY);
	return dataBuffer;
}