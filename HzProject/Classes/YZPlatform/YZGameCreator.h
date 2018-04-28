#ifndef YZGame_Creator_h__
#define YZGame_Creator_h__

#include "cocos2d.h"
#include "YZBaseType.h"
#include "YZUIExport.h"
#include "YZNetExport.h"

#include <unordered_map>

#define Apple_PlatForm (Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_IPAD || \
	Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_IPHONE || \
	Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_MAC)


typedef std::function<YZGameUIBase* (UINT uDeskId, bool autoCreate)> GAME_CREATE_SELECTOR;

typedef std::function<YZLayer* (Node* parent)> ROOM_CREATE_SELECTOR;

class YZGameCreator
{
public:
	enum GAMETYPE
	{
		UNKNOWN = 0,
		NORMAL,
		BR
	};
	static const BYTE INVALID_PRIORITY = 0xFF;

private:
	struct ItemCreator
	{
		BYTE priority;
		GAMETYPE type;
		UINT uNameId;
		UINT uKindId;
		GAME_CREATE_SELECTOR	gameSelector;
		ROOM_CREATE_SELECTOR	roomSelector;

		ItemCreator() : uNameId(0), uKindId(0), type(UNKNOWN), gameSelector(nullptr), roomSelector(nullptr)
		{

		}

		bool valid()
		{
			return (0 != uNameId) && (0 != uKindId) && (UNKNOWN != type) && (nullptr != gameSelector);
		}
	};

public:
	static YZGameCreator* getInstance();

public:
	// ע����Ϸ
	void addGame(UINT uNameId, UINT uKindId, GAMETYPE type, 
									GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector = nullptr);
	// ��ȡ��Ч����Ϸ
	bool getValidGames(std::vector<ComNameInfo*>* validGames);
	// ��ȡ��Ϸ���ȼ�
	BYTE getGamePriority(UINT uNameId);
	// ���õ�ǰѡ�����Ϸ
	void setCurrentGame(UINT uNameId);
	// ��ǰ��ϷID
	UINT getCurrentGameNameID() const;
	// ��ǰ��Ϸ����
	UINT getCurrentGameKindID() const;
	// ��ȡ��Ϸ����
	GAMETYPE getCurrentGameType() const;

public:
	// ������Ϸ�ͻ���
	bool startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate);
	// ������Ϸ����
	bool startGameRoom(UINT uNameId, Node* parent);

protected:
	// У����Ϸ
	BYTE validGame(UINT uNameId);

private:
	YZGameCreator();
	~YZGameCreator();

private:
	std::unordered_map<UINT, ItemCreator> _creators;
	ItemCreator* _currentCreator;

	LLONG	_basePriority;
};

#define GameCreator()	YZGameCreator::getInstance()

#endif // YZGame_Creator_h__
