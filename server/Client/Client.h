#pragma once

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <pthread.h>
#include <mstcpip.h>

#pragma comment(lib, "ws2_32")

#define PORT 5293
#define PACKET_SIZE 1024
#define SERVER_IP "220.86.81.119"

using namespace std;

struct TEST
{
	string str;
	int index;
	float flo;

	TEST() {}
};

class Client
{
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;

	//일종의 윈도우 핸들
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE] = {};	//서버에서 받아올 메세지
	char cMsg[PACKET_SIZE] = {};	//서버로 보낼 메세지

	tcp_keepalive tcpkl;
	DWORD dwError = 0L;

public:
	Client();
	~Client();

	void Init();

	void SendMsg();
	void GetMsg();
	void End();
};

