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

	dwError = 0;
	
	ZeroMemory(cMsg, PACKET_SIZE);
	cout << "�г��� ����: ";
	cin >> cMsg;

	/*TEST a;
	a.str = "�� �� �����ϳ�?";
	a.index = 178131;
	a.flo = 731.178;

	memcpy(cMsg, &a, sizeof(TEST));*/

	send(clientSock, cMsg, PACKET_SIZE, 0);

	cEvent = WSACreateEvent();
	WSAEventSelect(clientSock, cEvent, FD_READ | FD_CLOSE);

	//tcpkl.onoff = 1;
	//tcpkl.keepalivetime = 5000;	//1�� ���� ��ȣ�� �����ڴ�
	//tcpkl.keepaliveinterval = 5000; //��ȣ�� ���� �� ������ ���ٸ� 1�ʸ��� �� ���� �ϰڴ�.

	//WSAIoctl(clientSock, SIO_KEEPALIVE_VALS, &tcpkl,
	//	sizeof(tcp_keepalive), 0, 0, &dwError, NULL, NULL);
}

void Client::SendMsg()
{
	while (true)
	{
		ZeroMemory(cMsg, PACKET_SIZE);
		
		cin.getline(cMsg, PACKET_SIZE);

		if (strcmp(cMsg, "exit") == 0)
		{
			break;
		}

		//��� �������� ������ ������ ������ �Լ�
		send(clientSock, cMsg, strlen(cMsg), 0);
	}

	exit(0);
}

void Client::GetMsg()
{
	while (true)
	{
		ZeroMemory(sBuffer, PACKET_SIZE);

		//dwError = WSAGetLastError();
		WSAWaitForMultipleEvents(1, &cEvent, false, INFINITE, false);
		WSAEnumNetworkEvents(clientSock, cEvent, &netEvent);

		if (dwError == WSAECONNABORTED)
		{
			cout << "������ ������ϴ�." << endl;
			break;
		}

		if (netEvent.lNetworkEvents == FD_READ)
		{
			//��� �������κ��� ������ ������ �޾��ִ� �Լ�
			//�޾ƿ��������� �ش� ��ġ���� ���α׷��� �����
			recv(clientSock, sBuffer, PACKET_SIZE, 0);

			cout << sBuffer << endl;
		}
		else if (netEvent.lNetworkEvents == FD_CLOSE)
		{
			cout << "������ ����Ǿ� Ŭ���̾�Ʈ�� �����մϴ�" << endl;

			break;
		}
	}

	exit(0);
}

void Client::Close()
{
	closesocket(clientSock);
	WSACloseEvent(cEvent);

	WSACleanup();
}
