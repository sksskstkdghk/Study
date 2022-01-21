#include "Client.h"

void test();

//extern pthread_t pThed;
pthread_t pThed;
Client* client;
shared_ptr<thread> thed;

int main()
{
	client = new Client();
	client->Init();

	atexit(test);

	//thed = make_shared<thread>(&Client::GetMsg, client);
	//pthread_t pThed;

	//pthread_create(&pThed, NULL, &thed.native_handle(), NULL);

	//pThed = thed.native_handle();
	//thed->detach();

	client->SendMsg();

	return 0;
}

void test()
{
	client->Close();

	//thed->~thread();
	thed = nullptr;

	//int temp = pthread_cancel(pThed);

	while (true)
	{

	}

	delete client;
}