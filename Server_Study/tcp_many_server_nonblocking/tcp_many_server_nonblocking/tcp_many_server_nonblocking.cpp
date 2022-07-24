// tcp_many_server_nonblocking.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include "pch.h"
#include "../ImaysNet/ImaysNet.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>

using namespace std;

// ture가 되면 프로그램을 종료함.
volatile bool stopWorking = false;

void ProcessSignalAction(int sig_number)
{
    if (sig_number == SIGINT)
        stopWorking = true;
}

int main()
{
    // 사용자가 ctl-c를 누르면 메인루프를 종료한다.
    signal(SIGINT, ProcessSignalAction);

    try
    {
        /* 논블록 소켓으로 많은 수의 클라이언트를 받아 처리한다.
        *  많은 수의 클라이언트가 tcp 5555로 연결이 들어오고
        *  해당 클라이언트들을 Server Test Performance를 보낼 것이다.
        *  그것을 그냥 에코해준다.
        *  서버에서는 총 처리한 바이트수를 지속적으로 출력만 하자.
        */

        // TCP 연결 각각의 객체
        struct RemoteClient
        {
            Socket tcpConnection; // accept한 TCP 연결
        };
        unordered_map<RemoteClient*, shared_ptr<RemoteClient>> remoteClients;

        // TCP 연결을 받는 소켓
        Socket listenSocket(SocketType::Tcp);
        listenSocket.Bind(Endpoint("0.0.0.0", 5555));

        listenSocket.SetNonblocking();
        listenSocket.Listen();

        cout << "서버가 시작되었습니다..\n";
        cout << "CTL-C키를 누르면 프로그램을 종료합니다.\n";

        /* listen소켓과 TCP 연결 소켓 모두에 대해서 I/O가능(avail) 이벤트가 있을 때까지 기다린다.
        *  그리고 나서 I/O 가능 소켓에 대해서 일을 한다.
        *  여기에 넣은 소켓 핸들에 대해서 select나 poll을 한다.
        *  다만, receive나 accept에 대해서만 처리한다.
        */
        vector<PollFD> readFds;
        // 어느 소켓이 어느 RemoteClient에 대한 것인지를 가리킨다.
        vector<RemoteClient*> readFdsToRemoteClients;

        while (!stopWorking)
        {
            readFds.reserve(remoteClients.size() + 1);
            readFds.clear();
            readFdsToRemoteClients.reserve(remoteClients.size() + 1);
            readFdsToRemoteClients.clear();

            for (auto i : remoteClients)
            {
                PollFD item;
                item.m_pollfd.events = POLLRDNORM;
                item.m_pollfd.fd = i.second->tcpConnection.m_fd;
                item.m_pollfd.revents = 0;
                readFds.push_back(item);
                readFdsToRemoteClients.push_back(i.first);
            }

            // 마지막 항목은 listen 소켓이다.
            PollFD item2;
            item2.m_pollfd.events = POLLRDNORM;
            item2.m_pollfd.fd = listenSocket.m_fd;
            item2.m_pollfd.revents = 0;
            readFds.push_back(item2);

            // I/O 가능 이벤트가 있을 때까지 기다린다.
            Poll(readFds.data(), (int)readFds.size(), 100);

            // readFds를 수색해서 필요한 처리를 한다.
            int num = 0;
            for (auto readFd : readFds)
            {
                if (readFd.m_pollfd.revents != 0)
                {
                    if (num == readFds.size() - 1) // listen 소켓이라면
                    {
                        // accept을 처리한다.
                        auto remoteClient = make_shared<RemoteClient>();

                        // 이미 클라이언트 연결이 들어왔음 이벤트가 온 상태
                        string ignore;
                        listenSocket.Accept(remoteClient->tcpConnection, ignore);
                        remoteClient->tcpConnection.SetNonblocking();

                        // 새 클라이언트를 목록에 추가
                        remoteClients.insert({ remoteClient.get(), remoteClient });

                        cout << "Client joined. There are " << remoteClients.size() << " connections.\n";
                    }
                    else // TCP 연결 소켓이라면
                    {
                        // 받은 데이터를 그대로 회신
                        RemoteClient* remoteClient = readFdsToRemoteClients[num];

                        int ec = remoteClient->tcpConnection.Receive();
                        if (ec <= 0)
                        {
                            // 에러 혹은 소켓 종료시에는 소켓 제거
                            remoteClient->tcpConnection.Close();
                            remoteClients.erase(remoteClient);

                            cout << "Client left. There are " << remoteClients.size() << "connections.\n";
                        }
                        else
                        {
                            // 받은 데이터 그대로 송신
                            remoteClient->tcpConnection.Send(remoteClient->tcpConnection.m_receiveBuffer, ec);
                        }
                    }
                }
                num++;
            }
        }
        // 사용자가 CTL-c를 눌러서 루프를 나감. 모든 종료를 해야함.
        listenSocket.Close();
        remoteClients.clear();
    }
    catch (Exception& e)
    {
        cout << "Exception! " << e.what() << endl;
    }

    return 0;

}


