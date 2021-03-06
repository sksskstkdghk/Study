#pragma once

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <pthread.h>
#include <mstcpip.h>
#include <fstream>

#pragma comment(lib, "ws2_32")

#define PORT 5293
#define PACKET_SIZE 1024
#define SERVER_IP "220.86.81.46"

#define TEST_FILE_SIZE 1007616

using namespace std;

//나는 버퍼를 이런 구조체같은 형태로 사용하겠다.
//내부 순서 중요!!
//struct DATA_BUFFER
//{
//	int bufSize = 0;
//	char buffer[PACKET_MAX_SIZE] = {};
//
//	DATA_BUFFER() { ZeroMemory(buffer, PACKET_MAX_SIZE); }
//};

struct TEST01
{
	char str[100] = {};
	int index;
	float flo;

	TEST01() {}
};

struct TEST02
{
	char str[100] = {};
	INT64 index;
	double dou;

	TEST02() {}
};

class Client
{
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;

	//일종의 윈도우 핸들
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE + 4] = {};	//서버에서 받아올 메세지
	//char cBuffer[PACKET_SIZE + 4] = {};	//서버로 보낼 메세지
	unsigned char* cBuffer;
	DWORD recvBytes, sendBytes;
	DWORD flag;
	WSABUF recvDataBuf, sendDataBuf;
	WSAOVERLAPPED overlapped;

	ifstream stream;
	ofstream test;

	//tcp_keepalive tcpkl;
	int dwError;

	WSAEVENT cEvent;
	//WSANETWORKEVENTS netEvent;

	bool clientHeart;

public:
	Client();
	~Client();

	void Init();

	void SendMsg();
	void GetMsg();
	void Close();
};

