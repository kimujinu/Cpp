#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

#pragma pack(push, 1)
struct PktHeader
{
	short TotalSize;
	short Id;
	unsigned char Reserve;
};

struct PktBase
{
	short ErrorCode = (short)ERROR_CODE::NONE;
	void SetError(ERROR_CODE error) { ErrorCode = (short)error; }
};

//- 로그인 요청
const int MAX_USER_ID_SIZE = 16;
const int MAX_USER_PASSWORD_SIZE = 16;
struct PktLogInReq
{
	char szID[MAX_USER_ID_SIZE] = { 0, };
	char szPW[MAX_USER_PASSWORD_SIZE] = { 0, };
};

struct PktLogInRes : PktBase
{
};


//- 방에 들어가기 요청
const int MAX_ROOM_TITLE_SIZE = 16;
struct PktRoomEnterReq
{
	bool IsCreate;
	short RoomIndex;
};

struct PktRoomEnterRes : PktBase
{
};


//- 방에 있는 유저에게 새로 들어온 유저 정보 통보
struct PktRoomEnterUserInfoNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
};


//- 방 나가기 요청
struct PktRoomLeaveReq {};

struct PktRoomLeaveRes : PktBase
{
};

//- 방에서 나가는 유저 통보(다른 유저에게)
struct PktRoomLeaveUserInfoNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
};


//- 방 채팅
const int MAX_ROOM_CHAT_MSG_SIZE = 256;
struct PktRoomChatReq
{
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};

struct PktRoomChatRes : PktBase
{
};

struct PktRoomChatNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
	wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE] = { 0, };
};
#pragma pack(pop)



