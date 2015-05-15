#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketPlayerPosition : public Packet
{
public:
	PacketPlayerPosition(int playerID, float x, float y, float vX, float vY);
	DataBuffer GetData();

	short m_playerID;
	float m_positionX;
	float m_positionY;
	float m_velocityX;
	float m_velocityY;
};