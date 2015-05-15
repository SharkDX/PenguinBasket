#include "PacketSpawnPlayer.h"


PacketSpawnPlayer::PacketSpawnPlayer(int playerID, float x, float y) :
	Packet(PACKET_SPAWN_PLAYER, DeliveryMethod::ReliableOrdered)
{
	m_playerID = playerID;
	m_positionX = x;
	m_positionY = y;
}

DataBuffer PacketSpawnPlayer::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteShort(m_playerID);
	data.WriteFloat(m_positionX);
	data.WriteFloat(m_positionY);
	return data;
}
