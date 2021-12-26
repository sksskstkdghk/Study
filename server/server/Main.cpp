#include "ServerStudy.h"

int main()
{
	Server* server = new Server();
	server->Init();

	thread(&Server::ClientAccept, server).detach();

	server->SendMsg();

	return 0;
}