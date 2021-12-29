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
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;

	char cBuffer[PACKET_SIZE] = {};	//Ŭ���̾�Ʈ���� �޾ƿ� �޼���
	char sBuffer[PACKET_SIZE] = {};	//�������� ������ �޼���

	thread test;

	typedef pair<string, CLIENT> clientData;

	WSAEVENT events[10 + 1];	//�ش� Ŭ��� ����� �̺�Ʈ �ڵ�
	clientData clientDatas[10 + 1] = {};	//Ŭ�� ������ �迭(0�� °�� ������ ����.)
	int acceptClientSize;

	tcp_keepalive tcpkl;

	WSANETWORKEVENTS netEvent;	//Ŭ�󿡼� �޾ƿ� �̺�Ʈ �޼����� ���� �̺�Ʈ

	BOOL optval;
	bool serverHeart;	//������ ����ִ��� �׾�����

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

