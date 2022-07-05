
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib")

int main(void) 
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	// 1. 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed WSAStartup() \n");
	}

	// 2. socket 생성
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
		printf("Failed socket() \n");

	// 3. 서버 연결
	// 3-1. 연결할 서버 주소 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; // 주소체계를 사용하는데 IPv4를 사용한다고 셋팅한다.

	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr); // 자신의 로컬 ip로 컴퓨터 내부에서 사용한다.
	servAddr.sin_port = htons(30002); // port 설정

	// 3-2. 서버에 연결 시도
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("Failed connect() \n");

	// 4. 서버로부터 통신 대기
	int recvSize;
	char recvData[255];
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvSize == -1)
		printf("recv() Error \n");

	printf("recv %d message : %s \n", recvSize, recvData);

	// 5. 소켓 종료 -> 윈속 종료
	closesocket(hSocket);
	WSACleanup();


	system("중지");
	return 0;

}