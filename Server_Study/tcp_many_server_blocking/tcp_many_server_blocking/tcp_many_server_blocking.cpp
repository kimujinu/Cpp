// tcp_many_server_blocking.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "../ImaysNet/ImaysNet.h"
#include <iostream>
#include <stdio.h>
#include <thread>
#include <unordered_map>
#include <signal.h>
#include <memory>
#include <vector>
#include <deque>
#include <mutex>

using namespace std;

// TCP 연결 각각의 객체
struct RemoteClient
{
    shared_ptr<thread> thread; // 클라이언트 처리를 하는 스레드 1개
    Socket tcpConnection; // accept한 TCP 연결
};

// 여기 들어온 TCP 연결 객체들, key는 RemoteClient의 포인터값 자체이다.
unordered_map<RemoteClient*, shared_ptr<RemoteClient>> remoteClients;
// 여기에 들어간 RemoteClient는 곧 파괴된다.
deque<shared_ptr<RemoteClient>> remoteClientDeathNote;
// 여기에 +1이 발생하면 뭔가 할 일이 있다는 것이다.
Semaphore mainThreadWorkCount;

// 위 변수를 보호하는 mutex, 메인 스레드와 TCP 연결 받기를 처리하는 스레드가 공유하기 때문에 필요하다.
recursive_mutex remoteClientsMutex;
// 콘솔 출력을 여러 스레드가 하면 꼬일 수 있다. 그래서 일렬로 처리하자.
recursive_mutex consoleMutex;

volatile bool stopWorking = false;

shared_ptr<Socket> listenSocket; // 리스닝 소켓

void RemoteClientThread(shared_ptr<RemoteClient> remoteClient);
void ListenSocket();

void ProcessSignalAction(int sig_number)
{
    if (sig_number == SIGINT)
    {
        stopWorking = true;
        mainThreadWorkCount.Notify();
    }
}
int main()
{
    /*
    * 프로세스 우선순위를 낮추어서 실험을 불편하지 않게 해준다.
    */
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    try
    {
        // 사용자가 ctl-c를 누르면 메인루프를 종료하게 만든다.
        signal(SIGINT, ProcessSignalAction);

        // 클라이언트 연결 수 만큼 스레드를 생성하는 블로킹 소켓 방식이다.

        /* 많은 수의 클라이언트가 tcp 5555로 연결이 들어오고,
        *  그들은 Server Performance Test를 열심히 보낼 것이다.
        *  그것을 그냥 에코해주도록 하자.
        *  서버에서는 총 처리한 바이트수를 지속적으로 출력만 하자.
        */

        // TCP 연결을 받는 소켓
        listenSocket = make_shared<Socket>(SocketType::Tcp);
        listenSocket->Bind(Endpoint("0.0.0.0", 5555));
        listenSocket->Listen();

        cout << "서버가 시작되었습니다..\n";
        cout << "CTL-C를 누르면 프로그램을 종료합니다.\n";

        // 리스닝 소켓을 위한 스레드를 생성
        thread tcpListenThread(ListenSocketThread);

        while (!stopWorking)
        {
            // 메인 스레드에서 할 일이 있을 때까지 기다린다.
            mainThreadWorkCount.Wait();

            // 살생부 처리
            lock_guard<recursive_mutex> lock(remoteClientsMutex);

            while (!remoteClientDeathNote.empty())
            {
                auto remoteClientToDelete = remoteClientDeathNote.front();
                remoteClientToDelete->tcpConnection.Close();
                remoteClientToDelete->thread->join();
                remoteClients.erase(remoteClientToDelete.get()); // 클라이언트 목록에서 제거
                remoteClientDeathNote.pop_front(); // socket close도 이 과정에서 이루어진다.

                lock_guard<recursive_mutex> lock(consoleMutex);
                cout << "Client left. There are " << remoteClients.size() << "connections.\n";
            }
        }

        // 메인루프가 끝났다. 프로그램 종료를 해야하는 상황

        // 접속해 있는 모든 클라이언트 소켓도 닫는다.
        listenSocket->Close();
        {
            lock_guard<recursive_mutex> lock(remoteClientsMutex);
            for (auto i : remoteClients)
            {
                // 모든 스레드를 종료시킨다.
                i.second->tcpConnection.Close();
                i.second->thread->join();
            }
        }

        // 모든 스레드를 종료시킨다.
        tcpListenThread.join();

        // 스레드들이 모두 종료할때까지 기다리는 처리도 포함.
        remoteClients.clear();
    }
    catch (Exception& e) {
        cout << "Exception! " << e.what() << endl;
    }

    return 0;
}

// TCP 소켓 연결을 받는 처리를 하는 스레드
void ListenSocketThread()
{
    while (!stopWorking)
    {
        auto remoteClient = make_shared<RemoteClient>();
        string errorText;
        // TCP 연결 대기
        if (listenSocket->Accept(remoteClient->tcpConnection, errorText) == 0)
        {
            // TCP 연결이 들어옴.
            // RemoteClient 객체를 만들어야 함.
            {
                lock_guard<recursive_mutex> lock(remoteClientsMutex);
                remoteClients.insert({ remoteClient.get(), remoteClient });

                // TCP 클라이언트-서버 통신을 위한 스레드를 하나 만들어주자
                // TCP 연결 하나당 스레드 하나이다.
                remoteClient->thread = make_shared<thread>(RemoteClientThread, remoteClient);
            }

            // 콘솔 출력
            {
                lock_guard<recursive_mutex> lock(consoleMutex);
                cout << "Client joined There are " << remoteClients.size() << "conections.\n";
            }
        }
        else
        {
            // accept이 실패. 리스닝 스레드를 종료해야함.
            break; 
        }
    }
}

// 이미 들어온 TCP 연결을 처리하는 스레드
void RemoteClientThread(shared_ptr<RemoteClient> remoteClient)
{
    while (!stopWorking)
    {
        // 받은 데이터를 그대로 회신
        int ec = remoteClient->tcpConnection.Receive();
        if (ec <= 0)
        {
            if (ec <= 0)
            {
                // 에러 혹은 소켓 종료.
                break;
            }
        }
        remoteClient->tcpConnection.Send(remoteClient->tcpConnection.m_receiveBuffer, ec);
    }

    // 루프가 끝남. 이 RemoteClient는 종료되어야 함을 기록.
    remoteClient->tcpConnection.Close();
    lock_guard<recursive_mutex> lock(remoteClientsMutex);
    remoteClientDeathNote.push_back(remoteClient);
    mainThreadWorkCount.Notify();
}
