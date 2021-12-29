#pragma once

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>
#include <Windows.h>
#include <utility>
#include <pthread.h>
#include <mstcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5293
#define PACKET_SIZE 1024

using namespace std;

struct CLIENT
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr = {};
	int addrSize = sizeof clientAddr;
	DWORD dwError = 0L;

	//CLIENT() {}
};

struct TEST
{
	string str;
	int index;
	float flo;

	TEST() {}
};

class Server
{
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;

	char cBuffer[PACKET_SIZE] = {};	//클라이언트에서 받아올 메세지
	char sBuffer[PACKET_SIZE] = {};	//서버에서 보내는 메세지

	thread test;

	typedef pair<string, CLIENT> clientData;

	WSAEVENT events[10 + 1];	//해당 클라와 연결된 이벤트 핸들
	clientData clientDatas[10 + 1] = {};	//클라 데이터 배열(0번 째는 서버가 들어간다.)
	int acceptClientSize;

	tcp_keepalive tcpkl;

	WSANETWORKEVENTS netEvent;	//클라에서 받아온 이벤트 메세지를 받을 이벤트

	BOOL optval;
	bool serverHeart;	//서버가 살아있는지 죽었는지

public:
	Server();
	~Server();

	void Init();
	void ServerClose();

	void ClientAccept();

	void EventPorcess();

	void CloseEvent(int index);
	void ReadEvent(int index);

	void SendTargetMsg(char* name, char* msg);
	void SendAllMsg(char* msg);
	void SendMsg();

	void InsertMsg(char* target, char* str, int size);
};

