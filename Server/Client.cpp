#include "Client.h"
#include "Server.h"

Client::Client(Server* server, ENetPeer* enetPeer, int playerID)
{
	m_eNetPeer = enetPeer;
	m_ready = false;
	m_playerID = playerID;
	m_playerName = "Boobala";
	m_isActive = true;
	m_server = server;
	m_processIncomingPacketsThread = std::thread(&Client::ProcessIncomingPackets, this);
}

Client::~Client()
{
	
}

void Client::QueueIncomingPacket(std::shared_ptr<Packet> packet)
{
	m_incomingPacketsMutex.lock();
	m_incomingPackets.push(packet);
	m_incomingPacketsMutex.unlock();
}

void Client::SendPacket(std::shared_ptr<Packet> packet)
{
	int channel = 0;
	int flags = 0;
	
	switch (packet->GetDeliveryMethod())
	{
	case DeliveryMethod::ReliableOrdered:
		channel = 0;
		flags = ENET_PACKET_FLAG_RELIABLE;
		break;
	case DeliveryMethod::UnreliableOrdered:
		channel = 1;
		flags = 0;
		break;
	case DeliveryMethod::UnreliableUnordered:
		channel = 2;
		flags = ENET_PACKET_FLAG_UNSEQUENCED;
		break;
	default:
		assert(packet->GetDeliveryMethod() != DeliveryMethod::ReliableUnordered); //TODO Check how to use this behaviour with ENet
		break;
	}

	DataBuffer data = packet->GetData(); //return a copy of the data, so the packet itself is safe for deletion! (but will be deleted by the shared_ptr)
	SendData(data.GetRawData(), data.GetDataSize(), channel, flags);
}

void Client::SendData(void* data, int size, int channel, int flags)
{
	ENetPacket* packet = enet_packet_create(data, size, flags);
	enet_peer_send(m_eNetPeer, channel, packet); //Only quques them in the internal ENet's quque, will be send in the next call to enet_host_service() or enet_host_flush()
}

void Client::ProcessIncomingPackets()
{
	while (m_isActive)
	{
		m_incomingPacketsMutex.lock();

		while (m_incomingPackets.size() > 0)
		{
			std::shared_ptr<Packet> p = m_incomingPackets.front();
			m_incomingPackets.pop();

			//Map the packet to the it's respective function by type
			switch (p->GetHeader())
			{
			case PACKET_HAND_SHAKE:
				ReceivedPacketHandshake(std::static_pointer_cast<PacketHandshake>(p));
				break;
			case PACKET_PLAYER_POSITION:
				ReceivedPacketPlayerPosition(std::static_pointer_cast<PacketPlayerPosition>(p));
				break;
			case PACKET_CREATE_BLOCK:
				ReceivedPacketCreateBlock(std::static_pointer_cast<PacketCreateBlock>(p));
				break;
			case PACKET_DESTROY_BLOCK:
				ReceivedPacketDestroyBlock(std::static_pointer_cast<PacketDestroyBlock>(p));
				break;
			default:
				Logger::Print("Unknown packet received %d, from client %d", p->GetHeader(), m_playerID);
				break;
			}
		}

		m_incomingPacketsMutex.unlock();
	}
}

void Client::ReceivedPacketHandshake(std::shared_ptr<PacketHandshake> packet)
{
	Logger::Print("Client %d: Handshake received, protocol version: %d", m_playerID, packet->m_protocolVersion);
	//TODO Check if the protocol version of the client is legit, i.e matches the protocol version of the server

	std::shared_ptr<PacketHandshake> packetHandshake = std::make_shared<PacketHandshake>(PROTOCOL_VERSION, m_playerID);
	SendPacket(packetHandshake);
	m_server->BroadcastPacket(std::make_shared<PacketPlayerConnected>(m_playerID, this->GetPlayerName()), this);
	m_server->BroadcastPacket(std::make_shared<PacketSpawnPlayer>(m_playerID, 0, 0), this);
	std::shared_ptr<PacketMapData> packetMapData = std::make_shared<PacketMapData>(m_playerID, m_server->GetMap());
	SendPacket(packetMapData);

	//Inform the new play about all the other players
	for (std::map<int, Client*>::iterator it = m_server->m_Clients.begin(); it != m_server->m_Clients.end(); it++)
	{
		Client* client = it->second;

		if (client->GetPlayerID() == this->GetPlayerID())
			continue;

		this->SendPacket(std::make_shared<PacketPlayerConnected>(client->GetPlayerID(), client->GetPlayerName()));
		this->SendPacket(std::make_shared<PacketSpawnPlayer>(client->GetPlayerID(), 0, 0));
	}

	m_ready = true;
}

void Client::ReceivedPacketPlayerPosition(std::shared_ptr<PacketPlayerPosition> packet)
{
	m_server->BroadcastPacket(packet, this);
	//Logger::Print("%d, %f, %f", packet->m_PlayerID, packet->m_positionX, packet->m_positionY);
}

void Client::ReceivedPacketCreateBlock(std::shared_ptr<PacketCreateBlock> packet)
{
	m_server->GetMap()->SetBlock(packet->m_positionX, packet->m_positionY, packet->m_blockID);
	m_server->BroadcastPacket(packet, this);
}

void Client::ReceivedPacketDestroyBlock(std::shared_ptr<PacketDestroyBlock> packet)
{
	m_server->GetMap()->SetBlock(packet->m_positionX, packet->m_positionY, 0); //TODO Verify that 0 is the air block ID
	m_server->BroadcastPacket(packet, this);
}