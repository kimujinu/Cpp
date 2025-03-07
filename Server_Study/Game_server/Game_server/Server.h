#pragma once
#include <memory>

#include "Packet.h"
#include "ErrorCode.h"


namespace NServerNetLib
{
	struct ServerConfig;
	class ILog;
	class ITcpNetwork;
}

class UserManager;
class RoomManager;
class PacketProcess;

class Server
{
public:
	Server();
	~Server();

	ERROR_CODE Init();

	void Run();

	void Stop();


private:
	ERROR_CODE LoadConfig();

	void Release();


private:
	bool m_IsRun = false;

	std::unique_ptr<NServerNetLib::ServerConfig> m_pServerConfig; // 서버설정정보
	std::unique_ptr<NServerNetLib::ILog> m_pLogger; // 로그정보
	std::unique_ptr<NServerNetLib::ITcpNetwork> m_pNetwork; // 네트워크 정보

	std::unique_ptr<PacketProcess> m_pPacketProc; // 패킷프로세스 클래스
	std::unique_ptr<UserManager> m_pUserMgr; // 로그인된 클라이언트의 정보 클래스
	std::unique_ptr<RoomManager> m_pRoomMgr; 
};

