#include "ProudNetActor.h"

#include "EngineUtils.h"
#include "SimpleUE4GameModeBase.h"
#include "../../../Simple/Common/Simple_common.cpp"
#include "../../../Simple/Common/Simple_proxy.cpp"
#include "../../../Simple/Common/Simple_stub.cpp"

// RMI �۽��� ���� ������� ���Ͻ� ��ü�Դϴ�.
Simple::Proxy g_simpleProxy;

// RMI ������ ���� ������� ���� ��ü�Դϴ�.
SimpleStub g_simpleStub;

// P2P �׷쿡 join�ϰ� �Ǹ�, join�� �׷��� HostID�� �� ���� ������ ����Ǿ����ϴ�.
Proud::HostID g_recentP2PGroupHostID = Proud::HostID_None;

// ȭ�鿡 ������ �޼����Դϴ�.
Proud::String g_message = _PNT("");

// �ϳ��� P2P �׷� ���� �Ǿ�� ���� ä���� ���� RMI �����Լ��Դϴ�.
DEFRMI_Simple_P2PChat(SimpleStub)
{
	std::wstringstream tstrstream;

	tstrstream << "[Client] P2PChat: relayed="
		<< (rmiContext.m_relayed ? "true" : "false")
		<< " a=" << a.GetString()
		<< " b=" << b
		<< " c=" << c;

	g_message += Proud::String(tstrstream.str()) + LINE_TERMINATOR;

	// ����� RMI �����Լ������� ���������� true�� ��ȯ�ؾ� �˴ϴ�.
	// ��ȯ������ �ǹ̰� �����ϴ�.
	return true;
}

// ������ �����ִ� ä�� echo �޼����� ���� RMI �����Լ��Դϴ�.
DEFRMI_Simple_ShowChat(SimpleStub)
{
	std::wstringstream tstrstream;

	tstrstream << "[Client] ShowChat: a=" << a.GetString() << ",b=" << b << ",c=" << c;

	g_message += Proud::String(tstrstream.str()) + LINE_TERMINATOR;

	// ����� RMI �����Լ������� ���������� true�� ��ȯ�ؾ� �˴ϴ�.
	// ��ȯ������ �ǹ̰� �����ϴ�.
	return true;
}

// ������ ���� �޼����� ���� RMI �����Լ��Դϴ�.
DEFRMI_Simple_SystemChat(SimpleStub)
{
	std::wstringstream tstrstream;

	tstrstream << "[Client] SystemChat: txt=" << txt.GetString();

	g_message += Proud::String(tstrstream.str()) + LINE_TERMINATOR;

	// ����� RMI �����Լ������� ���������� true�� ��ȯ�ؾ� �˴ϴ�.
	// ��ȯ������ �ǹ̰� �����ϴ�.
	return true;
}

AProudNetActor::AProudNetActor()
	: m_pNetClient(Proud::CNetClient::Create())
{
	// Tick �Լ��� �� �����Ӹ��� ȣ��� �� �ֵ��� ���ݴϴ�.
	PrimaryActorTick.bCanEverTick = true;
}

// ������ ���۵� ���� Ȥ�� �� Actor�� Spawn�� ���Ŀ� ȣ��Ǿ����ϴ�.
void AProudNetActor::BeginPlay()
{
	Super::BeginPlay();

	// ������ Ŀ�ؼ��� �ξ������� ���θ� �����ϰ� �ֽ��ϴ�.
	bool isConnected = false;

	// �������� Ŀ�ؼ� �õ��� �Ϸ�Ǹ� ȣ��Ǿ����ϴ�.
	m_pNetClient->OnJoinServerComplete =
		[&](Proud::ErrorInfo* info, const Proud::ByteArray& replyFromServer)
	{
		if (info->m_errorType == Proud::ErrorType::Ok)
		{
			// �������� Ŀ�ؼǿ� �����Ͽ����ϴ�.
			printf("Succeed to connect server. Allocated hostID=%d\n", m_pNetClient->GetLocalHostID());
			isConnected = true;

			// ������ �޼����� �����ϴ�.
			g_simpleProxy.Chat(Proud::HostID_Server, // �������� HostID�Դϴ�.
				Proud::RmiContext::ReliableSend, // Reliable Ȥ�� UnReliable�� �����Ͻ� �� �� �ֽ��ϴ�.
				_PNT("Hello ProudNet~!!!."), 333, 22.33f); // PIDL ������ �Լ� �ñ״��ķ� �����Ͻ� �� �ִ� ������ ������ �Ķ���͵��Դϴ�.

		}
		else
		{
			// �������� Ŀ�ؼǿ� �����Ͽ����ϴ�.
			std::cout << "Failed to connect to server.\n";
		}
	};

	// �������� Ŀ�ؼ��� �������� ȣ��Ǿ����ϴ�.
	m_pNetClient->OnLeaveServer = [&](Proud::ErrorInfo* errorInfo) {
		// �� Ŀ�ؼ��� ������� ����غ��ϴ�.
		std::cout << "OnLeaveServer: " << Proud::StringT2A(errorInfo->m_comment).GetString() << std::endl;

		// ���� ������ break�ǵ��� �մϴ�.
		isConnected = false;
	};

	// �����ִ� P2P �׷쿡 �ٸ� �Ǿ Join�Ͽ����ÿ� ȣ��Ǿ����ϴ�.
	m_pNetClient->OnP2PMemberJoin = [&](Proud::HostID memberHostID, Proud::HostID groupHostID, int memberCount, const Proud::ByteArray& customField) {
		// memberHostID = �� �ݹ��� ȣ��Ǿ����� ���� ���� join�� �Ǿ��� HostID�Դϴ�.
		// groupHostID = �����ִ� P2P �׷��� HostID�Դϴ�.
		printf("[Client] P2P member %d joined group %d.\n", memberHostID, groupHostID);

		g_simpleProxy.P2PChat(memberHostID, Proud::RmiContext::ReliableSend,
			_PNT("Welcome!"), 5, 7.f);

		g_recentP2PGroupHostID = groupHostID;
	};

	// �����ִ� P2P �׷쿡�� �Ǿ ������ ȣ��Ǿ����ϴ�.
	m_pNetClient->OnP2PMemberLeave = [](Proud::HostID memberHostID, Proud::HostID groupHostID, int memberCount) {
		printf("[Client] P2P member %d left group %d.\n", memberHostID, groupHostID);
	};

	// Ŭ���̾�Ʈ�� ������ ���Ͻ� ��ü�� ���� ��ü�� ������ŵ�ϴ�.
	m_pNetClient->AttachProxy(&g_simpleProxy);
	m_pNetClient->AttachStub(&g_simpleStub);

	Proud::CNetConnectionParam cp;
	cp.m_protocolVersion = g_Version;
	cp.m_serverIP = _PNT("127.0.0.1");
	cp.m_serverPort = g_ServerPort;

	m_pNetClient->Connect(cp);
}

// �� �����Ӹ��� ȣ��Ǿ����ϴ�.
void AProudNetActor::Tick(float deltaTimeSecond)
{
	Super::Tick(deltaTimeSecond);

	m_pNetClient->FrameMove();
}