#pragma once

#include <iostream>
#include <map>

#ifdef COMMONCLASS_EXPORTS
#define COMMONCLASS_API __declspec(dllexport)
#else
#define COMMONCLASS_API __declspec(dllimport)
#endif

using namespace std;

#define MAX_CLIENT 1000

enum COMMONCLASS_API EPacketType
{
	LOGIN,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	CHAT,
	SIGNUP
};

class COMMONCLASS_API CommonClass
{
public:
	CommonClass();
	~CommonClass();
};