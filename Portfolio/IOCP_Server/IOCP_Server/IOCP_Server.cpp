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

constexpr int PORT_NUM = 3800; // ������ �ð��� �����Ǵ� ����� ����
constexpr int BUF_SIZE = 1024; 
constexpr int READ = 3;
constexpr int WRITE = 5;
constexpr int CLIENT_SIZE = 3000;
int login_count = 0;
int logout_count = 0;

typedef struct { // ���� ���� ����ü
	SOCKET hClientSock;
	SOCKADDR_IN clientAddr;
	CHAR name[20];
	CHAR ip[22];
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct { // ���� ���� ����ü
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	INT reMode; 
} PER_IO_DATA, *LPPER_IO_DATA;

vector<LPPER_HANDLE_DATA> UserList; // Ŭ���̾�Ʈ ��� vector container

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

	// ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WsaStartup Error");
	}

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	// ���� ������� ����� ������ ����
	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++) {
		_beginthreadex(NULL, 0, ThreadMain, (LPVOID)hComPort, 0, NULL);
	}

	// ���� ����
	hServSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT_NUM);

	// ���ε�
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
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA)); // LPPER_HANDLE_DATA �ʱ�ȭ
		inet_ntop(AF_INET, &clientAddr.sin_addr, handleInfo->ip, INET_ADDRSTRLEN); // ���ο� Ŭ���̾�Ʈ ip ��������
		handleInfo->hClientSock = hClientSock; // Ŭ���̾�Ʈ�� ������ ����ü�� ��� ���´�.
		cout << ++login_count << "\n";
		printf("New Client Access : %s\n", handleInfo->ip);
		memcpy(&(handleInfo->clientAddr), &clientAddr, addrLen);

		// ���� ����� ��Ʈ�� accept�� ���� return�� Ŭ���̾�Ʈ ������ ���´�.
		CreateIoCompletionPort((HANDLE)hClientSock, hComPort, (DWORD)handleInfo, 0);

		push_lock.lock();

		UserList.push_back(handleInfo);

		push_lock.unlock();

		// Ŭ���̾�Ʈ�� �����Ե� data �ʱ�ȭ
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
		// �񵿱� ����� �ϷḦ ���
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		
		// Ŭ���̾�Ʈ�� socket�� �����´�.
		sock = handleInfo->hClientSock;

		// ù ������ �б� ����
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
			message[bytesTrans] = '\0'; // ���ڿ��� ���� \0�� �߰��Ѵ�. (������ ���� ����)

			printf("name : %s\n", handleInfo->name);
			printf("Sock[%d] : %s\n", sock, message);

			// Ŭ���̾�Ʈ�� ���� ������ ����ü�� ������ �ٲ۴�.
			// ���� ������ ���⸦ �ൿ��.
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
			// ������ ����ü �ʱ�ȭ, ���� -> �б� ���� ����
			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->reMode = READ;

			// �б� ����
			if (WSARecv(sock, &(ioInfo->wsaBuf), 1, &bytesTrans, (LPDWORD)&flags, &(ioInfo->overlapped), NULL) == SOCKET_ERROR) 
			{
				if (WSAGetLastError() != WSA_IO_PENDING) {
					ErrorHandling("WSASend() error");
				}
			}
		}
		// ���� ����
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
