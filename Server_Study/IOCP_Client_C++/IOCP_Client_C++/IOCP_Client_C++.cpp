// IOCP_Client_C++.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_BUFFER = 1024;
const int SERVER_PORT = 30002;

struct stSOCKETINFO
{
    WSAOVERLAPPED overlapped;
    WSABUF dataBuf;
    SOCKET socket;
    char messageBuffer[MAX_BUFFER];
    int recvBytes;
    int sendBytes;
};

int main()
{
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (nResult != 0) {
        cout << "Error : " << WSAGetLastError() << "\n";
        return false;
    }

    // TCP 소켓 생성
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error : " << WSAGetLastError() << "\n";
        return false;
    }

    cout << "socket initialize success. " << "\n";

    SOCKADDR_IN stServerAddr; // 접속할 서버 정보를 저장할 구조체
    char szOutMsg[MAX_BUFFER];
    char sz_socketbuf[MAX_BUFFER];

    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_port = htons(SERVER_PORT);
    stServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    nResult = connect(clientSocket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
    if (nResult == SOCKET_ERROR) {
        cout << "Error : " << WSAGetLastError() << "\n";
        return false;
    }

    cout << "Connection success..." << "\n";

    while (true)
    {
        cout << ">>";
        cin >> szOutMsg;
        if (_strcmpi(szOutMsg, "quit") == 0)
            break;
        
        int nSendLen = send(clientSocket, szOutMsg, strlen(szOutMsg), 0);
        if (nSendLen == -1) {
            cout << "Error : " << WSAGetLastError() << "\n";
            return false;
        }

        cout << "Message sended : bytes[" << nSendLen << "], message : [" << szOutMsg << "]" << "\n";

        int nRecvLen = recv(clientSocket, sz_socketbuf, 1024, 0);
        if (nRecvLen == 0)
        {
            cout << "Client connection has been closed" << "\n";
            closesocket(clientSocket);
            return false;
        }
        else if (nRecvLen == -1) {
            cout << "Error : " << WSAGetLastError() << "\n";
            closesocket(clientSocket);
            return false;
        }

        sz_socketbuf[nRecvLen] = NULL;
        cout << "Message recv : bytes[" << nRecvLen << "], message : [" << sz_socketbuf << "]" << "\n";

    }
    closesocket(clientSocket);
    cout << "Client has been terminated.." << "\n";

    return 0;
}
