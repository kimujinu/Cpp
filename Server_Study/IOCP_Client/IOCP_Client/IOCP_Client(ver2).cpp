#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char* message);

int main() {
	
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;
	WSAOVERLAPPED overlapped;
	WSABUF dataBuf;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvAddr.sin_port = htons(30002);

	if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	WSAEVENT event = WSACreateEvent();
	
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = event;
	char message[1024] = { 0, };
	int sendBytes = 0;
	int recvBytes = 0;
	int flags = 0;

	while (true)
	{
		flags = 0;
		printf("������ ������(���Ḧ ���ϸ� exit))\n");

		fgets(message, 1024, stdin);

		if (!strcmp(message, "exit"))
			break;

		dataBuf.len = strlen(message);
		dataBuf.buf = message;

		if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				ErrorHandling("WSASend() error");
			}
		}

		WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);
		WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL);

		printf("���۵� ����Ʈ �� : %d\n", sendBytes);

		if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				ErrorHandling("WSASend() error");
			}
		}
		printf("Recv[%s]\n", dataBuf.buf);
	}
	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}