/* ������ �ʿ���.
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

const int BUF_SIZE = 1024;
const int NAME_SIZE = 20;

unsigned WINAPI SendMsg(void* arg); // ������ �����Լ�
unsigned WINAPI RecvMsg(void* arg); // ������ �����Լ�
void ErrorHandling(const char* msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main() {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;
	HANDLE sendThread, recvThread;

	//if (argc != 4) {
	//	printf("Usage : %s <IP> <Port> <name> \n", argv[0]);
	//	exit(1);
	//}
	cout << "connect.....\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // ������ ������ ����Ѵٰ� �ü���� �˸�
		ErrorHandling("WSAStartip() error!");

	//sprintf(name, "%s", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0); // ������ �ϳ� �����Ѵ�.

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(3800);

	if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { // ������ �����Ѵ�.
		ErrorHandling("connect() error");
	}

	// ���ӿ� �����ϸ� �� �� �Ʒ��� ����ȴ�.
	cout << "Connect Success! \n";
	cout << "Sending Client's name \n";
	send(sock, name, sizeof(name), 0);
	cout << "Success! \n";

	sendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&sock, 0, NULL); // �޽��� ���ۿ� �����尡 ����ȴ�.
	recvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&sock, 0, NULL); // �޽��� ���ſ� �����尡 ����ȴ�.

	WaitForSingleObject(sendThread, INFINITE); // ���ۿ� �����尡 �����ɶ����� ��ٸ���.
	WaitForSingleObject(recvThread, INFINITE); // ���ſ� �����尡 �����ɶ����� ��ٸ���.

	// Ŭ���̾�Ʈ�� ���Ḧ �õ��Ѵٸ� ���� �Ʒ��� ����ȴ�.
	closesocket(sock); // ���� ����
	WSACleanup(); // ������ ���� ��������� �ü���� �˸���.

	cout << "Client exit\n";
	return 0;
}

unsigned WINAPI SendMsg(void* arg) { // ���ۿ� �������Լ�
	SOCKET sock = *((SOCKET*)arg); // ������ ������ �����Ѵ�.
	char msg[BUF_SIZE];
	while (true)
	{
		fgets(msg, BUF_SIZE, stdin); // �Է��� �޴´�.
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) { // q�� �Է��ϸ� �����Ѵ�.
			closesocket(sock);
			send(sock, "", 0, 0);
			exit(0);
		}
		send(sock, msg, strlen(msg), 0); // nameMSG�� �������� ����.
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg) {
	SOCKET sock = *((SOCKET*)arg); // ������ ������ �����Ѵ�.
	char msg[NAME_SIZE + BUF_SIZE];
	int strLen;
	while (true)
	{
		strLen = recv(sock, msg, NAME_SIZE + BUF_SIZE - 1, 0); // �����κ��� �޽����� �����Ѵ�.
		if (strLen == -1) {
			return -1;
		}
		msg[strLen] = 0; // ���ڿ��� ���� �˸��� ���� ����
		cout << ">>" << msg << "\n";
	}
	return 0;
}

void ErrorHandling(const char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
*/