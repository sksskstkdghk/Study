#include "ServerStudy.h"

void ExitProgram();

#define OVERLAPPED_THREAD_SIZE 1

Server* server;
shared_ptr<thread> thed[OVERLAPPED_THREAD_SIZE] = {};

int main()
{
	server = new Server();
	server->Init();

	atexit(ExitProgram);

	//thed = make_shared<thread>(&Server::GetEvent, server);
	//thed[0] = make_shared<thread>(&Server::RecvBuffer, server);
	//thed[0] = make_shared<thread>(&Server::IOCPRecvBuffer, server);
	thed[0] = make_shared<thread>(&Server::SendMsg, server);

	//thed->detach();

	//server->ClientAccept();

	for (int i = 0; i < OVERLAPPED_THREAD_SIZE; i++)
		thed[i]->detach();

	server->ClientAccept();

	return 0;
}

void ExitProgram()
{
	server->ServerClose();

	for (int i = 0; i < 2; i++)
		thed[i] = nullptr;

	delete server;
}