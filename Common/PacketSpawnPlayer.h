#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketSpawnPlayer : public Packet
{
public:
	PacketSpawnPlayer(int playerID, float x, float y);
	DataBuffer GetData();

	short m_playerID;
	float m_positionX;
	float m_positionY;
};