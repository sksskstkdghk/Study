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
	//���� �ʱ�ȭ ������ �����ϱ����� ����ü
	WSADATA wsaData;
	WSABUF dataBuf;

	char cBuffer[PACKET_MAX_SIZE] = {};	//Ŭ���̾�Ʈ���� �޾ƿ� �޼���
	char sBuffer[PACKET_MAX_SIZE] = {};	//�������� ������ �޼���

	typedef pair<string, CLIENT> clientData;

	//Ŭ�� ������ �迭(0�� °�� ������ ����.)
	clientData clientDatas[CLIENT_MAX_SIZE] = {};
	WSAEVENT events[CLIENT_MAX_SIZE] = {};
	int acceptClientSize;
	DWORD flags;

	//tcp_keepalive tcpkl;

	//�ش� Ŭ��� ����� �̺�Ʈ �ڵ�
	//WSAEVENT events[CLIENT_MAX_SIZE];
	//WSABUF dataBUF;
	//WSANETWORKEVENTS netEvent;	//Ŭ�󿡼� �޾ƿ� �̺�Ʈ �޼����� ���� �̺�Ʈ

	BOOL optval;
	bool serverHeart;	//������ ����ִ��� �׾�����

public:
	Server();
	~Server();

	void Init();
	void ServerClose();	//���� �� ��� Ŭ���̾�Ʈ ���� ����(������ ����)

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

