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

	cBuffer = new unsigned char[TEST_FILE_SIZE];

	ZeroMemory(cBuffer, TEST_FILE_SIZE);
	ZeroMemory(sBuffer, PACKET_SIZE + 4);

	recvDataBuf.len = PACKET_SIZE + 4;
	recvDataBuf.buf = sBuffer;

	sendDataBuf.len = TEST_FILE_SIZE;
	sendDataBuf.buf = (char*)cBuffer;

	dwError = 0;
	recvBytes = sendBytes = 0;

	cEvent = WSACreateEvent();

	ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
	overlapped.hEvent = cEvent;

	connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(clientAddr));

	//WSAEventSelect(clientSock, cEvent, FD_READ | FD_CLOSE);

	//WSASend(clientSock, &sendDataBuf, 1, &sendBytes, 0, &overlapped, NULL);

	if (WSARecv(clientSock, &recvDataBuf, 1, &recvBytes, &flag, &overlapped, NULL) == SOCKET_ERROR)
	{
		cout << WSAGetLastError() << endl;

		/*if (WSAGetLastError() == WSA_IO_PENDING)
			cout << "new Client BackGround Recv Success" << endl;*/
	}
	else
		cout << "BackGround Recv Failure" << endl;

	if (WSASetEvent(cEvent))
	{
		cout << "WSASetEvent 성공" << endl;
	}
	else
	{
		cout << "WSASetEvent 실패" << endl;
		exit(0);
	}

	cout << "닉네임 설정: ";

	//tcpkl.onoff = 1;
	//tcpkl.keepalivetime = 5000;	//1초 마다 신호를 보내겠다
	//tcpkl.keepaliveinterval = 5000; //신호를 보낸 후 응답이 없다면 1초마다 재 전송 하겠다.

	//WSAIoctl(clientSock, SIO_KEEPALIVE_VALS, &tcpkl,
	//	sizeof(tcp_keepalive), 0, 0, &dwError, NULL, NULL);
}

void Client::SendMsg()
{
	BOOL result = TRUE;

	char asdf[100] = {};

	int size = 0;

	while (clientHeart)
	{
		stream.open("메종드마왕 로고.png", ios::in | ios::binary);

		//cin.getline(sendDataBuf.buf + 4, PACKET_SIZE);
		cin.getline(asdf, 100);

		/*if (strcmp(sendDataBuf.buf + 4, "exit") == 0)
		{
			clientHeart = false;
			continue;
		}*/

		//sendBytes = strlen(sendDataBuf.buf + 4) + 4;

		//클라가 서버에게 정보를 보내거나 서버가 클라에게 데이터를 보내는 함수
		//엄밀히 따지자면 해당 소켓에게 데이터를 전달하는 함수
		//memcpy_s(sendDataBuf.buf, 4, &sendBytes, 4);

		//stream.seekg(size);

		stream.read(sendDataBuf.buf, TEST_FILE_SIZE);

		cout << stream.tellg() << endl;

		//size += PACKET_SIZE;

		result =  WSAResetEvent(cEvent);

		if(result == FALSE)
			cout << "WSAResetEvent failed with error = " << WSAGetLastError() << endl;

		ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
		overlapped.hEvent = cEvent;

		//send(clientSock, cBuffer, a + 4, 0);
		if (WSASend(clientSock, &sendDataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSASend() failed with error %d\n", WSAGetLastError());
				continue;
			}
		}
		else
		{
			printf("WSASend() is OK! and %d만큼 데이터 보냄\n", sendBytes);
			stream.close();
		}
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
				cout << "서버가 종료되어 클라이언트를 종료합니다" << endl;

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
		//	//대상 소켓으로부터 보내온 정보를 받아주는 함수
		//	//받아오기전까지 해당 위치에서 프로그램이 멈춘다
		//	//recv(clientSock, sBuffer, PACKET_SIZE, 0);
		//	

		//	cout << sBuffer << endl;
		//}
		//else if (netEvent.lNetworkEvents == FD_CLOSE)
		//{
		//	cout << "서버가 종료되어 클라이언트를 종료합니다" << endl;

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
