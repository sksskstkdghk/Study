#include "Client.h"

void test();

//extern pthread_t pThed;
pthread_t pThed;
Client* client;

int main()
{
	client = new Client();
	client->Init();

	atexit(test);

	thread thed = thread(&Client::GetMsg, client);
	//pthread_t pThed;

	pThed.p = thed.native_handle();
	thed.detach();

	client->SendMsg();

	return 0;
}

void test()
{
	client->End();

	pthread_cancel(pThed);

	cout << "atexit È£Ãâ" << endl;
}