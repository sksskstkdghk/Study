#include "ServerStudy.h"

Server::Server()
{

}

Server::~Server()
{

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
	//IPPROTO_TCP�� TCP�� ����ϰڴٰ� ������ �ִ°�
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

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
	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));

	//listen�Լ��� ������ �����ϴ� ���·� ������ ���¸� �����Ѵ�. �� ������ ���� ��� ���·� �������
	//SOMAXCONN�� �ѹ��� ��û ������ �ִ� ���ӽ��� ���� �ǹ�(int �ִ밪 21��~~)
	listen(hListen, SOMAXCONN);

	clientDatas[0].first = "server";
	clientDatas[0].second.clientSock = hListen;
	clientDatas[0].second.clientAddr = tListenAddr;

	events[0] = WSACreateEvent();
	WSAEventSelect(hListen, events[0], FD_ACCEPT);
}

void Server::ServerClose()
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		closesocket(clientDatas[i].second.clientSock);
		WSACloseEvent(events[i]);
	}

	ZeroMemory(clientDatas, sizeof(clientData) * 11);
	ZeroMemory(events, sizeof(WSAEVENT) * 11);
}

void Server::EventPorcess()
{
	//TEST a;

	//memcpy(&a, cBuffer, sizeof(cBuffer));

	//a = *(TEST*)cBuffer;

	while (serverHeart)
	{
		ZeroMemory(cBuffer, PACKET_SIZE);

		DWORD index = WSAWaitForMultipleEvents(acceptClientSize, events, false, INFINITE, false);
		int error = WSAEnumNetworkEvents(clientDatas[index].second.clientSock, events[index], &netEvent);

		if (netEvent.lNetworkEvents == FD_ACCEPT)
		{
			ClientAccept();
		}
		else if (netEvent.lNetworkEvents == FD_CLOSE)
		{
			CloseEvent(index);
		}
		else if (netEvent.lNetworkEvents == FD_READ)
		{
			ReadEvent(index);
		}

		if (error == SOCKET_ERROR)
		{
			cout << clientDatas[index].first << "���� ������ �߻��Ͽ����ϴ�." << endl;
		}

		/*clientDatas[index].second.dwError = WSAGetLastError();

		if (clientDatas[index].second.dwError == WSAECONNABORTED)
		{
			cout << clientDatas[index].first << "�� ������ ������ϴ�." << endl;
			continue;
		}*/
	}

	if (serverHeart == false)
	{
		cout << "������ ����" << endl;
	}
}

void Server::CloseEvent(int index)
{
	cout << clientDatas[index].first << "���� �����Ͽ����ϴ�." << endl;

	ZeroMemory(sBuffer, PACKET_SIZE);

	InsertMsg(sBuffer, (char*)"���� �����Ͽ����ϴ�.", 20);
	InsertMsg(sBuffer, (char*)clientDatas[index].first.c_str(), clientDatas[index].first.size());

	SendAllMsg(sBuffer);
	
	closesocket(clientDatas[index].second.clientSock);
	WSACloseEvent(events[index]);

	acceptClientSize--;

	if (index < acceptClientSize)
		return;

	clientDatas[index].first = clientDatas[acceptClientSize].first;
	clientDatas[index].second = clientDatas[acceptClientSize].second;
	events[index] = events[acceptClientSize];
}

void Server::ReadEvent(int index)
{
	recv(clientDatas[index].second.clientSock, cBuffer, PACKET_SIZE, 0);

	//�̸� ����
	if (strcmp(clientDatas[index].first.c_str(), "") == 0)
	{
		clientDatas[index].first = cBuffer;
		return;
	}

	char* temp = (char*)clientDatas[index].first.c_str();
	InsertMsg(cBuffer, (char*)" : ", 3);
	InsertMsg(cBuffer, temp, strlen(temp));
	cout << cBuffer << endl;

	for (int i = 0; i < acceptClientSize; i++)
	{
		if (strcmp(clientDatas[i].first.c_str(), clientDatas[index].first.c_str()) != 0)
		{
			send(clientDatas[i].second.clientSock, cBuffer, PACKET_SIZE, 0);
		}
	}
}

void Server::ClientAccept()
{
	//clientDatas.resize(10);

	clientData temp;
	
	//accept �Լ��� �̿��Ͽ� ���� ��û�� �������ش�. �� �Լ��� ����ȭ�� ������� ���۵�
	//����ȭ�� ����̶� ��û�� �Ϸ�Ǳ� �� ������ ��� ��� ���¿� ���̰� �ȴ�.
	//���� ��û�� ���εǸ� ����� ������ ���� �� ���ϵȴ�.
	temp.second.clientSock = accept(clientDatas[0].second.clientSock, (SOCKADDR*)&temp.second.clientAddr, &temp.second.addrSize);
	
	clientDatas[acceptClientSize] = temp;
	
	events[acceptClientSize] = WSACreateEvent();
	WSAEventSelect(clientDatas[acceptClientSize].second.clientSock, events[acceptClientSize], FD_READ | FD_CLOSE);
	
	//WSAEventSelect(hListen, sEvent, FD_ACCEPT | FD_READ | FD_CLOSE);
	
	//�̰� �� ���� ����ϴ°ǵ�...
	//tcpkl.onoff = 1;
	//tcpkl.keepalivetime = 5000;	//1�� ���� ��ȣ�� �����ڴ�
	//tcpkl.keepaliveinterval = 5000; //��ȣ�� ���� �� ������ ���ٸ� 1�ʸ��� �� ���� �ϰڴ�.
	
	//WSAIoctl(clientDatas[index].second.clientSock, SIO_KEEPALIVE_VALS, &tcpkl, sizeof(tcp_keepalive), 0, 0, &clientDatas[index].second.dwError, NULL, NULL);
	//WSAIoctl(hListen, SIO_KEEPALIVE_VALS, &tcpkl, sizeof(tcp_keepalive), 0, 0, &clientDatas[index].second.dwError, NULL, NULL);
	
	//test = thread(&Server::EventPorcess, this, acceptClientSize);
	//test.detach();
	
	acceptClientSize++;
}

void Server::SendTargetMsg(char* name, char* msg)
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		if (strcmp(clientDatas[i].first.c_str(), name) == 0)
		{
			InsertMsg(msg, (char*)" : ", 3);
			InsertMsg(msg, (char*)"Server", 6);

			send(clientDatas[i].second.clientSock, msg, PACKET_SIZE, 0);
		}
	}
}

void Server::SendAllMsg(char* msg)
{
	for (int i = 0; i < acceptClientSize; i++)
	{
		send(clientDatas[i].second.clientSock, msg, PACKET_SIZE, 0);
	}
}

void Server::SendMsg()
{
	char name[PACKET_SIZE];

	while (true)
	{
		ZeroMemory(name, PACKET_SIZE);
		ZeroMemory(sBuffer, PACKET_SIZE);

		//cin >> name;
		cout << "���� ��� �Է�(��ü:all, ���� ����: exit): ";
		cin.getline(name, PACKET_SIZE);

		if (strcmp(name, "exit") == 0)
		{
			cout << "������ ����Ǿ����ϴ�." << endl;
			InsertMsg(sBuffer, (char*)"������ ����Ǿ����ϴ�.", 0);

			SendAllMsg(sBuffer);

			serverHeart = false;
			break;
		}

		cout << endl << "���� �޼��� �Է�: ";
		cin.getline(sBuffer, PACKET_SIZE);

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
