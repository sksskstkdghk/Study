#include "ServerStudy.h"

Server::Server()
{

}

Server::~Server()
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		//WSACloseEvent(events[i]);
	}

	serverHeart = false;
	ZeroMemory(clientDatas, sizeof(clientData) * acceptClientSize);
	//ZeroMemory(events, sizeof(WSAEVENT) * acceptClientSize);
}

void Server::Init()
{
	acceptClientSize = 1;	//������ ��ü Ŭ���̾�Ʈ ��(0 ��°�� ������ ���⿡ ��ǻ� ũ��� size - 1)
	serverHeart = true;

	//���� ������ ���� ���� ȣ�����ִ� �Լ�
	//�ش� �Լ��� � �������� ������ Ȱ���� ������ �˷��ش�.
	//1�� ���ڴ� ���� ������, 2�� ���ڴ� WSADATA����ü ������ Ÿ���� ����.
	//1�� ���ڴ� WORDŸ���̹Ƿ� MAKEWORD��ũ�θ� �̿��Ͽ� �Ǽ��� ���������� ��ȯ�����ش�.
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//������ ������ �ڵ�
	SOCKET hListen;

	SOCKADDR_IN tListenAddr = {};

	//tcp�������� ���� ����
	//tcp������ ũ�� 2������ ����
	//1. �ٸ� ��ǻ�ͷκ��� ������ ���� ���� ��û�� �������ִ� ����
	//2. �ٸ� ��ǻ�Ϳ� ����� ����
	//��ǻ�ʹ� IP�ּҶ�� ������ �̿��Ͽ� �ش� ��ǻ���� �ּ� ������ ������.
	//IP�ּҴ� ũ�� 2������ ������ �ȴ�. IVP4, IPV6
	//���� 32bit�ּ� ü��, 16byte�ּ� ü��� ����
	//PF_INET�� ����ϸ� IPV4 Ÿ���� ����ϰڴٴ� �ǹ�
	//SOCK_STREAM�� �־��ָ� ���������� ������ ����ڴٴ� ��
	//3�� ° ���ڴ� prorocol�� ����. protocol�� ��űԾ��� ����
	//IPPROTO_TCP�� TCP�� ����ϰڴٰ� ������ �ִ°�(0�� �־ �ڵ����� TCP�� �Ҵ�����)
	//hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	hListen = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, NULL, WSA_FLAG_OVERLAPPED);

	//optval = true;
	//setsockopt(hListen, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optval, sizeof(BOOL));

	tListenAddr.sin_family = AF_INET;

	//��Ʈ��ȣ ����(2byte��)
	//htons : host to network short�� ����
	//�ش� �Լ��� ��ġ�� �ݵ�� �򿣵�� ������� �����͸� ��ȯ�Ͽ� �����Ѵ�.
	tListenAddr.sin_port = htons(PORT);

	//������ ���� ���۵Ǵ� ��ǻ���� IP�ּҷ� ����
	//INADDR_ANY�� �־��ָ� ���� ��ǻ���� IP�ּҷ� ������
	//s_addr�� ipv4 internet address�� �ǹ��Ѵ�.
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind �Լ��� ���Ͽ� �ּ������� ����
	//�� lsiten������ ������ ���ӽ��θ� ����
	//������ ������ �ּ� ������ WinSock2.h�� ���ǵ� bind �Լ��� �̿��Ͽ� ���Ͽ� �����ش�.
	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(SOCKADDR_IN));

	//listen�Լ��� ������ �����ϴ� ���·� ������ ���¸� �����Ѵ�. �� ������ ���� ��� ���·� �������
	//SOMAXCONN�� �ѹ��� ��û ������ �ִ� ���ӽ��� ���� �ǹ�(int �ִ밪 21��~~)
	listen(hListen, SOMAXCONN);

	//CreateIoCompletionPort = Ŀ�� ��ü�� ����� �ִ� �Լ�
	//IOCP��ü�� ���� �� �ڵ� �Ķ���Ϳ� listen������ �־��ִ°� �ƴ� INVALID_HANDLE_VALUE�� �־���� �Ѵ�.
	//������ �Ű������� 0�̶�� ���� ��ǻ�Ϳ� �����ϴ� ��� cpu�ھ Ǯ�� Ȱ���ϰڴٴ� ��
	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//cBuffer = new char[(PACKET_MAX_SIZE + 4) * 10];
	//cBuffer = new unsigned char[PACKET_MAX_SIZE];
	sBuffer = new char[(PACKET_MAX_SIZE + 4) * 10];

	//ZeroMemory(cBuffer, (PACKET_MAX_SIZE + 4) * 10);
	ZeroMemory(cBuffer, PACKET_MAX_SIZE);
	ZeroMemory(sBuffer, (PACKET_MAX_SIZE + 4) * 10);

	//recvDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
	recvDataBuf.len = PACKET_MAX_SIZE;
	recvDataBuf.buf = cBuffer;

	sendDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
	sendDataBuf.buf = sBuffer;

	clientDatas[0].first = "server";
	clientDatas[0].second.clientSock = hListen;
	//clientDatas[0].second.clientAddr = tListenAddr;

	//events[0] = WSACreateEvent();

	/*if (WSASetEvent(events[0]) == false)
		cout << "WSASetEvent ����" << endl;
	else
		cout << "WSASetEvent ����" << endl;*/

	//WSASetEvent(events[0]);
	//WSAEventSelect(hListen, events[0], FD_ACCEPT);
}

//���� �� ��� Ŭ���̾�Ʈ ���� ����(������ ����)
void Server::ServerClose()
{
	for (int i = 1; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		//WSACloseEvent(events[i]);
	}

	ZeroMemory(clientDatas + 1, sizeof(clientData) * (acceptClientSize - 1));
	//ZeroMemory(events + 1, sizeof(WSAEVENT) * (acceptClientSize - 1));
}

void Server::EventProcess(DWORD index, int error)
{
	//if (error == SOCKET_ERROR)
	//{
	//	//cout << clientDatas[index].first << "���� ������ �߻��Ͽ����ϴ�." << endl;

	//	cout << index << "�� ������ ����: " << WSAGetLastError() << endl;
	//	return;
	//}

	//if (netEvent.lNetworkEvents == FD_ACCEPT)
	//{
	//	ClientAccept();
	//}
	//else if (netEvent.lNetworkEvents == FD_CLOSE)
	//{
	//	CloseEvent(index);
	//}
	//else if (netEvent.lNetworkEvents == FD_READ)
	//{
	//	ReadEvent(index);
	//}
}

void Server::EventProcess()
{
	
}

void Server::GetEvent()
{
	while (serverHeart)
	{
		ZeroMemory(cBuffer, PACKET_MAX_SIZE);

		//DWORD index = WSAWaitForMultipleEvents(acceptClientSize, &clientDatas[index].second.event, false, 1000, false);
		//int error = WSAEnumNetworkEvents(clientDatas[index].second.clientSock, events[index], &netEvent);


		//if (index >= CLIENT_MAX_SIZE)
			//continue;

		//thread(&Server::EventProcess, this, index, error).detach();
		//EventProcess(index, error);
	}

	if (serverHeart == false)
	{
		cout << "������ ����" << endl;
	}
}

void Server::CloseEvent(int index)
{
	cout << clientDatas[index].first << "���� �����Ͽ����ϴ�." << endl;

	ZeroMemory(sBuffer, PACKET_MAX_SIZE);

	InsertMsg(sBuffer, (char*)"���� �����Ͽ����ϴ�.", 20);
	InsertMsg(sBuffer, (char*)clientDatas[index].first.c_str(), clientDatas[index].first.size());

	SendAllMsg(sBuffer);
	
	closesocket(clientDatas[index].second.clientSock);
	//WSACloseEvent(events[index]);

	acceptClientSize--;

	if (index < acceptClientSize)
		return;

	clientDatas[index].first = clientDatas[acceptClientSize].first;
	clientDatas[index].second = clientDatas[acceptClientSize].second;
	//events[index] = events[acceptClientSize];
}

void Server::RecvBuffer()
{
	int index = 1;
	BOOL bResult = TRUE;

	//filebuf = new unsigned char[TEST_FILE_SIZE];

	while (serverHeart)
	{
		//if (acceptClientSize < 2)
		//	continue;

		////ZeroMemory(recvDataBuf.buf, PACKET_MAX_SIZE);

		//if ((index = WSAWaitForMultipleEvents(acceptClientSize, events, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		//{
		//	cout << "WSAWaitForMultipleEvents() failed " << WSAGetLastError() << endl;
		//	continue;
		//}
		//else
		//	cout << "WSAWaitForMultipleEvents() is OK" << endl;

		//if (index == WSA_WAIT_EVENT_0)
		//{
		//	WSAResetEvent(events[WSA_WAIT_EVENT_0]);
		//	cout << "�̰� ������!" << endl;
		//	continue;
		//}

		//bResult = WSAResetEvent(events[index - WSA_WAIT_EVENT_0]);

		//if (bResult == FALSE)
		//	cout << "WSAResetEvent failed with error = " << WSAGetLastError() << endl;
		//

		//stOverlappedEx* temp = &clientDatas[index - WSA_WAIT_EVENT_0].second.m_stRecvOverlappedEx;
		//string name = clientDatas[index - WSA_WAIT_EVENT_0].first;

		//if (WSAGetOverlappedResult(temp->clientSock, &temp->overlapped, &temp->recvBytes, FALSE, &flags) == TRUE)
		//{
		//	if (temp->recvBytes <= 0)
		//		cout << "no data here" << endl;
		//	else
		//	{
		//		if (temp->recvBytes < PACKET_MAX_SIZE)
		//		{
		//			cout << "�� �۰� ��" << endl;
		//		}

		//		stream.open("test.txt", ios::app | ios::binary);

		//		stream.write(cBuffer, temp->recvBytes);

		//		//memcpy_s(filebuf + temp->writeBytes, temp->recvBytes, recvDataBuf.buf, temp->recvBytes);

		//		temp->writeBytes += temp->recvBytes;

		//		cout << "�ٿ���� �� ũ��: " << temp->writeBytes << endl;
		//		stream.close();

		//		//Sleep(100);
		//	}


		//	/*if (temp->recvBytes <= 0)
		//		cout << "no data here" << endl;
		//	else
		//	{
		//		int size;

		//		memcpy_s(&size, 4, recvDataBuf.buf, 4);

		//		cout << size << " " << recvDataBuf.buf + 4 << endl;

		//		if (name.size() <= 0)
		//		{
		//			SetClientName(index);
		//			name = clientDatas[index].first;
		//		}
		//		else
		//			WriteData(*temp, name);
		//	}*/

		//	

		//	//stream << recvDataBuf.buf;

		//	flags = 0;
		//	ZeroMemory(&temp->overlapped, sizeof(WSAOVERLAPPED));
		//	temp->overlapped.hEvent = events[index];

		//	/*if(TEST_FILE_SIZE - temp->writeBytes > PACKET_MAX_SIZE)
		//		recvDataBuf.len = PACKET_MAX_SIZE;
		//	else
		//		recvDataBuf.len = TEST_FILE_SIZE - temp->writeBytes;*/

		//	recvDataBuf.buf = (char*)cBuffer;

		//	ZeroMemory(cBuffer, PACKET_MAX_SIZE);

		//	if (WSARecv(temp->clientSock, &recvDataBuf, 1, &temp->recvBytes, &flags, &temp->overlapped, NULL) == SOCKET_ERROR)
		//	{
		//		if (WSAGetLastError() == WSA_IO_PENDING)
		//		{
		//			/*recvDataBuf.len = (PACKET_MAX_SIZE + 4) * 10;
		//			recvDataBuf.buf = sBuffer;*/

		//			cout << name << " BackGround Recv Success" << endl;
		//		}
		//	}
		//	else
		//		cout << "BackGround Recv Failure" << endl;
		//}
	}

	//recv(clientDatas[index].second.clientSock, cBuffer, PACKET_MAX_SIZE, 0);

	////�̸� ����
	//if (strcmp(clientDatas[index].first.c_str(), "") == 0)
	//{
	//	clientDatas[index].first.insert(0, cBuffer);
	//	return;
	//}

	//char* temp = (char*)clientDatas[index].first.c_str();
	//InsertMsg(cBuffer, (char*)" : ", 3);
	//InsertMsg(cBuffer, temp, strlen(temp));
	//cout << cBuffer << endl;

	//for (int i = 0; i < acceptClientSize; i++)
	//{
	//	if (strcmp(clientDatas[i].first.c_str(), clientDatas[index].first.c_str()) != 0)
	//	{
	//		send(clientDatas[i].second.clientSock, cBuffer, PACKET_MAX_SIZE, 0);
	//	}
	//}
}

void Server::IOCPRecvBuffer()
{
	int index = 1;
	BOOL bResult = TRUE;

	while (serverHeart)
	{
		if (acceptClientSize < 2)
			continue;

		stOverlappedEx* temp = &clientDatas[index - WSA_WAIT_EVENT_0].second.m_stRecvOverlappedEx;
		//string name = clientDatas[index - WSA_WAIT_EVENT_0].first;

	}
}

void Server::WriteData(CLIENT client, const string name)
{
	int leftCount = 0;

	for (int i =1; i <= recvCount; i++)
	{
		int a;

		memcpy_s(&a, 4, recvDataBuf.buf + (i * (PACKET_MAX_SIZE + 4)), 4);

		if (a - PACKET_MAX_SIZE > 0)
			leftCount++;

		//if(cbuffer)
	}
}

void Server::SendBuffer()
{
	int index = 1;

	DWORD sendBytes;

	while (serverHeart)
	{
		if (acceptClientSize < 2)
			continue;

		if (index > acceptClientSize)
			index = 1;

		stOverlappedEx* temp = &clientDatas[index].second.m_stSendOverlappedEx;

		if (WSASend(clientDatas[index].second.clientSock, &temp->m_wsaBuf, 1,
			&sendBytes, 0, (LPWSAOVERLAPPED)&temp, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSASend() failed with error %d\n", WSAGetLastError());
				continue;
			}
		}
		else
			printf("WSASend() is OK!\n");
	}
}

void Server::SetClientName(int index)
{
	if (strcmp(clientDatas[index].first.c_str(), "") == 0)
		clientDatas[index].first.insert(0, (char*)cBuffer + 4);
}

void Server::ClientAccept()
{
	//clientDatas.resize(10);

	flags = 0;

	SOCKADDR clientAddr;
	int addrSize = 0;

	while (serverHeart)
	{
		/*if (acceptClientSize > 11)
			continue;*/

		clientData temp;
		

		//accept �Լ��� �̿��Ͽ� ���� ��û�� �������ش�. �� �Լ��� ����ȭ�� ������� ���۵�
		//����ȭ�� ����̶� ��û�� �Ϸ�Ǳ� �� ������ ��� ��� ���¿� ���̰� �ȴ�.
		//���� ��û�� ���εǸ� ����� ������ ���� �� ���ϵȴ�.
		temp.second.clientSock = accept(clientDatas[0].second.clientSock, &clientAddr, &addrSize);

		//clientDatas[acceptClientSize] = temp;
		//events[acceptClientSize] = WSACreateEvent();
		//ZeroMemory(&temp.second.overlapped, sizeof(WSAOVERLAPPED));
		//temp.second.overlapped.hEvent = events[acceptClientSize];
		//WSAEventSelect(temp.second.clientSock, temp.second.event, FD_READ | FD_CLOSE);

		//temp.second.overlapped.

		//hIocp = CreateIoCompletionPort((HANDLE)temp.second.clientSock, hIocp, , 0);

		clientDatas[acceptClientSize] = temp;

		if (WSARecv(clientDatas[acceptClientSize].second.clientSock, &clientDatas[acceptClientSize].second.m_stRecvOverlappedEx.m_wsaBuf, 1,
			&clientDatas[acceptClientSize].second.recvBytes, &flags, &clientDatas[acceptClientSize].second.overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
				cout << "new Client BackGround Recv Success" << endl;
		}
		else
			cout << "BackGround Recv Failure" << endl;

		/*if (WSASetEvent(events[acceptClientSize]) == false)
			cout << "WSASetEvent ����" << endl;
		else
			cout << "WSASetEvent ����" << endl;*/

		//WSAEventSelect(hListen, sEvent, FD_ACCEPT | FD_READ | FD_CLOSE);

		acceptClientSize++;
	}
}

void Server::SendTargetMsg(char* name, char* msg)
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		if (strcmp(clientDatas[i].first.c_str(), name) == 0)
		{
			InsertMsg(msg, (char*)" : ", 3);
			InsertMsg(msg, (char*)"Server", 6);

			send(clientDatas[i].second.clientSock, msg, PACKET_MAX_SIZE, 0);
		}
	}
}

void Server::SendAllMsg(char* msg)
{
	
}

void Server::SendMsg()
{
	char name[PACKET_MAX_SIZE];

	while (true)
	{
		/*ZeroMemory(name, PACKET_MAX_SIZE);
		ZeroMemory(sBuffer, PACKET_MAX_SIZE);*/

		//cin >> name;
		cout << "���� ��� �Է�(��ü:all, ���� ����: exit): ";
		cin.getline(name, PACKET_MAX_SIZE);

		if (strcmp(name, "exit") == 0)
		{
			cout << "������ ����Ǿ����ϴ�.\n";
			InsertMsg(sBuffer, (char*)"������ ����Ǿ����ϴ�.", 0);

			SendAllMsg(sBuffer);
			break;
		}

		cout <<  "\n���� �޼��� �Է�: ";
		cin.getline(sBuffer, PACKET_MAX_SIZE);

		if (strcmp(name, "all") == 0)
		{
			SendAllMsg(sBuffer);
		}
		else
		{
			SendTargetMsg(name, sBuffer);
		}
	}
}

void Server::InsertMsg(char* target, char* str, int size)
{
	memmove(target + size, target, strlen(target));
	memmove(target, str, strlen(str));
}
