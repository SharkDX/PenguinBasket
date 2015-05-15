#include "PacketCreateBlock.h"

PacketCreateBlock::PacketCreateBlock(char blockID, short chunkPosition, short positionX, short positionY)
	: Packet(PACKET_CREATE_BLOCK, DeliveryMethod::ReliableOrdered)
{
	m_blockID = blockID;
	m_chunkPosition = chunkPosition;
	m_positionX = positionX;
	m_positionY = positionY;
}

DataBuffer PacketCreateBlock::GetData()
{
	DataBuffer dataBuffer;
	dataBuffer.WriteChar(m_header);
	dataBuffer.WriteChar(m_blockID);
	dataBuffer.WriteShort(m_chunkPosition);
	dataBuffer.WriteShort(m_positionX);
	dataBuffer.WriteShort(m_positionY);
	return dataBuffer;
}