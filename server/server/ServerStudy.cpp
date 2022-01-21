#include "ServerStudy.h"

Server::Server()
{

}

Server::~Server()
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		WSACloseEvent(events[i]);
	}

	serverHeart = false;
	ZeroMemory(clientDatas, sizeof(clientData) * acceptClientSize);
	ZeroMemory(events, sizeof(WSAEVENT) * acceptClientSize);
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
	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));

	//listen함수는 연결을 수신하는 상태로 소켓의 상태를 변경한다. 즉 소켓을 접속 대기 상태로 만들어줌
	//SOMAXCONN은 한번에 요청 가능한 최대 접속승인 수를 의미(int 최대값 21억~~)
	listen(hListen, SOMAXCONN);

	ZeroMemory(cBuffer, PACKET_MAX_SIZE);
	ZeroMemory(sBuffer, PACKET_MAX_SIZE);

	//dataBUF.len = PACKET_MAX_SIZE;
	//dataBUF.buf = sBuffer;

	clientDatas[0].first = "server";
	clientDatas[0].second.clientSock = hListen;
	clientDatas[0].second.clientAddr = tListenAddr;

	events[0] = WSACreateEvent();
	WSASetEvent(events[0]);
	//WSAEventSelect(hListen, events[0], FD_ACCEPT);
}

//서버 내 모든 클라이언트 접속 종료(서버는 제외)
void Server::ServerClose()
{
	for (int i = 1; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		WSACloseEvent(events[i]);
	}

	ZeroMemory(clientDatas + 1, sizeof(clientData) * (acceptClientSize - 1));
	ZeroMemory(events + 1, sizeof(WSAEVENT) * (acceptClientSize - 1));
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
	WSACloseEvent(events[index]);

	acceptClientSize--;

	if (index < acceptClientSize)
		return;

	clientDatas[index].first = clientDatas[acceptClientSize].first;
	clientDatas[index].second = clientDatas[acceptClientSize].second;
	events[index] = events[acceptClientSize];
}

void Server::RecvBuffer()
{
	int index = 1;

	while (serverHeart)
	{
		if (acceptClientSize < 2)
			continue;

		if (index >= acceptClientSize)
			index = 1;

		CLIENT* temp = &clientDatas[index].second;
		string name = clientDatas[index].first;

		dataBuf.buf = cBuffer;
		dataBuf.len = PACKET_MAX_SIZE;

		if (WSARecv(temp->clientSock, &dataBuf, 1, &temp->recvBytes, &flags, &temp->overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				cout << name << " BackGround Recv Success" << endl;

				index = WSAWaitForMultipleEvents(acceptClientSize, events, TRUE, WSA_INFINITE, FALSE);

				if (index == WSA_WAIT_FAILED)
				{
					cout << "Error - Wait Filure" << endl;
					continue;
				}

				if (index == WSA_WAIT_EVENT_0) continue;

				if (WSAGetOverlappedResult(temp->clientSock, &temp->overlapped, &temp->recvBytes, FALSE, &flags) == TRUE)
				{

				}
			}
			else
			{
				cout << "BackGround Recv Failure" << endl;

				
			}
		}

		ZeroMemory(dataBuf.buf, PACKET_MAX_SIZE);
		
		index++;

		//Sleep(1000);
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

void Server::SendBuffer()
{

}

void Server::SetClientName(int index)
{
	if (strcmp(clientDatas[index].first.c_str(), "") == 0)
		clientDatas[index].first.insert(0, cBuffer);
}

void Server::ClientAccept()
{
	//clientDatas.resize(10);

	flags = 0;

	while (serverHeart)
	{
		clientData temp;

		//accept 함수를 이용하여 접속 요청을 수락해준다. 이 함수는 동기화된 방식으로 동작됨
		//동기화된 방식이란 요청이 완료되기 전 까지는 계속 대기 상태에 놓이게 된다.
		//접속 요청이 승인되면 연결된 소켓이 생성 후 리턴된다.
		temp.second.clientSock = accept(clientDatas[0].second.clientSock, (SOCKADDR*)&temp.second.clientAddr, &temp.second.addrSize);

		events[acceptClientSize] = WSACreateEvent();
		temp.second.overlapped.hEvent = events[acceptClientSize];
		//WSAEventSelect(temp.second.clientSock, temp.second.event, FD_READ | FD_CLOSE);

		if (WSASetEvent(events[acceptClientSize]) == false)
			cout << "WSASetEvent 실패" << endl;
		else
			cout << "WSASetEvent 성공" << endl;

		clientDatas[acceptClientSize] = temp;

		//WSAEventSelect(hListen, sEvent, FD_ACCEPT | FD_READ | FD_CLOSE);

		//이거 뭐 어케 사용하는건데...
		//tcpkl.onoff = 1;
		//tcpkl.keepalivetime = 5000;	//1초 마다 신호를 보내겠다
		//tcpkl.keepaliveinterval = 5000; //신호를 보낸 후 응답이 없다면 1초마다 재 전송 하겠다.

		//WSAIoctl(clientDatas[index].second.clientSock, SIO_KEEPALIVE_VALS, &tcpkl, sizeof(tcp_keepalive), 0, 0, &clientDatas[index].second.dwError, NULL, NULL);
		//WSAIoctl(hListen, SIO_KEEPALIVE_VALS, &tcpkl, sizeof(tcp_keepalive), 0, 0, &clientDatas[index].second.dwError, NULL, NULL);

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
	//WSASend(clientDatas[0].second.clientSock, &dataBUF, )

	/*for (int i = 0; i < acceptClientSize; i++)
	{
		send(clientDatas[i].second.clientSock, msg, PACKET_MAX_SIZE, 0);
	}*/
}

void Server::SendMsg()
{
	char name[PACKET_MAX_SIZE];

	while (true)
	{
		ZeroMemory(name, PACKET_MAX_SIZE);
		ZeroMemory(sBuffer, PACKET_MAX_SIZE);

		//cin >> name;
		cout << "보낼 대상 입력(전체:all, 서버 종료: exit): ";
		cin.getline(name, PACKET_MAX_SIZE);

		if (strcmp(name, "exit") == 0)
		{
			cout << "서버가 종료되었습니다." << endl;
			InsertMsg(sBuffer, (char*)"서버가 종료되었습니다.", 0);

			SendAllMsg(sBuffer);
			break;
		}

		cout << endl << "보낼 메세지 입력: ";
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
