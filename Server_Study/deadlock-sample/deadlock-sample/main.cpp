// deadlock-sample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "CriticalSection.h"
#include <iostream>

using namespace std;

int a;
CriticalSection a_mutex;
int b;
CriticalSection b_mutex;

int main()
{
	// t1 쓰레드 시작.
	thread t1([]()
		{
			while (true)
			{
				CriticalSectionLock lock(a_mutex);
				a++;
				CriticalSectionLock lock2(b_mutex);
				b++;
				cout << "t1 done.\n";
			}
		});

	// t2 쓰레드 시작.
	thread t2([]()
		{
			while (true)
			{
				CriticalSectionLock lock(b_mutex);
				b++;
				CriticalSectionLock lock2(a_mutex);
				a++;
				cout << "t1 done.\n";
			}
		});

	// 스레드들의 일이 끝날때까지 기다린다.
	// 사실상 무한루프이기 때문에 끝나지 못한다.
	t1.join();
	t2.join();

	return 0;
}

