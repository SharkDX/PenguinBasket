#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketDestroyBlock : public Packet
{
public:
	PacketDestroyBlock(short m_chunkPosition, short positionX, short positionY);
	DataBuffer GetData();

	short m_chunkPosition;
	short m_positionX;
	short m_positionY;
};