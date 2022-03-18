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
	SOCKET			clientSock;			//Cliet와 연결되는 소켓
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O작업을 위한 변수

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

//WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O구조체
	SOCKET		m_socketClient;			//클라이언트 소켓(사용 안하시는데 들어있다 왜지??)
	WSABUF		m_wsaBuf;				//Overlapped I/O작업 버퍼
	char		m_szBuf[PACKET_MAX_SIZE]; //데이터 버퍼
	IOOperation m_eOperation;			//작업 동작 종류
};

//클라이언트 정보를 담기위한 구조체
struct stClientInfo
{
	SOCKET			m_socketClient;			//Cliet와 연결되는 소켓
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O작업을 위한 변수

	stClientInfo()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};

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
	//소켓 초기화 정보를 저장하기위한 구조체
	WSADATA wsaData;

	WSABUF recvDataBuf, sendDataBuf;

	char cBuffer[PACKET_MAX_SIZE] = {};	//클라이언트에서 데이터를 받아와 덮어쓰는 버퍼
	char* sBuffer;	//클라에서 받아온 메세지를 다시 보내는 버퍼
	int recvCount = 0;	//각 클라로부터 받아온 데이터의 갯수
	int sendCount = 0;

	typedef pair<string, CLIENT> clientData;

	//클라 데이터 배열(0번 째는 서버가 들어간다.)
	clientData clientDatas[CLIENT_MAX_SIZE] = {};
	//WSAEVENT events[CLIENT_MAX_SIZE] = {};
	int acceptClientSize;
	DWORD flags;

	ofstream stream;

	HANDLE hIocp;

	//tcp_keepalive tcpkl;

	//해당 클라와 연결된 이벤트 핸들
	//WSAEVENT events[CLIENT_MAX_SIZE];
	//WSABUF dataBUF;
	//WSANETWORKEVENTS netEvent;	//클라에서 받아온 이벤트 메세지를 받을 이벤트

	BOOL optval;
	bool serverHeart;	//서버가 살아있는지 죽었는지

public:
	Server();
	~Server();

	void Init();
	void ServerClose();	//서버 내 모든 클라이언트 접속 종료(서버는 제외)

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

