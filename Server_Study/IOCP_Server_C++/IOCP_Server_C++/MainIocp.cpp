#include "pch.h"
#include "MainIocp.h"
#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>

// static 변수 초기화
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
		printf_s("[INFO] DB 접속 성공\n");
	}
	else {
		printf_s("[ERROR] DB 접속 실패\n");
	}

	// 패킷 함수 포인터에 함수 지정
	fnProcess[EPacketType::SIGNUP].funcProcessPacket = SignUp;
	fnProcess[EPacketType::LOGIN].funcProcessPacket = Login;
	fnProcess[EPacketType::CHAT].funcProcessPacket = BroadcastChat;
	fnProcess[EPacketType::LOGOUT_PLAYER].funcProcessPacket = LogoutCharacter;
}

MainIocp::~MainIocp()
{
	WSACleanup(); // winsock의 사용을 끝낸다.
	// 다 사용한 객체 삭제
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

	Conn.Close(); // DB 연결 종료
}

void MainIocp::StartServer()
{
	IocpBase::StartServer();
}

bool MainIocp::CreateWorkerThread()
{
	unsigned int threadId;
	
	// 시스템 정보 가져오기
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf_s("[INFO] CPU 갯수 : %d\n", sysInfo.dwNumberOfProcessors);
	nThreadcnt = sysInfo.dwNumberOfProcessors * 2; // 적절한 작업 스레드의 갯수는 (cpu * 2) + 1
	
	hWorkerHandle = new HANDLE[nThreadcnt]; // thread handler 선언

	// thread 생성
	for (int i = 0; i < nThreadcnt; i++) {
		hWorkerHandle[i] = (HANDLE*)_beginthreadex(
			NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId
		);
		if (hWorkerHandle[i] == NULL)
		{
			printf_s("[ERROR] Worker Thread 생성 실패\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	printf_s("[INFO] Worker Thread 시작..\n");
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
		printf_s("[ERROR] WSASend 실패 : ", WSAGetLastError());
	}
}

void MainIocp::WorkerThread()
{
	BOOL bResult; // 함수 호출 성공 여부
	int nResult;
	// Overlapped I/O 작업에서 전송된 데이터 크기
	DWORD recvBytes;
	DWORD sendBytes;
	// Completion Key를 받을 포인터 변수
	stSOCKETINFO* pCompletionKey;
	// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	stSOCKETINFO* pSocketInfo;
	DWORD dwFlags = 0;

	while (bWorkerThread)
	{
		/*
		* 이 함수로 인해 쓰레드들은 WaitingThread Queue에 대기상태로 들어가게 된다.
		* 완료된 Overlapped I/O 작업이 발생하면 IOCP Queue에서 완료된 작업을 가져와 뒷처리함
		*/
		bResult = GetQueuedCompletionStatus(
			hIOCP,
			&recvBytes, // 실제로 전송된 바이트
			(PULONG_PTR)&pCompletionKey,// completion key
			(LPOVERLAPPED*)&pSocketInfo, // overlapped I/O 객체
			INFINITE // 대기할 시간
		);

		if (!bResult && recvBytes == 0)
		{
			printf_s("[INFO] socket(%d) 접속 끊김\n", pSocketInfo->socket);
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
			int PacketType; // 패킷 종류
			 // 클라이언트 정보 역직렬화
			stringstream RecvStream;

			RecvStream << pSocketInfo->dataBuf.buf;
			RecvStream >> PacketType;

			// 패킷 처리
			if (fnProcess[PacketType].funcProcessPacket != nullptr)
			{
				fnProcess[PacketType].funcProcessPacket(RecvStream, pSocketInfo);
			}
			else {
				printf_s("[ERROR] 정의 되지 않은 패킷 %d\n", PacketType);
			}
		}
		catch (const exception& e) {
			printf_s("[ERROR] 알 수 없는 예외 발생 : %s\n", e.what());
		}
		Recv(pSocketInfo); // 클라이언트 대기
	}
}

void MainIocp:: (stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	string Id;
	string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	printf_s("[INFO] 회원가입 시도 {%s}/{%s}\n", Id, Pw);

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

	printf_s("[INFO] 로그인 시도 {%s}/{%s}\n", Id, Pw);

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
	printf_s("[INFO] (%d) 로그아웃 요청 수신\n", SessionId);
	//EnterCriticalSection(&csPlayers);
	//CharactersInfo.players[SessionId].IsAlive = false;
	//LeaveCriticalSection(&csPlayers);
	SessionSocket.erase(SessionId);
	printf_s("[INFO] 클라이언트 수 : %d\n", SessionSocket.size());
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

	// 직렬화
	SendStream << EPacketType::RECV_PLAYER << endl;
	SendStream << CharactersInfo << endl;

	// 중요!. data.buf에다가 직접 데이터를 쓰면 쓰레기값이 전달될 수 있음
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

}