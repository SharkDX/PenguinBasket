#include "Server.h"

Server::Server(int port)
{
	m_map = new Map(100, 100);
	m_ENetAddress.host = ENET_HOST_ANY;
	m_ENetAddress.port = port;
	m_ENetHost = enet_host_create(&m_ENetAddress, 32, 3, 0, 0);
	m_IsRunning = false;
	Logger::Print("Server created and bound to port %d", port);
}

Server::~Server()
{
	enet_host_destroy(m_ENetHost);
}

void Server::Run()
{
	Logger::Print("Server is now online and listening to incoming traffic");
	m_IsRunning = true;
	int nextAvailableID = 0;
	ENetEvent event;

	while (m_IsRunning)
	{
		while (enet_host_service(m_ENetHost, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				Client* c = new Client(this, event.peer, nextAvailableID);
				m_Clients.insert(std::pair<int, Client*>(nextAvailableID, c));
				nextAvailableID++;
				event.peer->data = c;
				Logger::Print("A new client connected from %x:%u assigned with id %d", event.peer->address.host, event.peer->address.port, m_Clients.size() - 1);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				ProcessPacket(event.packet, event.peer);
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				m_ClientsMutex.lock();
				Client* c = m_Clients[event.data];
				event.peer->data = NULL;
				int playerID = c->GetPlayerID();
				m_Clients.erase(playerID);
				//delete c; //Delete the client after removed it reference from all other places
				//BroadcastPacket(std::make_shared<PacketPlayerDisconnected>(playerID), nullptr);
				Logger::Print("Client %d has disconnected", playerID);
				m_ClientsMutex.unlock();
				break;
			}
			}
		}
	}

	m_IsRunning = false;
}

void Server::ProcessPacket(ENetPacket* packet, ENetPeer* peer)
{
	//TOOD Need to change it to a more effienct method like in the client.
	//TODO Maybe move the server related logic to handle here before assign the packets to the client?

	unsigned char header = packet->data[0];
	Client* client = (Client*)peer->data;
	DataBuffer dataBuffer = DataBuffer(((char*)packet->data + 1), packet->dataLength - 1);

	switch (header)
	{
		case PACKET_HAND_SHAKE:
		{
			int protocolVersion = 0;
			memcpy(&protocolVersion, &packet->data[1], sizeof(int));
			client->QueueIncomingPacket(std::shared_ptr<Packet>(new PacketHandshake(protocolVersion, (short)client->GetPlayerID())));
			break;
		}
		case PACKET_PING:
		{
			client->SendPacket(std::shared_ptr<Packet>(new PacketPing()));
			break;
		}
		case PACKET_PLAYER_POSITION:
		{
			short playerID = 0;
			memcpy(&playerID, &packet->data[1], sizeof(short));

			float posX = 0;
			memcpy(&posX, &packet->data[3], sizeof(float));

			float posY = 0;
			memcpy(&posY, &packet->data[7], sizeof(float));

			float velX = 0;
			memcpy(&velX, &packet->data[11], sizeof(float));

			float velY = 0;
			memcpy(&velY, &packet->data[15], sizeof(float));

			//Logger::Print("%d, %f, %f", playerID, posX, posY);
			client->QueueIncomingPacket(std::make_shared<PacketPlayerPosition>(playerID, posX, posY, velX, velY));
			break;
		}
		case PACKET_CREATE_BLOCK:
		{
			char blockID = dataBuffer.ReadChar();
			short chunkPos = dataBuffer.ReadShort(); //TODO Use that when the engine will run on chunks
			short posX = dataBuffer.ReadShort();
			short posY = dataBuffer.ReadShort();
			client->QueueIncomingPacket(std::make_shared<PacketCreateBlock>(blockID, chunkPos, posX, posY));
			break;
		}
		case PACKET_DESTROY_BLOCK:
		{
			short chunkPos = dataBuffer.ReadShort(); //TODO Use that when the engine will run on chunks
			short posX = dataBuffer.ReadShort();
			short posY = dataBuffer.ReadShort();
			client->QueueIncomingPacket(std::make_shared<PacketDestroyBlock>(chunkPos, posX, posY));
			break;
		}
	}
}

void Server::BroadcastPacket(std::shared_ptr<Packet> packet, Client* fromClient)
{
	for (std::map<int, Client*>::iterator it = m_Clients.begin(); it != m_Clients.end(); it++)
	{
		Client* c = (*it).second;

		if (c->GetReady() == false || (fromClient != nullptr && fromClient->GetPlayerID() == c->GetPlayerID()))
			continue;

		c->SendPacket(packet);
	}
}

Map* Server::GetMap()
{
	return m_map;
}