#pragma once

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <assert.h>
#include <enet/enet.h>
#include "Logger.h"
#include "Packet.h"
#include "DataBuffer.h"
#include "Player.h"
#include "Packets.h"

class Server; //Forward

class Client
{
public:
	Client(Server* server, ENetPeer* enetPeer, int playerID);
	~Client();

	bool GetReady() { return m_ready; }
	int GetPlayerID() { return m_playerID; }
	std::string GetPlayerName() { return m_playerName; }
	void QueueIncomingPacket(std::shared_ptr<Packet> packet);
	void SendPacket(std::shared_ptr<Packet> packet);

	Player m_Player;
	std::mutex m_incomingPacketsMutex;
private:
	Server* m_server;
	bool m_ready;
	ENetPeer* m_eNetPeer;
	int m_playerID;
	std::string m_playerName;
	bool m_isActive;
	std::queue<std::shared_ptr<Packet>> m_incomingPackets;
	std::thread m_processIncomingPacketsThread;
	
	//Proccessing incoming packets
	void ProcessIncomingPackets();

	/* Pass the packet data to ENet */
	void SendData(void* data, int size, int channel, int flags);

	/* Packet handlers */
	void ReceivedPacketHandshake(std::shared_ptr<PacketHandshake> packet);
	void ReceivedPacketPlayerPosition(std::shared_ptr<PacketPlayerPosition> packet);
	void ReceivedPacketCreateBlock(std::shared_ptr<PacketCreateBlock> packet);
	void ReceivedPacketDestroyBlock(std::shared_ptr<PacketDestroyBlock> packet);
};
