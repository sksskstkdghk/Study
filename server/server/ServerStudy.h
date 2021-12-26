#pragma once

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>
#include <Windows.h>
#include <utility>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5293
#define PACKET_SIZE 1024

using namespace std;

struct CLIENT
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr = {};
	int clientSize = sizeof clientAddr;

	CLIENT() {}
};

typedef pair<string, CLIENT> clientData;

class Server
{
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;

	//������ ������ �ڵ�
	SOCKET hListen;

	SOCKADDR_IN tListenAddr = {};

	char cBuffer[PACKET_SIZE] = {};	//Ŭ���̾�Ʈ���� �޾ƿ� �޼���
	char sBuffer[PACKET_SIZE] = {};	//�������� ������ �޼���

	thread test;

	vector<clientData> clientDatas;

public:
	Server();
	~Server();

	void Init();

	void ClientAccept();

	void GetMsg(int index);

	void SendTargetMsg(char* name, char* msg);
	void SendAllMsg(char* msg);
	void SendMsg();

	void InsertMsg(char* target, char* str, int size);

	void End();
};

