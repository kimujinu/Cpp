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

//- �α��� ��û
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


//- �濡 ���� ��û
const int MAX_ROOM_TITLE_SIZE = 16;
struct PktRoomEnterReq
{
	bool IsCreate;
	short RoomIndex;
};

struct PktRoomEnterRes : PktBase
{
};


//- �濡 �ִ� �������� ���� ���� ���� ���� �뺸
struct PktRoomEnterUserInfoNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
};


//- �� ������ ��û
struct PktRoomLeaveReq {};

struct PktRoomLeaveRes : PktBase
{
};

//- �濡�� ������ ���� �뺸(�ٸ� ��������)
struct PktRoomLeaveUserInfoNtf
{
	char UserID[MAX_USER_ID_SIZE] = { 0, };
};


//- �� ä��
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



