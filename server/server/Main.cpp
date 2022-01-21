#include "ServerStudy.h"

void ExitProgram();

Server* server;
shared_ptr<thread> thed[2] = {};

int main()
{
	server = new Server();
	server->Init();

	atexit(ExitProgram);

	//thed = make_shared<thread>(&Server::GetEvent, server);
	thed[0] = make_shared<thread>(&Server::RecvBuffer, server);
	thed[1] = make_shared<thread>(&Server::ClientAccept, server);

	//thed->detach();

	//server->ClientAccept();

	for (int i = 0; i < 2; i++)
		thed[i]->detach();

	server->SendMsg();

	return 0;
}

void ExitProgram()
{
	server->ServerClose();

	for (int i = 0; i < 2; i++)
		thed[i] = nullptr;

	delete server;
}