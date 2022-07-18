
#include <iostream>
#include <thread>

#include "Server.h"

int main()
{
	Server server;
	server.Init();
	
	std::thread runThread([&]() {
		server.Run();}
	);

	std::cout << "아무키나 누르시오..";
	getchar();

	server.Stop();
	runThread.join();

	return 0;
}
