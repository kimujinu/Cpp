#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // ��Ƽ����Ʈ ���� ���� define
#pragma comment(lib, "ws2_32.lib") // winsock2 ����� ���� �ڸ�Ʈ �߰�

#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
#include "CommonClass.h"

using namespace std;

#define MAX_BUFFER 1024
#define SERVER_PORT 30002

struct stSOCKETINFO // IOCP ���� ����ü
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuf;
	SOCKET socket;
	char messageBuffer[MAX_BUFFER];
	int recvBytes;
	int sendBytes;
};

struct FuncProcess // ��Ŷ ó�� �Լ� ������
{
	void(*funcProcessPacket)(stringstream& RecvStream, stSOCKETINFO* pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};

class IocpBase
{
public:
	IocpBase();
	virtual ~IocpBase();

	// ���� ��� �� ���� ���� ����
	bool Initialize();
	// ���� ����
	virtual void StartServer();
	// �۾� ������ ����
	virtual bool CreateWorkThread();
	// �۾� ������ 
	virtual void WorkerThread();
	// Ŭ���̾�Ʈ���� �۽�
	virtual void Send(stSOCKETINFO* pSocket);
	// Ŭ���̾�Ʈ ���� ���
	virtual void Recv(stSOCKETINFO* pSocket);


protected:
	stSOCKETINFO* SocketInfo; // ���� ����
	SOCKET ListenSocket; // ���� ���� ����
	HANDLE hIOCP; // IOCP ��ü �ڵ�
	bool bAccept; // ��û ���� �÷���
	bool bWorkerThread; // �۾� ������ ���� �÷���
	HANDLE* hWorkerHandle; // �۾� ������ �ڵ�
	int nThreadcnt;
};