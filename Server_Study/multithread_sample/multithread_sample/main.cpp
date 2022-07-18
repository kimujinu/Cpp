// multithread_sample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;
const int MaxCount = 50;

const int ThreadCount = 4;

bool IsPrimeNumber(int number)
{
    if (number == 1)
        return false;
    if (number == 2 || number == 3)
        return true;
    for (int i = 2; i < number - 1; i++)
    {
        if ((number % i) == 0)
            return false;
    }
    return true;
}

void PrintNumbers(const vector<int>& primes)
{
    for (int v : primes)
    {
        cout << v << endl;
    }
}

void prime_number_normal()
{
    vector<int> primes;

    auto t0 = chrono::system_clock::now();

    for (int i = 1; i <= MaxCount; i++)
    {
        if (IsPrimeNumber(i))
        {
            primes.push_back(i);
        }
    }
    auto t1 = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    cout << "Took " << duration << " milliseconds." << endl;

    PrintNumbers(primes);
}

void prime_number_errorneous()
{
    // 각 쓰레드는 여기서 값을 꺼내온다.
    int num = 1;

    vector<int> primes;

    auto t0 = chrono::system_clock::now();

    // 작동할 워커 스레드
    vector<shared_ptr<thread>> threads;

    for (int i = 0; i < ThreadCount; i++)
    {
        shared_ptr<thread> thread1(new thread([&]() {
            // 각 쓰레드의 메인함수
            // 값을 가져올 수 있으면 루프를 돈다.
            while (true)
            {
                int n;
                n = num;
                num++;

                if (n >= MaxCount)
                    break;

                if (IsPrimeNumber(n))
                {
                    primes.push_back(n);
                }
            }
         }));
        // 쓰레드 객체를 일단 갖고 있는다.
        threads.push_back(thread1);
    }

    for (auto thread : threads)
    {
        /*
        * 해당하는 쓰레드들이 실행을 종료하면 리턴하는 함수
        * 없으면 main 함수가 먼저 종료되어서 쓰레드가 소멸해버림
        * 쓰레드 작업이 끝날 때까지 main 함수가 끝나지 않게하는 방법
        */
        thread->join(); 
    }

    auto t1 = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    cout << "Took " << duration << " milliseconds." << endl;

    PrintNumbers(primes);
}

void prime_number_multithread()
{
    // 각 쓰레드는 여기서 값을 꺼내온다.
    int num = 1;
    recursive_mutex num_mutex;

    vector<int> primes;
    recursive_mutex primes_mutex;

    auto t0 = chrono::system_clock::now();

    // 작동할 워커 쓰레드
    vector<shared_ptr<thread>> threads;
    
    for (int i = 0; i < ThreadCount; i++)
    {
        shared_ptr<thread> thread1(new thread([&]() {
            // 각 쓰레드의 메인 함수
            // 값을 가져올 수 있으면 루프를 돈다.
            while (true)
            {
                int n;
                {
                    lock_guard<recursive_mutex> num_lock(num_mutex);
                    n = num;
                    num++;
                }
                if (n >= MaxCount)
                    break;

                if (IsPrimeNumber(n))
                {
                    lock_guard<recursive_mutex> primes_lock(primes_mutex);
                    primes.push_back(n);
                }
            }
        }));
        // 쓰레드 객체를 갖고 있는다.
        threads.push_back(thread1);
    }

    // 모든 쓰레드가 일을 마칠 때까지 기다린다.
    for (auto thread : threads)
    {
        thread->join();
    }

    auto t1 = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cout << "Took " << duration << " milliseconds." << endl;

    PrintNumbers(primes);
}

void prime_number_openmp()
{
    vector<int> primes;

    recursive_mutex primes_mutex;

    auto t0 = chrono::system_clock::now();

    #pragma omp parallel for schedule(guided)
    for (int i = 1; i <= MaxCount; i++)
    {
        if (IsPrimeNumber(i))
        {
            lock_guard<recursive_mutex> primes_lock(primes_mutex);
            primes.push_back(i);
        }
    }
    auto t1 = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cout << "Took " << duration << " milliseconds." << endl;

    PrintNumbers(primes);
}

int main()
{
    prime_number_normal();
    prime_number_errorneous();
    prime_number_multithread();
    prime_number_openmp();
    return 0;
}
