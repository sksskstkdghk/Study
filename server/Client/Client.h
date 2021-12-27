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
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;

	//������ ������ �ڵ�
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE] = {};	//�������� �޾ƿ� �޼���
	char cMsg[PACKET_SIZE] = {};	//������ ���� �޼���

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

