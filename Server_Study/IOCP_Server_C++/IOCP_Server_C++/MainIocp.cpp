#include "pch.h"
#include "MainIocp.h"
#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>

// static ���� �ʱ�ȭ
map<int, SOCKET> MainIocp::SessionSocket;
cCharactersInfo MainIocp::CharactersInfo;
DBConnector MainIocp::Conn;
//CRITICAL_SECTION MainIocp::csPlayers;

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	MainIocp* pOverlappedEvent = (MainIocp*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

MainIocp::MainIocp()
{
	//InitializeCriticalSection(&csPlayers);

	if (Conn.Connect(DB_ADDRESS, DB_ID, DB_PW, DB_SCHEMA, DB_PORT))
	{
		printf_s("[INFO] DB ���� ����\n");
	}
	else {
		printf_s("[ERROR] DB ���� ����\n");
	}

	// ��Ŷ �Լ� �����Ϳ� �Լ� ����
	fnProcess[EPacketType::SIGNUP].funcProcessPacket = SignUp;
	fnProcess[EPacketType::LOGIN].funcProcessPacket = Login;
	fnProcess[EPacketType::CHAT].funcProcessPacket = BroadcastChat;
	fnProcess[EPacketType::LOGOUT_PLAYER].funcProcessPacket = LogoutCharacter;
}

MainIocp::~MainIocp()
{
	WSACleanup(); // winsock�� ����� ������.
	// �� ����� ��ü ����
	if (SocketInfo) 
	{
		delete[] SocketInfo;
		SocketInfo = NULL;
	}

	if (hWorkerHandle)
	{
		delete[] hWorkerHandle;
		hWorkerHandle = NULL;
	}

	Conn.Close(); // DB ���� ����
}

void MainIocp::StartServer()
{
	IocpBase::StartServer();
}

bool MainIocp::CreateWorkerThread()
{
	unsigned int threadId;
	
	// �ý��� ���� ��������
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf_s("[INFO] CPU ���� : %d\n", sysInfo.dwNumberOfProcessors);
	nThreadcnt = sysInfo.dwNumberOfProcessors * 2; // ������ �۾� �������� ������ (cpu * 2) + 1
	
	hWorkerHandle = new HANDLE[nThreadcnt]; // thread handler ����

	// thread ����
	for (int i = 0; i < nThreadcnt; i++) {
		hWorkerHandle[i] = (HANDLE*)_beginthreadex(
			NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId
		);
		if (hWorkerHandle[i] == NULL)
		{
			printf_s("[ERROR] Worker Thread ���� ����\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	printf_s("[INFO] Worker Thread ����..\n");
	return true;
}

void MainIocp::Send(stSOCKETINFO* pSocket)
{
	int nResult;
	DWORD sendBytes;
	DWORD dwFlags = 0;

	nResult = WSASend(
		pSocket->socket,
		&(pSocket->dataBuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("[ERROR] WSASend ���� : ", WSAGetLastError());
	}
}

void MainIocp::WorkerThread()
{
	BOOL bResult; // �Լ� ȣ�� ���� ����
	int nResult;
	// Overlapped I/O �۾����� ���۵� ������ ũ��
	DWORD recvBytes;
	DWORD sendBytes;
	// Completion Key�� ���� ������ ����
	stSOCKETINFO* pCompletionKey;
	// I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
	stSOCKETINFO* pSocketInfo;
	DWORD dwFlags = 0;

	while (bWorkerThread)
	{
		/*
		* �� �Լ��� ���� ��������� WaitingThread Queue�� �����·� ���� �ȴ�.
		* �Ϸ�� Overlapped I/O �۾��� �߻��ϸ� IOCP Queue���� �Ϸ�� �۾��� ������ ��ó����
		*/
		bResult = GetQueuedCompletionStatus(
			hIOCP,
			&recvBytes, // ������ ���۵� ����Ʈ
			(PULONG_PTR)&pCompletionKey,// completion key
			(LPOVERLAPPED*)&pSocketInfo, // overlapped I/O ��ü
			INFINITE // ����� �ð�
		);

		if (!bResult && recvBytes == 0)
		{
			printf_s("[INFO] socket(%d) ���� ����\n", pSocketInfo->socket);
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		pSocketInfo->dataBuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		try {
			int PacketType; // ��Ŷ ����
			 // Ŭ���̾�Ʈ ���� ������ȭ
			stringstream RecvStream;

			RecvStream << pSocketInfo->dataBuf.buf;
			RecvStream >> PacketType;

			// ��Ŷ ó��
			if (fnProcess[PacketType].funcProcessPacket != nullptr)
			{
				fnProcess[PacketType].funcProcessPacket(RecvStream, pSocketInfo);
			}
			else {
				printf_s("[ERROR] ���� ���� ���� ��Ŷ %d\n", PacketType);
			}
		}
		catch (const exception& e) {
			printf_s("[ERROR] �� �� ���� ���� �߻� : %s\n", e.what());
		}
		Recv(pSocketInfo); // Ŭ���̾�Ʈ ���
	}
}

void MainIocp:: (stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	string Id;
	string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	printf_s("[INFO] ȸ������ �õ� {%s}/{%s}\n", Id, Pw);

	stringstream SendStream;
	SendStream << EPacketType::SIGNUP << endl;
	SendStream << Conn.SignUpAccount(Id, Pw) << endl;

	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

	Send(pSocket);
}

void MainIocp::Login(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	string Id;
	string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	printf_s("[INFO] �α��� �õ� {%s}/{%s}\n", Id, Pw);

	stringstream SendStream;
	SendStream << EPacketType::LOGIN << endl;
	SendStream << Conn.SearchAccount(Id, Pw) << endl;

	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

	Send(pSocket);
}

void MainIocp::LogoutCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	int SessionId;
	RecvStream >> SessionId;
	printf_s("[INFO] (%d) �α׾ƿ� ��û ����\n", SessionId);
	//EnterCriticalSection(&csPlayers);
	//CharactersInfo.players[SessionId].IsAlive = false;
	//LeaveCriticalSection(&csPlayers);
	SessionSocket.erase(SessionId);
	printf_s("[INFO] Ŭ���̾�Ʈ �� : %d\n", SessionSocket.size());
	//WriteCharactersInfoToSocket(pSocket);
}

void MainIocp::BroadcastChat(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	stSOCKETINFO* client = new stSOCKETINFO;

	int SessionId;
	string Temp;
	string Chat;

	RecvStream >> SessionId;
	getline(RecvStream, Temp);
	Chat += to_string(SessionId) + "_:_";
	while (RecvStream >> Temp)
	{
		Chat += Temp + "_";
	}
	Chat += '\0';

	printf_s("[CHAT] %s \n", Chat);

	stringstream SendStream;
	SendStream << EPacketType::CHAT << endl;
	SendStream << Chat;

	Broadcast(SendStream);
}

void MainIocp::Broadcast(stringstream& SendStream)
{
	stSOCKETINFO* client = new stSOCKETINFO;
	for (const auto& kvp : SessionSocket)
	{
		client->socket = kvp.second;
		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		client->dataBuf.buf = client->messageBuffer;
		client->dataBuf.len = SendStream.str().length();

		Send(client);
	}
}

void MainIocp::WriteCharactersInfoToSocket(stSOCKETINFO* pSocket)
{
	stringstream SendStream;

	// ����ȭ
	SendStream << EPacketType::RECV_PLAYER << endl;
	SendStream << CharactersInfo << endl;

	// �߿�!. data.buf���ٰ� ���� �����͸� ���� �����Ⱚ�� ���޵� �� ����
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

}