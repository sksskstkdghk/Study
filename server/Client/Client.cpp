#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

void Client::Init()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
	
	ZeroMemory(cMsg, PACKET_SIZE);
	cout << "닉네임 설정: ";
	cin >> cMsg;
	send(clientSock, cMsg, strlen(cMsg), 0);
}

void Client::SendMsg()
{
	while (true)
	{
		ZeroMemory(cMsg, PACKET_SIZE);
		
		cin.getline(cMsg, PACKET_SIZE);

		//대상 소켓으로 서버가 정보를 보내는 함수
		send(clientSock, cMsg, strlen(cMsg), 0);
	}
}

void Client::GetMsg()
{
	while (true)
	{
		ZeroMemory(sBuffer, PACKET_SIZE);

		//대상 소켓으로부터 보내온 정보를 받아주는 함수
		recv(clientSock, sBuffer, PACKET_SIZE, 0);

		cout << sBuffer << endl;
	}
}

void Client::End()
{
	WSACleanup();
}
