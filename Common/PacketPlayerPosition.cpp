#include "PacketPlayerPosition.h"

PacketPlayerPosition::PacketPlayerPosition(int playerID, float x, float y, float vX, float vY) :
	Packet(PACKET_PLAYER_POSITION, DeliveryMethod::UnreliableOrdered)
{
	m_playerID = playerID;
	m_positionX = x;
	m_positionY = y;
	m_velocityX = vX;
	m_velocityY = vY;
}

DataBuffer PacketPlayerPosition::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteShort(m_playerID);
	data.WriteFloat(m_positionX);
	data.WriteFloat(m_positionY);
	data.WriteFloat(m_velocityX);
	data.WriteFloat(m_velocityY);
	return data;
}