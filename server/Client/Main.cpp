#include "Client.h"

void test();

//extern pthread_t pThed;
pthread_t pThed;
Client* client;
shared_ptr<thread> thed;

int main()
{
	//FILE* file = fopen("메종드마왕 로고.png", "r");

	//fstream stream;

	//stream.open("메종드마왕 로고.txt");
	/*stream.open("테스트.txt", fstream::app);

	if (stream.is_open())
	{
		char str[20] = "이거슨 수류탄이여!!";

		stream << str << endl;
	}*/

	client = new Client();
	client->Init();

	atexit(test);

	client->SendMsg();

	//thed = make_shared<thread>(&Client::GetMsg, client);
	//pthread_t pThed;

	//pthread_create(&pThed, NULL, &thed.native_handle(), NULL);

	//pThed = thed.native_handle();
	//thed->detach();

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