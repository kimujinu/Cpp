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

class COMMONCLASS_API cCharactersInfo
{
public:
	cCharactersInfo();
	~cCharactersInfo();

	map<int, cCharactersInfo> players;

	friend ostream& operator<<(ostream& stream, cCharactersInfo& info)
	{
		stream << info.players.size() << endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << endl;
			stream << kvp.second << endl;
		}
		return stream;
	}

	friend istream& operator>>(istream& stream, cCharactersInfo& info)
	{
		int nPlayers = 0;
		int SessionId = 0;
		info.players.clear();

		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++) {
			stream >> SessionId;
		}

		return stream;
	}
};

class COMMONCLASS_API CommonClass
{
public:
	CommonClass();
	~CommonClass();
};