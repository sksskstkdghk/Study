#pragma once

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32")

#define PORT 5293
#define PACKET_SIZE 1024
#define SERVER_IP "220.86.76.116"

using namespace std;

class Client
{
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;

	//일종의 윈도우 핸들
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE] = {};	//서버에서 받아올 메세지
	char cMsg[PACKET_SIZE] = {};	//서버로 보낼 메세지

public:
	Client();
	~Client();

	void Init();

	void SendMsg();
	void GetMsg();
	void End();
};

