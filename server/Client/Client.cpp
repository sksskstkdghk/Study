#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

void Client::Init()
{
	clientHeart = true;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	clientSock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(clientAddr));

	dwError = 0;
	recvBytes = sendBytes = 0;

	cEvent = WSACreateEvent();
	//WSAEventSelect(clientSock, cEvent, FD_READ | FD_CLOSE);
	if (WSASetEvent(cEvent))
	{
		cout << "WSASetEvent ����" << endl;
	}
	else
	{
		cout << "WSASetEvent ����" << endl;
		exit(0);
	}
	overlapped.hEvent = cEvent;

	ZeroMemory(cBuffer, PACKET_SIZE);
	send(clientSock, cBuffer, strlen(cBuffer), 0);

	cout << "�г��� ����: ";
	cin >> cBuffer;

	send(clientSock, cBuffer, strlen(cBuffer), 0);
	//WSASend(clientSock, &dataBuf, 1, &sendBytes, flag, &overlapped, NULL);

	//tcpkl.onoff = 1;
	//tcpkl.keepalivetime = 5000;	//1�� ���� ��ȣ�� �����ڴ�
	//tcpkl.keepaliveinterval = 5000; //��ȣ�� ���� �� ������ ���ٸ� 1�ʸ��� �� ���� �ϰڴ�.

	//WSAIoctl(clientSock, SIO_KEEPALIVE_VALS, &tcpkl,
	//	sizeof(tcp_keepalive), 0, 0, &dwError, NULL, NULL);
}

void Client::SendMsg()
{
	while (clientHeart)
	{
		ZeroMemory(cBuffer, PACKET_SIZE);
		
		cin.getline(cBuffer, PACKET_SIZE);

		if (strcmp(cBuffer, "exit") == 0)
		{
			clientHeart = false;
			continue;
		}

		//Ŭ�� �������� ������ �����ų� ������ Ŭ�󿡰� �����͸� ������ �Լ�
		//������ �����ڸ� �ش� ���Ͽ��� �����͸� �����ϴ� �Լ�
		send(clientSock, cBuffer, strlen(cBuffer), 0);
	}
}

void Client::GetMsg()
{
	while (clientHeart)
	{
		ZeroMemory(sBuffer, PACKET_SIZE);

		recv(clientSock, sBuffer, strlen(sBuffer), NULL);

		cout << sBuffer << endl;

		/*if (WSARecv(clientSock, &dataBuf, 1, &recvBytes, &flag, &overlapped, NULL) == SOCKET_ERROR)
		{
			dwError = WSAGetLastError();

			if (dwError == WSA_IO_PENDING)
			{
				WSAWaitForMultipleEvents(1, &cEvent, TRUE, WSA_INFINITE, FALSE);

				if (WSAGetOverlappedResult(clientSock, &overlapped, &recvBytes, FALSE, &flag))
				{

				}
			}
			else if (dwError == WSAECONNABORTED)
			{
				cout << "������ ����Ǿ� Ŭ���̾�Ʈ�� �����մϴ�" << endl;

				break;
			}
		}*/
		
		//int isError = WSAEnumNetworkEvents(clientSock, cEvent, &netEvent);

		//if (isError == SOCKET_ERROR)
		//{
		//	cout << WSAGetLastError() << endl;

		//	continue;
		//}

		//if (netEvent.lNetworkEvents == FD_READ)
		//{
		//	//��� �������κ��� ������ ������ �޾��ִ� �Լ�
		//	//�޾ƿ��������� �ش� ��ġ���� ���α׷��� �����
		//	//recv(clientSock, sBuffer, PACKET_SIZE, 0);
		//	

		//	cout << sBuffer << endl;
		//}
		//else if (netEvent.lNetworkEvents == FD_CLOSE)
		//{
		//	cout << "������ ����Ǿ� Ŭ���̾�Ʈ�� �����մϴ�" << endl;

		//	break;
		//}
	}
}

void Client::Close()
{
	closesocket(clientSock);
	WSACloseEvent(cEvent);

	WSACleanup();
}
