#include "ServerStudy.h"

Server::Server()
{

}

Server::~Server()
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		//WSACloseEvent(events[i]);
	}

	serverHeart = false;
	ZeroMemory(clientDatas, sizeof(clientData) * acceptClientSize);
	//ZeroMemory(events, sizeof(WSAEVENT) * acceptClientSize);
}

void Server::Init()
{
	acceptClientSize = 1;	//접속한 전체 클라이언트 수(0 번째는 서버가 들어가기에 사실상 크기는 size - 1)
	serverHeart = true;

	//서버 생성시 가장 먼저 호출해주는 함수
	//해당 함수로 어떤 윈도우즈 소켓을 활용할 것이지 알려준다.
	//1번 인자는 소켓 버전이, 2번 인자는 WSADATA구조체 포인터 타입이 들어간다.
	//1번 인자는 WORD타입이므로 MAKEWORD매크로를 이용하여 실수를 정수형으로 변환시켜준다.
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//일종의 윈도우 핸들
	SOCKET hListen;

	SOCKADDR_IN tListenAddr = {};

	//tcp소켓으로 소켓 생성
	//tcp소켓은 크게 2가지로 나뉨
	//1. 다른 컴퓨터로부터 들어오는 접속 승인 요청을 수락해주는 소켓
	//2. 다른 컴퓨터와 연결된 소켓
	//컴퓨터는 IP주소라는 개념을 이용하여 해당 컴퓨터의 주소 정보를 만들어낸다.
	//IP주소는 크게 2가지로 나뉘게 된다. IVP4, IPV6
	//각각 32bit주소 체계, 16byte주소 체계로 나뉨
	//PF_INET를 사용하면 IPV4 타입을 사용하겠다는 의미
	//SOCK_STREAM을 넣어주면 연결지항형 소켓을 만들겠다는 뜻
	//3번 째 인자는 prorocol이 들어간다. protocol은 통신규약을 말함
	//IPPROTO_TCP는 TCP를 사용하겠다고 지정해 주는것(0을 넣어도 자동으로 TCP로 할당해줌)
	//hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	hListen = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, NULL, WSA_FLAG_OVERLAPPED);

	//optval = true;
	//setsockopt(hListen, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optval, sizeof(BOOL));

	tListenAddr.sin_family = AF_INET;

	//포트번호 설정(2byte내)
	//htons : host to network short의 약자
	//해당 함수를 거치면 반드시 빅엔디안 방식으로 데이터를 변환하여 설정한다.
	tListenAddr.sin_port = htons(PORT);

	//서버는 현재 동작되는 컴퓨터의 IP주소로 설정
	//INADDR_ANY를 넣어주면 현재 컴퓨터의 IP주소로 설정함
	//s_addr은 ipv4 internet address를 의미한다.
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind 함수는 소켓에 주소정보를 연결
	//즉 lsiten소켓의 역할은 접속승인만 해줌
	//위에서 설정한 주소 정보를 WinSock2.h에 정의된 bind 함수를 이용하여 소켓에 묶어준다.
	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(SOCKADDR_IN));

	//listen함수는 연결을 수신하는 상태로 소켓의 상태를 변경한다. 즉 소켓을 접속 대기 상태로 만들어줌
	//SOMAXCONN은 한번에 요청 가능한 최대 접속승인 수를 의미(int 최대값 21억~~)
	listen(hListen, SOMAXCONN);

	//CreateIoCompletionPort = 커널 객체를 만들어 주는 함수
	//IOCP객체를 만들 때 핸들 파라미터에 listen소켓을 넣어주는게 아닌 INVALID_HANDLE_VALUE를 넣어줘야 한다.
	//마지막 매개변수가 0이라면 현재 컴퓨터에 존재하는 모든 cpu코어를 풀로 활용하겠다는 뜻
	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//cBuffer = new char[(PACKET_MAX_SIZE + 4) * 10];
	//cBuffer = new unsigned char[PACKET_MAX_SIZE];
	sBuffer = new char[(PACKET_MAX_SIZE + 4) * 10];

	//ZeroMemory(cBuffer, (PACKET_MAX_SIZE + 4) * 10);
	ZeroMemory(cBuffer, PACKET_MAX_SIZE);
	ZeroMemory(sBuffer, (PACKET_MAX_SIZE + 4) * 10);

	//recvDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
	recvDataBuf.len = PACKET_MAX_SIZE;
	recvDataBuf.buf = cBuffer;

	sendDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
	sendDataBuf.buf = sBuffer;

	clientDatas[0].first = "server";
	clientDatas[0].second.clientSock = hListen;
	//clientDatas[0].second.clientAddr = tListenAddr;

	//events[0] = WSACreateEvent();

	/*if (WSASetEvent(events[0]) == false)
		cout << "WSASetEvent 실패" << endl;
	else
		cout << "WSASetEvent 성공" << endl;*/

	//WSASetEvent(events[0]);
	//WSAEventSelect(hListen, events[0], FD_ACCEPT);
}

//서버 내 모든 클라이언트 접속 종료(서버는 제외)
void Server::ServerClose()
{
	for (int i = 1; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		//WSACloseEvent(events[i]);
	}

	ZeroMemory(clientDatas + 1, sizeof(clientData) * (acceptClientSize - 1));
	//ZeroMemory(events + 1, sizeof(WSAEVENT) * (acceptClientSize - 1));
}

void Server::EventProcess(DWORD index, int error)
{
	//if (error == SOCKET_ERROR)
	//{
	//	//cout << clientDatas[index].first << "소켓 에러가 발생하였습니다." << endl;

	//	cout << index << "번 소켓의 에러: " << WSAGetLastError() << endl;
	//	return;
	//}

	//if (netEvent.lNetworkEvents == FD_ACCEPT)
	//{
	//	ClientAccept();
	//}
	//else if (netEvent.lNetworkEvents == FD_CLOSE)
	//{
	//	CloseEvent(index);
	//}
	//else if (netEvent.lNetworkEvents == FD_READ)
	//{
	//	ReadEvent(index);
	//}
}

void Server::EventProcess()
{
	
}

void Server::GetEvent()
{
	while (serverHeart)
	{
		ZeroMemory(cBuffer, PACKET_MAX_SIZE);

		//DWORD index = WSAWaitForMultipleEvents(acceptClientSize, &clientDatas[index].second.event, false, 1000, false);
		//int error = WSAEnumNetworkEvents(clientDatas[index].second.clientSock, events[index], &netEvent);


		//if (index >= CLIENT_MAX_SIZE)
			//continue;

		//thread(&Server::EventProcess, this, index, error).detach();
		//EventProcess(index, error);
	}

	if (serverHeart == false)
	{
		cout << "스레드 종료" << endl;
	}
}

void Server::CloseEvent(int index)
{
	cout << clientDatas[index].first << "님이 종료하였습니다." << endl;

	ZeroMemory(sBuffer, PACKET_MAX_SIZE);

	InsertMsg(sBuffer, (char*)"님이 종료하였습니다.", 20);
	InsertMsg(sBuffer, (char*)clientDatas[index].first.c_str(), clientDatas[index].first.size());

	SendAllMsg(sBuffer);
	
	closesocket(clientDatas[index].second.clientSock);
	//WSACloseEvent(events[index]);

	acceptClientSize--;

	if (index < acceptClientSize)
		return;

	clientDatas[index].first = clientDatas[acceptClientSize].first;
	clientDatas[index].second = clientDatas[acceptClientSize].second;
	//events[index] = events[acceptClientSize];
}

void Server::RecvBuffer()
{
	int index = 1;
	BOOL bResult = TRUE;

	//filebuf = new unsigned char[TEST_FILE_SIZE];

	while (serverHeart)
	{
		//if (acceptClientSize < 2)
		//	continue;

		////ZeroMemory(recvDataBuf.buf, PACKET_MAX_SIZE);

		//if ((index = WSAWaitForMultipleEvents(acceptClientSize, events, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		//{
		//	cout << "WSAWaitForMultipleEvents() failed " << WSAGetLastError() << endl;
		//	continue;
		//}
		//else
		//	cout << "WSAWaitForMultipleEvents() is OK" << endl;

		//if (index == WSA_WAIT_EVENT_0)
		//{
		//	WSAResetEvent(events[WSA_WAIT_EVENT_0]);
		//	cout << "이건 서버야!" << endl;
		//	continue;
		//}

		//bResult = WSAResetEvent(events[index - WSA_WAIT_EVENT_0]);

		//if (bResult == FALSE)
		//	cout << "WSAResetEvent failed with error = " << WSAGetLastError() << endl;
		//

		//stOverlappedEx* temp = &clientDatas[index - WSA_WAIT_EVENT_0].second.m_stRecvOverlappedEx;
		//string name = clientDatas[index - WSA_WAIT_EVENT_0].first;

		//if (WSAGetOverlappedResult(temp->clientSock, &temp->overlapped, &temp->recvBytes, FALSE, &flags) == TRUE)
		//{
		//	if (temp->recvBytes <= 0)
		//		cout << "no data here" << endl;
		//	else
		//	{
		//		if (temp->recvBytes < PACKET_MAX_SIZE)
		//		{
		//			cout << "더 작게 옴" << endl;
		//		}

		//		stream.open("test.txt", ios::app | ios::binary);

		//		stream.write(cBuffer, temp->recvBytes);

		//		//memcpy_s(filebuf + temp->writeBytes, temp->recvBytes, recvDataBuf.buf, temp->recvBytes);

		//		temp->writeBytes += temp->recvBytes;

		//		cout << "다운받은 총 크기: " << temp->writeBytes << endl;
		//		stream.close();

		//		//Sleep(100);
		//	}


		//	/*if (temp->recvBytes <= 0)
		//		cout << "no data here" << endl;
		//	else
		//	{
		//		int size;

		//		memcpy_s(&size, 4, recvDataBuf.buf, 4);

		//		cout << size << " " << recvDataBuf.buf + 4 << endl;

		//		if (name.size() <= 0)
		//		{
		//			SetClientName(index);
		//			name = clientDatas[index].first;
		//		}
		//		else
		//			WriteData(*temp, name);
		//	}*/

		//	

		//	//stream << recvDataBuf.buf;

		//	flags = 0;
		//	ZeroMemory(&temp->overlapped, sizeof(WSAOVERLAPPED));
		//	temp->overlapped.hEvent = events[index];

		//	/*if(TEST_FILE_SIZE - temp->writeBytes > PACKET_MAX_SIZE)
		//		recvDataBuf.len = PACKET_MAX_SIZE;
		//	else
		//		recvDataBuf.len = TEST_FILE_SIZE - temp->writeBytes;*/

		//	recvDataBuf.buf = (char*)cBuffer;

		//	ZeroMemory(cBuffer, PACKET_MAX_SIZE);

		//	if (WSARecv(temp->clientSock, &recvDataBuf, 1, &temp->recvBytes, &flags, &temp->overlapped, NULL) == SOCKET_ERROR)
		//	{
		//		if (WSAGetLastError() == WSA_IO_PENDING)
		//		{
		//			/*recvDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
		//			recvDataBuf.buf = sBuffer;*/

		//			cout << name << " BackGround Recv Success" << endl;
		//		}
		//	}
		//	else
		//		cout << "BackGround Recv Failure" << endl;
		//}
	}

	//recv(clientDatas[index].second.clientSock, cBuffer, PACKET_MAX_SIZE, 0);

	////이름 설정
	//if (strcmp(clientDatas[index].first.c_str(), "") == 0)
	//{
	//	clientDatas[index].first.insert(0, cBuffer);
	//	return;
	//}

	//char* temp = (char*)clientDatas[index].first.c_str();
	//InsertMsg(cBuffer, (char*)" : ", 3);
	//InsertMsg(cBuffer, temp, strlen(temp));
	//cout << cBuffer << endl;

	//for (int i = 0; i < acceptClientSize; i++)
	//{
	//	if (strcmp(clientDatas[i].first.c_str(), clientDatas[index].first.c_str()) != 0)
	//	{
	//		send(clientDatas[i].second.clientSock, cBuffer, PACKET_MAX_SIZE, 0);
	//	}
	//}
}

void Server::IOCPRecvBuffer()
{
	int index = 1;
	BOOL bResult = TRUE;

	while (serverHeart)
	{
		if (acceptClientSize < 2)
			continue;

		stOverlappedEx* temp = &clientDatas[index - WSA_WAIT_EVENT_0].second.m_stRecvOverlappedEx;
		//string name = clientDatas[index - WSA_WAIT_EVENT_0].first;

	}
}

void Server::WriteData(CLIENT client, const string name)
{
	int leftCount = 0;

	for (int i =1; i <= recvCount; i++)
	{
		int a;

		memcpy_s(&a, 4, recvDataBuf.buf + (i * (PACKET_MAX_SIZE + 4)), 4);

		if (a - PACKET_MAX_SIZE > 0)
			leftCount++;

		//if(cbuffer)
	}
}

void Server::SendBuffer()
{
	int index = 1;

	DWORD sendBytes;

	while (serverHeart)
	{
		if (acceptClientSize < 2)
			continue;

		if (index > acceptClientSize)
			index = 1;

		stOverlappedEx* temp = &clientDatas[index].second.m_stSendOverlappedEx;

		if (WSASend(clientDatas[index].second.clientSock, &temp->m_wsaBuf, 1,
			&sendBytes, 0, (LPWSAOVERLAPPED)&temp, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSASend() failed with error %d\n", WSAGetLastError());
				continue;
			}
		}
		else
			printf("WSASend() is OK!\n");
	}
}

void Server::SetClientName(int index)
{
	if (strcmp(clientDatas[index].first.c_str(), "") == 0)
		clientDatas[index].first.insert(0, (char*)cBuffer + 4);
}

void Server::ClientAccept()
{
	//clientDatas.resize(10);

	flags = 0;

	SOCKADDR clientAddr;
	int addrSize = 0;

	while (serverHeart)
	{
		/*if (acceptClientSize > 11)
			continue;*/

		clientData temp;
		

		//accept 함수를 이용하여 접속 요청을 수락해준다. 이 함수는 동기화된 방식으로 동작됨
		//동기화된 방식이란 요청이 완료되기 전 까지는 계속 대기 상태에 놓이게 된다.
		//접속 요청이 승인되면 연결된 소켓이 생성 후 리턴된다.
		temp.second.clientSock = accept(clientDatas[0].second.clientSock, &clientAddr, &addrSize);

		//clientDatas[acceptClientSize] = temp;
		//events[acceptClientSize] = WSACreateEvent();
		//ZeroMemory(&temp.second.overlapped, sizeof(WSAOVERLAPPED));
		//temp.second.overlapped.hEvent = events[acceptClientSize];
		//WSAEventSelect(temp.second.clientSock, temp.second.event, FD_READ | FD_CLOSE);

		//temp.second.overlapped.

		//hIocp = CreateIoCompletionPort((HANDLE)temp.second.clientSock, hIocp, , 0);

		clientDatas[acceptClientSize] = temp;

		if (WSARecv(clientDatas[acceptClientSize].second.clientSock, &clientDatas[acceptClientSize].second.m_stRecvOverlappedEx.m_wsaBuf, 1,
			&clientDatas[acceptClientSize].second.recvBytes, &flags, &clientDatas[acceptClientSize].second.overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
				cout << "new Client BackGround Recv Success" << endl;
		}
		else
			cout << "BackGround Recv Failure" << endl;

		/*if (WSASetEvent(events[acceptClientSize]) == false)
			cout << "WSASetEvent 실패" << endl;
		else
			cout << "WSASetEvent 성공" << endl;*/

		//WSAEventSelect(hListen, sEvent, FD_ACCEPT | FD_READ | FD_CLOSE);

		acceptClientSize++;
	}
}

void Server::SendTargetMsg(char* name, char* msg)
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		if (strcmp(clientDatas[i].first.c_str(), name) == 0)
		{
			InsertMsg(msg, (char*)" : ", 3);
			InsertMsg(msg, (char*)"Server", 6);

			send(clientDatas[i].second.clientSock, msg, PACKET_MAX_SIZE, 0);
		}
	}
}

void Server::SendAllMsg(char* msg)
{
	
}

void Server::SendMsg()
{
	char name[PACKET_MAX_SIZE];

	while (true)
	{
		/*ZeroMemory(name, PACKET_MAX_SIZE);
		ZeroMemory(sBuffer, PACKET_MAX_SIZE);*/

		//cin >> name;
		cout << "보낼 대상 입력(전체:all, 서버 종료: exit): ";
		cin.getline(name, PACKET_MAX_SIZE);

		if (strcmp(name, "exit") == 0)
		{
			cout << "서버가 종료되었습니다.\n";
			InsertMsg(sBuffer, (char*)"서버가 종료되었습니다.", 0);

			SendAllMsg(sBuffer);
			break;
		}

		cout <<  "\n보낼 메세지 입력: ";
		cin.getline(sBuffer, PACKET_MAX_SIZE);

		if (strcmp(name, "all") == 0)
		{
			SendAllMsg(sBuffer);
		}
		else
		{
			SendTargetMsg(name, sBuffer);
		}
	}
}

void Server::InsertMsg(char* target, char* str, int size)
{
	memmove(target + size, target, strlen(target));
	memmove(target, str, strlen(str));
}
