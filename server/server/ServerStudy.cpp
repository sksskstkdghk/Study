#include "ServerStudy.h"

Server::Server()
{

}

Server::~Server()
{

}

void Server::Init()
{
	//���� ������ ���� ���� ȣ�����ִ� �Լ�
	//�ش� �Լ��� � �������� ������ Ȱ���� ������ �˷��ش�.
	//1�� ���ڴ� ���� ������, 2�� ���ڴ� WSADATA����ü ������ Ÿ���� ����.
	//1�� ���ڴ� WORDŸ���̹Ƿ� MAKEWORD��ũ�θ� �̿��Ͽ� �Ǽ��� ���������� ��ȯ�����ش�.
	WSAStartup(MAKEWORD(2, 2), &wsaData);

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


	//closesocket(hClient);
	//closesocket(hListen);

	//WSACleanup();
}

void Server::GetMsg(int index)
{
	//�̸� �ޱ�
	recv(clientDatas[index].second.clientSock, cBuffer, PACKET_SIZE, 0);
	clientDatas[index].first = cBuffer;

	while (true)
	{
		ZeroMemory(cBuffer, PACKET_SIZE);
		recv(clientDatas[index].second.clientSock, cBuffer, PACKET_SIZE, 0);

		char* temp = (char*)clientDatas[index].first.c_str();
		InsertMsg(cBuffer, (char*)" : ", 3);
		InsertMsg(cBuffer, temp, strlen(temp));
		cout << cBuffer << endl;

		for (int i = 0; i < clientDatas.size(); i++)
		{
			if (strcmp(clientDatas[i].first.c_str(), clientDatas[index].first.c_str()) != 0)
			{
				send(clientDatas[i].second.clientSock, cBuffer, PACKET_SIZE, 0);
			}
		}
	}
}

void Server::ClientAccept()
{
	int index = 0;

	while (true)
	{
		clientDatas.push_back(clientData("", CLIENT()));
		//accept �Լ��� �̿��Ͽ� ���� ��û�� �������ش�. �� �Լ��� ����ȭ�� ������� ���۵�
		//����ȭ�� ����̶� ��û�� �Ϸ�Ǳ� �� ������ ��� ��� ���¿� ���̰� �ȴ�.
		//���� ��û�� ���εǸ� ����� ������ ���� �� ���ϵȴ�.
		clientDatas[index].second.clientSock = accept(hListen, (SOCKADDR*)&clientDatas[index].second.clientAddr, &clientDatas[index].second.clientSize);

		test = thread(&Server::GetMsg, this, index);
		test.detach();

		index++;
	}
}

void Server::SendTargetMsg(char* name, char* msg)
{
	for (int i = 0; i < clientDatas.size(); i++)
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
	for (int i = 0; i < clientDatas.size(); i++)
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
		cin.getline(name, PACKET_SIZE);
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

void Server::End()
{
	closesocket(hListen);

	WSACleanup();
}
