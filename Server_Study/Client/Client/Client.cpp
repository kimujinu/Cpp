
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib")

int main(void) 
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed WSAStartup() \n");
		return 1;
	}

	// 2. socket ����
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET) {
		printf("Failed socket() \n");
		WSACleanup();
	}

	// 3. ���� ����
	// 3-1. ������ ���� �ּ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; // �ּ�ü�踦 ����ϴµ� IPv4�� ����Ѵٰ� �����Ѵ�.

	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr); // �ڽ��� ���� ip�� ��ǻ�� ���ο��� ����Ѵ�.
	servAddr.sin_port = htons(30002); // port ����

	// 3-2. ������ ���� �õ�
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		printf("Failed connect() \n");
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	// 4. �����κ��� ��� ���
	int recvSize;
	char recvData[255];
	char sendData[255];
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvSize == SOCKET_ERROR)
		printf("recv() Error \n");

	printf("recv : %s \n", recvData);

	// 5. ����� ������ ���
	int count = 10;
	while (count--)
	{
		sendData[0] = count;
		send(hSocket, sendData, 1, 0);
		printf("send Data : %d \n", sendData[0]);
		recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
		if (recvSize == SOCKET_ERROR) {
			printf("recv() Error \n");
			break;
		}
		printf("recv Data : %d \n", recvData[0]);
	}

	// 5. ���� ���� -> ���� ����
	closesocket(hSocket);
	WSACleanup();

	system("����");
	return 0;

}