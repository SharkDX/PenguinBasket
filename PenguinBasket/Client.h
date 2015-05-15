#pragma once

#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <assert.h>
#include <iostream> //Temp?
#include <enet/enet.h>
#include "Packets.h"

typedef void(*ProcessPacket)(ENetPacket* packet, ENetPeer* peer);

class Client
{
public:
	int clientId;

	ProcessPacket processPacketFunc = nullptr;

	Client();
	~Client();

	bool Connect(std::string host, int port);
	void Disconnect();
	void Start();
	void SendPacket(std::shared_ptr<Packet> packet);
	

private:
	ENetHost* m_ENetClient;
	ENetPeer* m_ENetServer;
	bool m_IsRunning;

	std::queue<std::shared_ptr<Packet>> m_OutgoingPackets;
	std::mutex m_OutgoingPacketsMutex;

	std::thread m_RunThread;
	std::thread m_SenderThread;

	void Run();
	void SendData(void* data, int size, int channel, int flags);
	void Sender();
};