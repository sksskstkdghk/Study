#pragma once

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>
#include <Windows.h>
#include <utility>
#include <pthread.h>
#include <mstcpip.h>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5293
#define PACKET_MAX_SIZE 1024
#define CLIENT_MAX_SIZE 10
#define MAX_THREAD_SIZE 4

#define TEST_FILE_SIZE 1007616

using namespace std;

struct SocketInfo
{
	OVERLAPPED overlapped;
	SOCKET fd;
	char buf[PACKET_MAX_SIZE];
	int readn;
	int writen;
	WSABUF wsabuf;
};

struct CLIENT
{
	SOCKET			clientSock;			//Cliet�� ����Ǵ� ����
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O�۾��� ���� ����

	CLIENT()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		clientSock = INVALID_SOCKET;
	}

	//SOCKET clientSock;
	//SOCKADDR_IN clientAddr = {};
	//int addrSize = sizeof clientAddr;
	////DWORD dwError = 0L;
	//OVERLAPPED overlapped;

	//DWORD recvBytes = 0, sendBytes = 0, writeBytes = 0;

	//void Init()
	//{
	//	recvBytes = 0;
	//	sendBytes = 0;
	//	writeBytes = 0;
	//}

	//CLIENT() {}
};

enum class IOOperation
{
	RECV,
	SEND
};

//WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O����ü
	SOCKET		m_socketClient;			//Ŭ���̾�Ʈ ����(��� ���Ͻôµ� ����ִ� ����??)
	WSABUF		m_wsaBuf;				//Overlapped I/O�۾� ����
	char		m_szBuf[PACKET_MAX_SIZE]; //������ ����
	IOOperation m_eOperation;			//�۾� ���� ����
};

//Ŭ���̾�Ʈ ������ ������� ����ü
struct stClientInfo
{
	SOCKET			m_socketClient;			//Cliet�� ����Ǵ� ����
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O�۾��� ���� ����

	stClientInfo()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};

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

	WSABUF recvDataBuf, sendDataBuf;

	char cBuffer[PACKET_MAX_SIZE] = {};	//Ŭ���̾�Ʈ���� �����͸� �޾ƿ� ����� ����
	char* sBuffer;	//Ŭ�󿡼� �޾ƿ� �޼����� �ٽ� ������ ����
	int recvCount = 0;	//�� Ŭ��κ��� �޾ƿ� �������� ����
	int sendCount = 0;

	typedef pair<string, CLIENT> clientData;

	//Ŭ�� ������ �迭(0�� °�� ������ ����.)
	clientData clientDatas[CLIENT_MAX_SIZE] = {};
	//WSAEVENT events[CLIENT_MAX_SIZE] = {};
	int acceptClientSize;
	DWORD flags;

	ofstream stream;

	HANDLE hIocp;

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
	void IOCPRecvBuffer();
	void WriteData(CLIENT client, const string name);
	void SendBuffer();
	void SetClientName(int index);

	void SendTargetMsg(char* name, char* msg);
	void SendAllMsg(char* msg);
	void SendMsg();

	void InsertMsg(char* target, char* str, int size);

	//void CALLBACK CompRoutine();
};

