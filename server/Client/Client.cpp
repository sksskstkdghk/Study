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

	tcpkl.onoff = 1;
	tcpkl.keepalivetime = 1000;	//1�� ���� ��ȣ�� �����ڴ�
	tcpkl.keepaliveinterval = 1000; //��ȣ�� ���� �� ������ ���ٸ� 1�ʸ��� �� ���� �ϰڴ�.

	WSAIoctl(clientSock, SIO_KEEPALIVE_VALS, &tcpkl,
		sizeof(tcp_keepalive), 0, 0, &dwError, NULL, NULL);
	
	ZeroMemory(cMsg, PACKET_SIZE);
	cout << "�г��� ����: ";
	cin >> cMsg;

	/*TEST a;
	a.str = "�� �� �����ϳ�?";
	a.index = 178131;
	a.flo = 731.178;

	memcpy(cMsg, &a, sizeof(TEST));*/

	send(clientSock, cMsg, PACKET_SIZE, 0);
}

void Client::SendMsg()
{
	while (true)
	{
		ZeroMemory(cMsg, PACKET_SIZE);
		
		cin.getline(cMsg, PACKET_SIZE);

		if (strcmp(cMsg, "exit") == 0)
		{
			exit(0);
		}

		//��� �������� ������ ������ ������ �Լ�
		send(clientSock, cMsg, strlen(cMsg), 0);
	}
}

void Client::GetMsg()
{
	while (true)
	{
		dwError = WSAGetLastError();

		if (dwError == WSAECONNABORTED)
		{
			cout << "�� ������ ������ϴ�." << endl;
			continue;
		}

		ZeroMemory(sBuffer, PACKET_SIZE);

		//��� �������κ��� ������ ������ �޾��ִ� �Լ�
		recv(clientSock, sBuffer, PACKET_SIZE, 0);

		cout << sBuffer << endl;
	}
}

void Client::End()
{
	closesocket(clientSock);

	WSACleanup();

	cout << "atexit ȣ��" << endl;
}
