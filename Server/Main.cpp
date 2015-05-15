#include <iostream>
#include <enet/enet.h>
#include "Server.h"

using namespace std;

int main(int argc, char const *argv[])
{
	enet_initialize();
	Server* serv = new Server(1234);
	serv->Run();
	enet_deinitialize();
	return 0;
}