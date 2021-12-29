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
	cout << "닉네임 설정: ";
	cin >> cMsg;

	/*TEST a;
	a.str = "이 건 가능하냐?";
	a.index = 178131;
	a.flo = 731.178;

	memcpy(cMsg, &a, sizeof(TEST));*/

	send(clientSock, cMsg, PACKET_SIZE, 0);

	cEvent = WSACreateEvent();
	WSAEventSelect(clientSock, cEvent, FD_READ | FD_CLOSE);

	//tcpkl.onoff = 1;
	//tcpkl.keepalivetime = 5000;	//1초 마다 신호를 보내겠다
	//tcpkl.keepaliveinterval = 5000; //신호를 보낸 후 응답이 없다면 1초마다 재 전송 하겠다.

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

		//대상 소켓으로 서버가 정보를 보내는 함수
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
			cout << "연결이 끊겼습니다." << endl;
			break;
		}

		if (netEvent.lNetworkEvents == FD_READ)
		{
			//대상 소켓으로부터 보내온 정보를 받아주는 함수
			//받아오기전까지 해당 위치에서 프로그램이 멈춘다
			recv(clientSock, sBuffer, PACKET_SIZE, 0);

			cout << sBuffer << endl;
		}
		else if (netEvent.lNetworkEvents == FD_CLOSE)
		{
			cout << "서버가 종료되어 클라이언트를 종료합니다" << endl;

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
