#pragma once

#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <enet/enet.h>
#include "Packets.h"
#include "Client.h"
#include "Map.h"
#include "Logger.h"

class Server
{
public:
	Server(int port);
	~Server();

	void Run();
	void BroadcastPacket(std::shared_ptr<Packet> packet, Client* fromClient);
	Map* GetMap();
	
	std::map<int, Client*> m_Clients;

private:
	ENetHost* m_ENetHost;
	ENetAddress m_ENetAddress;
	Map* m_map;

	bool m_IsRunning;
	std::mutex m_ClientsMutex;

	void ProcessPacket(ENetPacket* packet, ENetPeer* peer); //TODO Figure out a more suitable name for this function
};