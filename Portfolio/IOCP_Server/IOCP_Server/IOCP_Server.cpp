#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <WS2tcpip.h>
#include <mutex>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

mutex push_lock;
mutex erase_lock;
mutex sock_lock;

constexpr int PORT_NUM = 3800; // 컴파일 시간에 결정되는 상수를 만듬
constexpr int BUF_SIZE = 1024; 
constexpr int READ = 3;
constexpr int WRITE = 5;
constexpr int CLIENT_SIZE = 3000;
int login_count = 0;
int logout_count = 0;

typedef struct { // 소켓 정보 구조체
	SOCKET hClientSock;
	SOCKADDR_IN clientAddr;
	CHAR name[20];
	CHAR ip[22];
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct { // 버퍼 정보 구조체
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	INT reMode; 
} PER_IO_DATA, *LPPER_IO_DATA;

vector<LPPER_HANDLE_DATA> UserList; // 클라이언트 통신 vector container

void ErrorHandling(const char *message);
unsigned __stdcall ThreadMain(void * CompletionPortIO);

int main() {
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	DWORD recvBytes, flags = 0;
	INT i;

	// 윈속 시작
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WsaStartup Error");
	}

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	// 메인 쓰레드와 연결된 쓰레드 생성
	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++) {
		_beginthreadex(NULL, 0, ThreadMain, (LPVOID)hComPort, 0, NULL);
	}

	// 소켓 설정
	hServSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT_NUM);

	// 바인딩
	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, CLIENT_SIZE);

	while (true)
	{
		sock_lock.lock();

		SOCKET hClientSock;
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);

		hClientSock = accept(hServSock, (SOCKADDR*)&clientAddr, &addrLen);

		sock_lock.unlock();
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA)); // LPPER_HANDLE_DATA 초기화
		inet_ntop(AF_INET, &clientAddr.sin_addr, handleInfo->ip, INET_ADDRSTRLEN); // 새로운 클라이언트 ip 가져오기
		handleInfo->hClientSock = hClientSock; // 클라이언트의 정보를 구조체에 담아 놓는다.
		cout << ++login_count << "\n";
		printf("New Client Access : %s\n", handleInfo->ip);
		memcpy(&(handleInfo->clientAddr), &clientAddr, addrLen);

		// 소켓 입출력 포트에 accept을 통해 return된 클라이언트 정보를 묶는다.
		CreateIoCompletionPort((HANDLE)hClientSock, hComPort, (DWORD)handleInfo, 0);

		push_lock.lock();

		UserList.push_back(handleInfo);

		push_lock.unlock();

		// 클라이언트가 가지게될 data 초기화
		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		memset(ioInfo->buffer, 0x00, BUF_SIZE);

		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->reMode = READ;

		recv(handleInfo->hClientSock, handleInfo->name, 20, 0);
		WSARecv(handleInfo->hClientSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}

unsigned __stdcall ThreadMain(void * pComPort) 
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	int flags = 0;
	CHAR message[BUF_SIZE];

	while (true)
	{
		// 비동기 입출력 완료를 대기
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		
		// 클라이언트의 socket을 가져온다.
		sock = handleInfo->hClientSock;

		// 첫 시작은 읽기 상태
		if (ioInfo->reMode == READ) {
			puts("\n Message Received");
			if (bytesTrans == 0) {
				erase_lock.lock();
				vector<LPPER_HANDLE_DATA>::iterator iter;
				for (iter = UserList.begin(); iter != UserList.end(); ++iter) {
					if ((*iter)->hClientSock == sock) {
						cout << "Logout"
							<< "IP :" << (*iter)->ip << ", "
							<< "Sock :" << (*iter)->hClientSock << ", "
							<< "name :" << (*iter)->name << "\n ";
						UserList.erase(iter);
						cout << ++logout_count << "\n";
						break;
					}
				}
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				erase_lock.unlock();
				continue;
			}
			memcpy(message, ioInfo->wsaBuf.buf, BUF_SIZE);
			message[bytesTrans] = '\0'; // 문자열의 끝에 \0을 추가한다. (쓰레기 버퍼 방지)

			printf("name : %s\n", handleInfo->name);
			printf("Sock[%d] : %s\n", sock, message);

			// 클라이언트가 가진 데이터 구조체의 정보를 바꾼다.
			// 이젠 서버가 쓰기를 행동함.
			vector<LPPER_HANDLE_DATA>::iterator iter;

			free(ioInfo);

			for (iter = UserList.begin(); iter != UserList.end(); ++iter) {
				ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
				memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
				int len = strlen(message);
				ioInfo->wsaBuf.len = len;
				strcpy_s(ioInfo->buffer, message);
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->reMode = WRITE;
				if (bytesTrans == 0) {
					closesocket(sock);
					free(handleInfo);
					free(ioInfo);
					continue;
				}

				if (WSASend((*iter)->hClientSock, &(ioInfo->wsaBuf), 1, &bytesTrans, 0, &(ioInfo->overlapped), NULL) == SOCKET_ERROR) {
					if (WSAGetLastError() != WSA_IO_PENDING) {
						ErrorHandling("WSASend() error");
					}
				}
			}
			// 데이터 구조체 초기화, 쓰기 -> 읽기 모드로 변경
			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->reMode = READ;

			// 읽기 시작
			if (WSARecv(sock, &(ioInfo->wsaBuf), 1, &bytesTrans, (LPDWORD)&flags, &(ioInfo->overlapped), NULL) == SOCKET_ERROR) 
			{
				if (WSAGetLastError() != WSA_IO_PENDING) {
					ErrorHandling("WSASend() error");
				}
			}
		}
		// 쓰기 상태
		else {
			printf("Message Send \n");
			free(ioInfo);
		}
	}
	return 0;
}

void ErrorHandling(const char* message) {
	perror(message);
	exit(1);
}
