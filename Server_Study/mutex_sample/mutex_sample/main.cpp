// mutex_sample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <mutex>

void read(int x){}
void write(int x){}
void sum(int x) {}

using namespace std;

int main()
{
    int x = 0, y = 0;
    recursive_mutex mx;
    {
        lock_guard<recursive_mutex> lock(mx);
        srand(0);
        read(x);
        write(y);
        sum(x);
    }
    return 0;
}

