#include "ServerStudy.h"
#include <sstream>

void ExitProgram();

Server* server;
shared_ptr<thread> thed;

int main()
{
	server = new Server();
	server->Init();

	atexit(ExitProgram);

	thed = make_shared<thread>(&Server::EventPorcess, server);
	thed->detach();

	std::stringstream ss;
	ss << thed->get_id();

	server->SendMsg();

	return 0;
}

void ExitProgram()
{
	server->ServerClose();

	thed = nullptr;

	delete server;
}