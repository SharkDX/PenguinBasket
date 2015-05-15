#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketCreateBlock : public Packet
{
public:
	PacketCreateBlock(char blockID, short chunkPosition, short positionX, short positionY);
	DataBuffer GetData();

	char m_blockID;
	short m_chunkPosition;
	short m_positionX;
	short m_positionY;
};