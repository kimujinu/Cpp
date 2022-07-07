#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

int main(void) 
{
	WSADATA wsaData;
	SOCKET listenSocket;
	SOCKADDR_IN serv_Addr;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed WSAStartup() \n");
		return 1;
	}

	// 2.socket ����
	listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		WSACleanup();
		printf("Failed socket() \n");
		return 1;
	}
	// 3. ���� �ڽ��� �ּҸ� ����
	// 3-1. ���� �ּ� ����
	memset(&serv_Addr, 0, sizeof(serv_Addr));
	serv_Addr.sin_family = AF_INET;
	serv_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET, "127.0.0.1", &serv_Addr.sin_addr);
	serv_Addr.sin_port = htons(30002);

	if (bind(listenSocket, (SOCKADDR*)&serv_Addr, sizeof(serv_Addr)) == SOCKET_ERROR) {
		printf("Binding Error \n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	if (listen(listenSocket, 5) == SOCKET_ERROR) {
		printf("listen Error \n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server Start \n");

	// 3-2. Ŭ���̾�Ʈ ���� ��ٸ���
	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeClientAddr = sizeof(clientAddr);

	char sendData[255] = "Hi";
	char recvByte;
	char recvData[255];
	int recvCount = 0;

	// 4. ������ �õ��� Ŭ���̾�Ʈ ����� ��� ó��
	while (1) 
	{
		// 4-1. Ŭ���̾�Ʈ ���� ��ٸ���
		clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
		if (clientSocket == INVALID_SOCKET)
			printf("Failed accept() \n");
		recvCount = 0;

		printf("accept client socket : %d \n", clientSocket);
		printf("%d.%d.%d.%d : %d \n", clientAddr.sin_addr.S_un.S_un_b.s_b1,
			clientAddr.sin_addr.S_un.S_un_b.s_b2,
			clientAddr.sin_addr.S_un.S_un_b.s_b3,
			clientAddr.sin_addr.S_un.S_un_b.s_b4,
			clientAddr.sin_port);

		// 4-2. ����� Ŭ���̾�Ʈ���� send
		send(clientSocket, sendData, strlen(sendData) + 1, 0);

		// 4-3. ������ client�� ������ ���������� recv ��⸦ �ݺ��Ѵ�.
		printf("recv count : ");
		while ((recvByte = recv(clientSocket, recvData, sizeof(recvData),0)) > 0)
		{
			// ������ �״�� ������.
			printf("[%d]%d ", ++recvCount, recvData[0]);
			send(clientSocket, recvData, recvByte, 0);
		}

		// 4-4. ������ ����ó��
		printf("\n");
		closesocket(clientSocket);
		printf("close socket : %d \n", clientSocket);
	}
	
	// 5. ���� ���� -> ���� ����
	closesocket(listenSocket);
	WSACleanup();

	return 0;


}