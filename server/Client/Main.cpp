#include "Client.h"

int main()
{
	Client* client = new Client();
	client->Init();

	thread(&Client::GetMsg, client).detach();
	client->SendMsg();


	return 0;
}