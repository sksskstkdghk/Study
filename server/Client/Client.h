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
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;

	//������ ������ �ڵ�
	SOCKET clientSock;

	SOCKADDR_IN clientAddr = {};

	char sBuffer[PACKET_SIZE] = {};	//�������� �޾ƿ� �޼���
	char cMsg[PACKET_SIZE] = {};	//������ ���� �޼���

public:
	Client();
	~Client();

	void Init();

	void SendMsg();
	void GetMsg();
	void End();
};

