// IOCP_Server_C++.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "MainIocp.h"

int main()
{
	MainIocp iocp_server;
	if (iocp_server.Initialize()) {
		iocp_server.StartServer();
	}
	return 0;
}

