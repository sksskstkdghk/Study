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
#define PACKET_MAX_SIZE 1024
#define CLIENT_MAX_SIZE 10 + 1

using namespace std;

struct CLIENT
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr = {};
	int addrSize = sizeof clientAddr;
	//DWORD dwError = 0L;
	DWORD recvBytes, sendBytes;
	WSAOVERLAPPED overlapped;

	//CLIENT() {}
};

struct TEST01
{
	char str[100] = {};
	int index;
	float flo;

	TEST01() { memcpy_s(str, 100, "asdf", 4); str[98] = '0'; }
};

struct TEST02
{
	char str[100] = {};
	INT64 index;
	double dou;

	TEST02() {}
};

class Server
{
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;
	WSABUF dataBuf;

	char cBuffer[PACKET_MAX_SIZE] = {};	//클라이언트에서 받아올 메세지
	char sBuffer[PACKET_MAX_SIZE] = {};	//서버에서 보내는 메세지

	typedef pair<string, CLIENT> clientData;

	//클라 데이터 배열(0번 째는 서버가 들어간다.)
	clientData clientDatas[CLIENT_MAX_SIZE] = {};
	WSAEVENT events[CLIENT_MAX_SIZE] = {};
	int acceptClientSize;
	DWORD flags;

	//tcp_keepalive tcpkl;

	//해당 클라와 연결된 이벤트 핸들
	//WSAEVENT events[CLIENT_MAX_SIZE];
	//WSABUF dataBUF;
	//WSANETWORKEVENTS netEvent;	//클라에서 받아온 이벤트 메세지를 받을 이벤트

	BOOL optval;
	bool serverHeart;	//서버가 살아있는지 죽었는지

public:
	Server();
	~Server();

	void Init();
	void ServerClose();	//서버 내 모든 클라이언트 접속 종료(서버는 제외)

	void ClientAccept();

	void EventProcess(DWORD index, int error);
	void EventProcess();
	void GetEvent();

	void CloseEvent(int index);
	void RecvBuffer();
	void SendBuffer();
	void SetClientName(int index);

	void SendTargetMsg(char* name, char* msg);
	void SendAllMsg(char* msg);
	void SendMsg();

	void InsertMsg(char* target, char* str, int size);

	//void CALLBACK CompRoutine();
};

