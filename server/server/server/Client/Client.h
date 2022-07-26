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

//���� ���۸� �̷� ����ü���� ���·� ����ϰڴ�.
//���� ���� �߿�!!
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
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;

	//������ ������ �ڵ�
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE + 4] = {};	//�������� �޾ƿ� �޼���
	//char cBuffer[PACKET_SIZE + 4] = {};	//������ ���� �޼���
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

