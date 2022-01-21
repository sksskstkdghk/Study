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

	char sBuffer[PACKET_SIZE] = {};	//�������� �޾ƿ� �޼���
	char cBuffer[PACKET_SIZE] = {};	//������ ���� �޼���
	DWORD recvBytes, sendBytes;
	//DWORD flag;
	//WSABUF dataBuf;
	WSAOVERLAPPED overlapped;

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

