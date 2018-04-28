#include "ErMahjongGameTableUI.h"
#include "ErMahjongGameTableLogic.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "YZUIExport.h"
#include "HallLayer.h"
#include "GameDesk.h"
#include "GamePrompt.h"
#include "FontConfig.h"
#include "ErMahjongCardOutList.h"
#include "ErMahjongCardListBoard.h"
#include "Util/JzLog.h"
#include "FTPFileManager.h"
#include "Util/Jzlog.h"
#include "Util/DateUtils.h"
#include "SimpleAudioEngine.h"
#include "../../HelpLayer.h"
#include "UI/HttpSprite.h"
#include "SocketErrorLayer.h"
#include "../../ShareLayer.h"

#define MAJIANG_TAG_SHAIZI1		5000   //˜ª◊”1
#define MAJIANG_TAG_SHAIZI2		50001	//˜ª◊”2

#define ERMAJANG_TAG_BTNWANFA    1000
#define ERMAJANG_TAG_BTNREADY    1001
#define ERMAJANG_TAG_BTNBUYDI    1002
#define ERMAJANG_TAG_BTNDINGDI   1003
#define ERMAJANG_TAG_BTNCANCLE   1004
#define ERMAJANG_TAG_BTNVoice    1005
#define ERMAJANG_TAG_BTNCAUTO    1006
#define ERMAJANG_TAG_BTNRIGHT    1007
#define ERMAJANG_TAG_BTNLEFT     1008
#define ERMAJANG_TAG_BTNSET      1009
#define ERMAJANG_TAG_BTNCHAT     1010
#define ERMAJANG_TAG_BTNCCHAT    1011
#define ERMAJANG_TAG_BTNCSET     1012
#define ERMAJANG_TAG_BTN_SENDMSG 1013
#define ERMAJANG_TAG_BTNGOON     1014
#define ERMAJANG_TAG_BTNINVITE   1005
#define ERMAJANG_TAG_BTNCHATSEND   1015


#define ERMAJANG_TAG_BTNPASS     2001
#define ERMAJANG_TAG_BTNCHI      2002
#define ERMAJANG_TAG_BTNPENG     2003
#define ERMAJANG_TAG_BTNGANG     2004
#define ERMAJANG_TAG_BTNWIN      2005
#define ERMAJANG_TAG_BTNTING     2006

#define ERMAJANG_TAG_BTNCHIT    3001
#define ERMAJANG_TAG_BTNCHIZ    3002
#define ERMAJANG_TAG_BTNCHIW    3003

#define ERMAJANG_TAG_LAYOUTRESULT   4000
#define ERMAJANG_TAG_BTNRESULT_CONTINUE   4001
#define ERMAJANG_TAG_BTNRESULT_CHANGE  4002

#define Tag_Disband             212
#define Tag_Disband2             219

#define TAG_TURE_DISBAND        217
#define TAG_CANEL_DISBAND       218

#define Tag_NoAgree_Disband     213
#define Tag_Agree_Disband       214

#define TAG_BTN_PTLANGUAGE      215
#define TAG_BTN_WHLANGUAGE      216

#define TAG_SHARE_IS_OPEN 1

#define TAG_USE_TWO_LANGUAGE 0


#define FileName_Chat_Face		"erMaJiang/chat/expressionanim"

static const char* GAME_SOUND_MAN_PENG				= "erMaJiang/music/playmahs/man_peng.mp3";
static const char* GAME_SOUND_MAN_CHI				= "erMaJiang/music/playmahs/man_chi.mp3";
static const char* GAME_SOUND_MAN_GANG				= "erMaJiang/music/playmahs/man_gang.mp3";
static const char* GAME_SOUND_MAN_HU				= "erMaJiang/music/playmahs/man_hu.mp3";

static const char* GAME_SOUND_WOMAN_PENG			= "erMaJiang/music/playmahs/woman_peng.mp3";
static const char* GAME_SOUND_WOMAN_CHI				= "erMaJiang/music/playmahs/woman_chi.mp3";
static const char* GAME_SOUND_WOMAN_GANG			= "erMaJiang/music/playmahs/woman_gang.mp3";
static const char* GAME_SOUND_WOMAN_HU				= "erMaJiang/music/playmahs/woman_hu.mp3";

static const char* GAME_BGM_MUSIC				= "erMaJiang/music/bgm1.mp3";

static const char* GAME_SOUND_READY_BOY = "erMaJiang/music/boy_ready.mp3";
static const char* GAME_SOUND_READY_GIRL = "erMaJiang/music/girl_ready.mp3";

static const char* GAME_SOUND_OUT_CARD ="erMaJiang/music/outCard.mp3";

const char* Player_Normal_M  = "erMaJiang/DisplayPicture/man.png";
const char* Player_Normal_W  = "erMaJiang/DisplayPicture/woman.png";

#define CHAT_BG_LEFT  "erMaJiang/chat_bubble_left.png"
#define CHAT_BG_RIGHT  "erMaJiang/chat_bubble_right.png"


const char* IMAGE_BTN_EXIT = "erMaJiang/buttons/btn_exit.png";
const char* IMAGE_BTN_DISBAND = "erMaJiang/buttons/btn_js.png";

const char* IMAGE_BTN_PTLANGUAGE_N = "erMaJiang/buttons/btn-putong.png";
const char* IMAGE_BTN_PTLANGUAGE_S = "erMaJiang/buttons/btn-putong_d.png";

const char* IMAGE_BTN_WHLANGUAGE_N = "erMaJiang/buttons/btn-wuhu.png";
const char* IMAGE_BTN_WHLANGUAGE_S = "erMaJiang/buttons/btn-wuhu_d.png";

#define IMAGE_BTN_READLY  "erMaJiang/btn_start.png"
#define IMAGE_BTN_INVITE  "erMaJiang/btn_invite.png"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../../WxSDK/WxSDKManager.h"
#include "../../WeChatManager.h"
#include "SystemInfoManager.h"
#endif


namespace ErMahjong
{

	GameTableUI* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUI* tableUI = new GameTableUI();
		if (tableUI->init(bDeskIndex, bAutoCreate))
		{
			tableUI->autorelease();
			return tableUI;

		}
		else
		{
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if (!YZLayer::init())
		{
			return false;
		}
        
        _instance = this;
        tempTime = 0;
        _lastOutCartSeatNo = -1;
        bDarkGuangSeatNo = -1;
        m_iNowGameCount = 0;
        bDarkGuangCard = 0x00;
        _bankerFirstAction = 0;
        _bankerFirstActionCard = 0;
        _isLastGame = false;
        m_bHaveGangAction = false;
        m_bGangActionEnd = false;
        playerCount = 0;
        _lastCardCount = 112;
        _leftShow = false;
        canChat = true;
		voicesList.clear();
        _gameStart = false;
        _isPlayVoice = false;
        _canRecording = true;
        //TextureCache::getInstance()->removeUnusedTextures();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
        RoomLogic()->clearDeskOwnerInfo();
        
		Size size = Director::getInstance()->getVisibleSize();
	    this->ignoreAnchorPointForPosition(false);
        this->setPosition(size.width / 2, size.height / 2);
        
	    loadLayout();

	    _tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
	    _tableLogic->enterGame();
        
        //自动准备
        if (playerCount < PLAYER_COUNT) {
            _btnReady->setTag(ERMAJANG_TAG_BTNINVITE);
            _btnReady->loadTextures(IMAGE_BTN_INVITE, IMAGE_BTN_INVITE);
             _btnReady->setVisible(true);
            if(PlatformLogic()->loginResult.bClose)
            {
                 _btnReady->setVisible(false);
            }
           
        }else{
            _btnReady->setVisible(false);
        }
        _tableLogic->sendShowStart();
        _imgReady[0]->setVisible(true);
        
        auto btnDiaband  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableWidget, "btn_disband"));
        if (_tableLogic->checkUserIsDeskOwner()) {
            btnDiaband->loadTextures(IMAGE_BTN_DISBAND, IMAGE_BTN_DISBAND);
        }else{
            btnDiaband->loadTextures(IMAGE_BTN_EXIT, IMAGE_BTN_EXIT);
        }
    
        YZAudioEngine::getInstance()->playBackgroundMusic(GAME_BGM_MUSIC , true);
        
		this->scheduleUpdate();
		
        soundUtil = SoundRecordUtil::create();
        soundUtil->InitSoundRecordUtil(CC_CALLBACK_1(GameTableUI::uploadFileEnd, this), CC_CALLBACK_1(GameTableUI::downloadFileEnd, this));
        this->addChild(soundUtil);
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","openListensSystemInfo", "()V");
        
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }
        #endif
        
        /*
         //FOR TEST
         _caiShenCard = 0x04;
         this->showMahjongLayout( _caiShenCard, true);
         
         std::vector<BYTE> values;
         
         values.push_back(0x01);
         values.push_back(0x02);
         values.push_back(0x03);
         values.push_back(0x04);
         values.push_back(0x05);
         values.push_back(0x06);
         values.push_back(0x07);
         values.push_back(0x09);
         
         for (int i = 0; i< 5; i++) {
         BYTE card = 0x09;
         values.push_back(card);
         }
         
         BYTE mySeatNo = 0;
         
         this->showAllUserCard(mySeatNo,values , mySeatNo);
        */
        /*
        MJGameEndData data;
        data.bLiuJu = false;
        data.bZiMo = false;
        data.cbHuCard[0] = 0;
        data.cbHuCard[1] = 0x01;
        data.cbHuCard[2] = 0x01;
        data.cbHuCard[3] = 0x01;
        data.cbZaCardData.push_back(0x01);
        data.cbZaCardData.push_back(0x02);
        data.cbZhongZaCardData.push_back(0x01);
        memset(data.iChangeMoney, 0, sizeof(data.iChangeMoney));
        
        std::map<BYTE, std::vector<BYTE>> actionCardInfoMap;
        actionCardInfoMap.clear();
        std::vector<BYTE> actionValues;
        actionValues.clear();
        actionCardInfoMap.insert(make_pair(0, actionValues));
        actionCardInfoMap.insert(make_pair(1, actionValues));
        actionCardInfoMap.insert(make_pair(2, actionValues));
        actionCardInfoMap.insert(make_pair(3, actionValues));
        
        std::map<BYTE , std::vector<BYTE>> cardInfoMap;
        cardInfoMap.insert(make_pair(0, values));
        cardInfoMap.insert(make_pair(1, values));
        cardInfoMap.insert(make_pair(2, values));
        cardInfoMap.insert(make_pair(3, values));
        this->doGameEndResp(data, actionCardInfoMap, cardInfoMap);
        */
        //this->showActionBtn(0, WIK_LEFT, 0x06);
        
        //this->showActionAnimation(0, ActionType::WIK_CHI_HU);
        
        return true;
	}
    
    void GameTableUI::changeRoomUIByPlayCount(int nowGameCount){
        Size winSize = Director::getInstance()->getVisibleSize();
        
        if (nowGameCount == 0) {
        
            Point playerPoint[4];
            playerPoint[0] = Point(winSize.width / 2 - 50, 20);
            playerPoint[1] = Point(winSize.width - 100 , _tableWidget->getContentSize().height / 2);
            playerPoint[2] = Point(winSize.width/2 - 50 , _tableWidget->getContentSize().height - 100);
            playerPoint[3] = Point(20 , winSize.height / 2 );
            
            for(int i = 0; i < PLAYER_COUNT; i++)
            {
                _players[i]->setPosition(playerPoint[i]);
                _players[i]->setVisible(true);
            }
            
            _btnReady->setPosition(Vec2(winSize.width /2 , winSize.height/2));
            _btnSet->setVisible(false);
            _btnChat->setVisible(false);
            _btnDiaband->setVisible(true);

        }else{
            
            if (nowGameCount != 1) {
                Point playerPoint[4];
                playerPoint[0] = Point(0 , winSize.height * .2);
                playerPoint[1] = Point(winSize.width - 100 , _tableWidget->getContentSize().height * .61);
                playerPoint[2] = Point(winSize.width * .78f , _tableWidget->getContentSize().height * .83);
                playerPoint[3] = Point(0 , winSize.height * .65);
                
                for(int i = 0; i < PLAYER_COUNT; i++)
                {
                    _players[i]->setPosition(playerPoint[i]);
                    _players[i]->setVisible(true);
                }
            }

            _btnReady->setVisible(false);
            _btnSet->setVisible(true);
            _btnChat->setVisible(true);
            _btnDiaband->setVisible(false);
        }
        
    }
	
	void GameTableUI::updateDeskOwnerId(int userId){
        for (int i = 0; i < PLAYER_COUNT; i++) {
            _players[i]->setDeskOwnerID(userId);
        }
    }
	
	void GameTableUI::onEnter()
	{
		YZGameUIBase::onEnter();
	}

	void GameTableUI::onExit()
	{
        if (socketErrorLayer != NULL) {
            socketErrorLayer->removeFromParentAndCleanup(true);
            socketErrorLayer = NULL;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","stopListensSystemInfo", "()V");
        
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }
#endif

        
		YZGameUIBase::onExit();
	}

	void GameTableUI::addUser(BYTE seatNo, bool bMe) 
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setVisible(true);
	    _players[seatNo]->setUserId(_tableLogic->getUserId(seatNo));
        _players[seatNo]->setDeskOwnerID(_tableLogic->getDeskOwner());
	    _players[seatNo]->setHead(_tableLogic->getUserByDeskStation(seatNo));
        
        playerCount++;
        
        if ( playerCount == PLAYER_COUNT && _tableLogic->getUserIsAgree(seatNo)) {
            _btnReady->setVisible(false);
        }
        
	}
	void GameTableUI::removeUser(BYTE seatNo, bool bMe)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setVisible(false);
	    _players[seatNo]->setUserId(INVALID_USER_ID);
        _imgReady[seatNo]->setVisible(false);
        
        playerCount--;
	}

	void GameTableUI::setUserName(BYTE seatNo, const std::string& name)
	{
		if (!isValidSeat(seatNo))	return;
	    _players[seatNo]->setUserName(name);
	}

	void GameTableUI::showUserMoney(BYTE seatNo, bool visible)
	{
		if (!isValidSeat(seatNo))	return;
	    _players[seatNo]->showMoney(visible);
	}

	void GameTableUI::setUserMoney(BYTE seatNo, INT money)
	{
		if (!isValidSeat(seatNo))	return;
        _players[seatNo]->changeUserScore(money);
	}
    
	void GameTableUI::setUserHead(BYTE seatNo) 
	{
		if(!isValidSeat(seatNo))	return;
		_players[seatNo]->setHead(_tableLogic->getUserByDeskStation(seatNo));
	}

	void GameTableUI::showUserReady(BYTE seatNo,bool boy,bool visible)
	{
		_imgReady[seatNo]->setVisible(visible);
        
	}
    
    void GameTableUI::playUserReadyEffect(bool isBoy){
        if (isBoy) {
            YZAudioEngine::getInstance()->playEffect(GAME_SOUND_READY_BOY);
        }else{
            YZAudioEngine::getInstance()->playEffect(GAME_SOUND_READY_GIRL);
        }
    }
    

	void GameTableUI::setBanker(BYTE seatNo,bool visible)
	{
		if(!isValidSeat(seatNo))	return;
		_players[seatNo]->setBanker(visible);
	}

	void GameTableUI::showTingCard(BYTE seatNo,bool visible)
	{
		if(!isValidSeat(seatNo))	return;
		_players[seatNo]->showTingCard(visible);
	}

	void GameTableUI::showReadyBtn( bool visible)
	{
		_btnReady->setVisible(visible);
	}
    
    BYTE GameTableUI::getPlayerGuangCard(BYTE seatNO){
        return  _cardListBoard[seatNO]->getGuangCardValue();
    }


	void GameTableUI::showActionBtn(BYTE seatNo , BYTE actionWik , BYTE card)
	{        
        JzLog::shareLog()->log(kLogLevelDebug, "显示事件按钮...Wik : %d  Card : %d" , actionWik,card);
        
        _bankerSeatNo = seatNo;
        _bankerCard = card;
        _cardListBoard[seatNo]->setCanOutCard(false);
        
        if ((actionWik & ActionType::WIK_PENG) != 0) {
            this->checkActionBtnShow(ActionType::WIK_PENG , card);
        }
        
        if ((actionWik & ActionType::WIK_LEFT) != 0) {
            this->checkActionBtnShow(ActionType::WIK_LEFT , card);
        }

        if ((actionWik & ActionType::WIK_CENTER) != 0) {
            this->checkActionBtnShow(ActionType::WIK_CENTER , card);
        }
        
        if ((actionWik & ActionType::WIK_RIGHT) != 0) {
            this->checkActionBtnShow(ActionType::WIK_RIGHT , card);
        }
        
        if ((actionWik & ActionType::WIK_GANG) != 0) {
            this->checkActionBtnShow(ActionType::WIK_GANG , card);
        }
        
        if ((actionWik & ActionType::WIK_CHI_HU) != 0) {
            this->checkActionBtnShow(ActionType::WIK_CHI_HU , card);
        }
        
        _btnType[0]->setVisible(true);
        
    }
    
    //玩家点击了按钮，选择了操作动作调用
    void GameTableUI::showActionCard(MJActionData data,ActionType actionType){
        
        this->hideAllAcitonBtn();
        
        if (actionType == ActionType::WIK_NULL) {
            if (data.acSeatNo == 0 && data.outSeatNo == 0) {
                _cardListBoard[0]->setCanOutCard(true);
            }
            return;
        }
        
        this->playActionSound(data.acSeatNo, actionType);
        this->showActionAnimation(data.acSeatNo,actionType);
        
        if (actionType == ActionType::WIK_CHI_HU) {
            CCLOG("GameOver...");
        }else{
            
            if (isValidSeat(_lastOutCartSeatNo)) {
                _cardOutList[_lastOutCartSeatNo]->removeLastCard();
            }
            
            if (data.bDarkGuang) {
                waitingForAnGuang = true;
                bDarkGuangSeatNo = data.acSeatNo;
                bDarkGuangCard = data.card;
            }
            
            if (data.acSeatNo == 0) {
                
                if (actionType == ActionType::WIK_GANG) {
                    gangActionCard.clear();
                    m_bHaveGangAction = true;
                    gangActionSeatNo = data.acSeatNo;
                }
            }
            _cardListBoard[data.acSeatNo]->doUserOperateCard(data.bDarkGuang,actionType, data.card);
        }
    }
    
    void GameTableUI::playActionSound(BYTE seatNo , ActionType aType){
        
        if(!isValidSeat(seatNo)){
            return;
        }
        
        bool isBoy = false;
        
        if (_players[seatNo] ->isVisible() && _players[seatNo]->getIsBoy()) {
            isBoy = true;
        }
        
        if (isBoy) {
            switch (aType) {
                case WIK_PENG:
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("man_peng.mp3"));
                    break;
                case WIK_CHI_HU:
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_HU);
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("man_hu.mp3"));
                    break;
                case WIK_LEFT:
                    break;
                case WIK_CENTER:
                    break;
                case WIK_RIGHT:
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_CHI);
                    break;
                case WIK_GANG:
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("man_gang.mp3"));
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_GANG);
                break;
            }
        }else{
            switch (aType) {
                case WIK_PENG:
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("woman_peng.mp3"));
                    break;
                case WIK_CHI_HU:
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_HU);
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("woman_hu.mp3"));
                    break;
                case WIK_LEFT:
                    
                case WIK_CENTER:
                    
                case WIK_RIGHT:
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_CHI);
                    break;
                case WIK_GANG:
                    YZAudioEngine::getInstance()->playEffect(FormatSoundFile("woman_gang.mp3"));
                    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_MAN_GANG);
                    break;
            }
        }
    }
    
    Vec2 GameTableUI::getAnimatePoint(BYTE seatNo){
        Size winSize = Director::getInstance()->getWinSize();
        Vec2 point = Vec2::ZERO;
        
        switch (seatNo) {
            case 0:
                point = Vec2(winSize.width/2, winSize.height * .3f);
                break;
            case 1:
                point = Vec2(winSize.width * .75f, winSize.height/2);
                break;
            case 2:
                point = Vec2(winSize.width/2, winSize.height * .75f);
                break;
            case 3:
                point = Vec2(winSize.width * .2f, winSize.height/2);
                break;
        }
        
        return point;
    }

    void GameTableUI::showActionAnimation(BYTE seatNo ,ActionType aType){
        Size size  = Director::getInstance()->getVisibleSize();
        switch (aType) {
            case ActionType::WIK_PENG:
            {
                Sprite* pSprite = NULL;
                Animation* pAnimation = Animation::create();
                for (int i = 1; i <= 10; i++)
                {
                    char strImagePath[128] = { 0 };
                    sprintf(strImagePath, "erMaJiang/anim/peng/peng_%d.png", i);
                    if (1 == i)
                    {
                        pSprite = Sprite::create(strImagePath);
                        pSprite->setPosition(getAnimatePoint(seatNo));
                        pSprite->setLocalZOrder(85);
                        pSprite->setScale(1.5f);
                        pSprite->setName("btnAction");
                        this->addChild(pSprite , 85);
                    }
                    pAnimation->addSpriteFrameWithFile(strImagePath);
                }
                pAnimation->setDelayPerUnit(1.5f / 10.0f);
                pAnimation->setRestoreOriginalFrame(true);
                
                Animate* pAnimate = Animate::create(pAnimation);

                pSprite->runAction(Sequence::create(pAnimate , CallFunc::create([=]{pSprite->removeFromParentAndCleanup(true);}) , NULL));
                
            }
                break;
                
            case ActionType::WIK_GANG:
            {
                Sprite* pSprite = NULL;
                Animation* pAnimation = Animation::create();
                for (int i = 1; i <= 11; i++)
                {
                    char strImagePath[128] = { 0 };
                    sprintf(strImagePath, "erMaJiang/anim/gang/gang_%d.png", i);
                    if (1 == i)
                    {
                        pSprite = Sprite::create(strImagePath);
                        pSprite->setPosition(getAnimatePoint(seatNo));
                        pSprite->setLocalZOrder(85);
                        pSprite->setName("btnAction");
                        pSprite->setScale(1.5f);
                        this->addChild(pSprite , 85);
                    }
                    pAnimation->addSpriteFrameWithFile(strImagePath);
                }
                pAnimation->setDelayPerUnit(1.5f / 10.0f);
                pAnimation->setRestoreOriginalFrame(true);
                
                Animate* pAnimate = Animate::create(pAnimation);
                
                pSprite->runAction(Sequence::create(pAnimate , CallFunc::create([=]{pSprite->removeFromParentAndCleanup(true);}) , NULL));
            }
                break;
                
            case ActionType::WIK_CHI_HU:
            {
                Sprite* pSprite = NULL;
                Animation* pAnimation = Animation::create();
                for (int i = 1; i <= 11; i++)
                {
                    char strImagePath[128] = { 0 };
                    sprintf(strImagePath, "erMaJiang/anim/hu/hu_%d.png", i);
                    if (1 == i)
                    {
                        pSprite = Sprite::create(strImagePath);
                        pSprite->setPosition(getAnimatePoint(seatNo));
                        pSprite->setLocalZOrder(85);
                        pSprite->setScale(1.5f);
                        pSprite->setName("btnAction");
                        this->addChild(pSprite , 85);
                    }
                    pAnimation->addSpriteFrameWithFile(strImagePath);
                }
                pAnimation->setDelayPerUnit(1.5f / 10.0f);
                pAnimation->setRestoreOriginalFrame(true);
                
                Animate* pAnimate = Animate::create(pAnimation);
                
                pSprite->runAction(Sequence::create(pAnimate , CallFunc::create([=]{pSprite->removeFromParentAndCleanup(true);}) , NULL));
            }
                break;
                
            case ActionType::WIK_LEFT:
                
            case ActionType::WIK_CENTER:
            
            case ActionType::WIK_RIGHT:
            {
                Sprite* pSprite = NULL;
                Animation* pAnimation = Animation::create();
                for (int i = 1; i <= 15; i++)
                {
                    char strImagePath[128] = { 0 };
                    sprintf(strImagePath, "erMaJiang/anim/chi/chi_%d.png", i);
                    if (1 == i)
                    {
                        pSprite = Sprite::create(strImagePath);
                        pSprite->setPosition(size.width / 2, size.height/2);
                        pSprite->setLocalZOrder(85);
                        pSprite->setName("btnAction");
                        this->addChild(pSprite , 85);
                    }
                    pAnimation->addSpriteFrameWithFile(strImagePath);
                }
                pAnimation->setDelayPerUnit(1.0f / 10.0f);
                pAnimation->setRestoreOriginalFrame(true);
                
                Animate* pAnimate = Animate::create(pAnimation);
                
                pSprite->runAction(Sequence::create(pAnimate , CallFunc::create([=]{pSprite->removeFromParentAndCleanup(true);}) , NULL));
            }

                break;
                
            default:
                break;
        }
    
    }
    
    
    void GameTableUI::changeImageTexture(ImageView * image , BYTE cardValue){
    
        BYTE cbColor=(cardValue&MASK_COLOR)>>4;
        BYTE cbValue=(cardValue&MASK_VALUE);
        char filename[60];
        switch (cbColor)
        {
            case MJColor::Wan:
                sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                break;
            case MJColor::Tiao:
                sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                break;
            case MJColor::Bing:
                sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                break;
            case MJColor::Feng:
                sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                break;
        }

        image->loadTexture(filename);
    }
    
    
    void GameTableUI::checkActionBtnShow(BYTE actionType, BYTE cardValue){
        switch (actionType) {
            case ActionType::WIK_LEFT:
                _btnType[1]->setVisible(true);
                _btnChiType[0]->setVisible(true);
                _imgChiType[0]->setVisible(true);
                
                for (int i = 1; i<= 3; i++) {
                    char strImageName[128] = { 0 };
                    sprintf(strImageName, "Image_chow%d", i);
                    auto image =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[0],strImageName));
                    
                    this->changeImageTexture(image, cardValue + (i-1));
                }
                
                break;
                
            case ActionType::WIK_CENTER:
                _btnType[1]->setVisible(true);
                _btnChiType[1]->setVisible(true);
                _imgChiType[1]->setVisible(true);
                
                for (int i = 1; i<= 3; i++) {
                    char strImageName[128] = { 0 };
                    sprintf(strImageName, "Image_chow%d", i);
                    auto image =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[1],strImageName));
                    
                    this->changeImageTexture(image, i == 1 ? cardValue - 0x01 : i == 2 ? cardValue : cardValue + 0x01);
                }

                
                break;
            case ActionType::WIK_RIGHT:
                _btnType[1]->setVisible(true);
                _btnChiType[2]->setVisible(true);
                _imgChiType[2]->setVisible(true);
                
                
                for (int i = 3; i > 0 ; i--) {
                    char strImageName[128] = { 0 };
                    sprintf(strImageName, "Image_chow%d", i);
                    auto image =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[2],strImageName));
                    
                    this->changeImageTexture(image, cardValue - (3-i));
                }

                
                break;
                
            case ActionType::WIK_PENG:
                _btnType[2]->setVisible(true);
                break;
                
                
            case ActionType::WIK_GANG:
                _btnType[3]->setVisible(true);
                break;
                
            case ActionType::WIK_LISTEN:
                _btnType[5]->setVisible(true);
                break;
                
            case ActionType::WIK_CHI_HU:
                _btnType[4]->setVisible(true);
                break;
        }
    }
    
    void GameTableUI::hideAllAcitonBtn(){
        for (int i = 0; i < 6; i++)
        {
            _btnType[i]->setVisible(false);
        }
        
        _PanelChi->setVisible(false);
        
        for (int i = 0; i< 3; i++) {
            _btnChiType[i]->setVisible(false);
            _imgChiType[i]->setVisible(false);
        }
    }
    
	void GameTableUI::showMahjongLayout(BYTE cardValue , bool visible)
	{
    
    }
    
    void GameTableUI::setTureCaiShenCard(BYTE card){
        _caiShenCard = card;
    }

    
#pragma mark 显示财神牌
    
	void GameTableUI::showCaiShenCard(BYTE cardValue,bool visible)
	{
		auto canshen_bg= dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "canshen_bg"));
		auto Image_value= dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "Image_value"));

		if (canshen_bg == NULL || Image_value == NULL)
		{
			return;
		}

        canshen_bg->setVisible(visible);
        
        if (!visible) {
            return;
        }
        
        BYTE cbColor=(cardValue&MASK_COLOR)>>4;
        BYTE cbValue=(cardValue&MASK_VALUE);
		char filename[60];
        switch (cbColor)
        {
            case MJColor::Wan:
                sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                break;
            case MJColor::Tiao:
                sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                break;
            case MJColor::Bing:
                sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                break;
            case MJColor::Feng:
                sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                break;
        }
        Image_value->loadTexture(filename);

    }

	void GameTableUI::setWaitTime(BYTE seatNo, BYTE time, bool visible,int statues)
	{
		if(isValidSeat(seatNo))
		{
			_currentStatues=statues;
			if (_currentStatues==3 &&_resultWidget)
			{
				
			}
			else
			{
				auto panel_gameTime = dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget, "Panel_clock"));
				panel_gameTime->setVisible(visible);
				if(visible)
				{
					auto label = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(panel_gameTime, "AtlasLabel_timeCount"));
					if(label != nullptr)
					{
                        BYTE logicSeatNo = _tableLogic->getLogicViewSeatNoByLocal(seatNo);
                        
						auto image_top = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(panel_gameTime, "Image_dir3_up"));
						auto image_down = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(panel_gameTime, "Image_dir1_up"));
						auto image_left = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(panel_gameTime, "Image_dir2_up"));
						auto image_right = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(panel_gameTime, "Image_dir4_up"));

                        
						switch (logicSeatNo)
						{
							case 0:
                                
                                image_top->setVisible(false);
                                image_left -> setVisible(true);
                                image_right -> setVisible(false);
                                image_down->setVisible(false);
                                
                                
                                image_top->stopAllActions();
                                image_down->stopAllActions();
                                image_right->stopAllActions();
                                //image_left->runAction(RepeatForever::create(Blink::create(1, 2)));
                                
                                
							break;

							case 1:
                                image_top->setVisible(false);
                                image_left -> setVisible(false);
                                image_down -> setVisible(true);
                                image_right->setVisible(false);
                                
                                image_top->stopAllActions();
                                image_left->stopAllActions();
                                image_right->stopAllActions();
                                //image_down->runAction(RepeatForever::create(Blink::create(1, 2)));

								break;
						
							case 2:
                               
                                image_top->setVisible(false);
                                image_left -> setVisible(false);
                                image_right -> setVisible(true);
                                image_down->setVisible(false);
                                
                                image_top->stopAllActions();
                                image_left->stopAllActions();
                                image_down->stopAllActions();
                                //image_right->runAction(RepeatForever::create(Blink::create(1, 2)));

								break;

							case 3:
                              
                                image_left->setVisible(false);
                                image_right->setVisible(false);
                                image_down->setVisible(false);
                                image_top -> setVisible(true);
                                
                                image_right->stopAllActions();
                                image_left->stopAllActions();
                                image_down->stopAllActions();
                                //image_top->runAction(RepeatForever::create(Blink::create(1, 2)));
								break;
						}
                        char cTime[20] = {0};
						sprintf(cTime, "%d", time);
						label->setTag(time);
						label->setString(cTime);
						_timerText = label;
						startTimer();
					}
				}
				else
				{
					stopTimer();
					_timerText = nullptr;
				}
			}
		}
		
	}

	void GameTableUI::showAutoOutCardBtn(bool visible)
	{
	}

	void GameTableUI::setAutoOutCardBtn(bool visible)
	{
	}

	void GameTableUI::showCAutoOutCardBtn(bool visible)
	{
		_cancleAutobg->setVisible(visible);
	}

	void GameTableUI::showUserHandCard(BYTE seatNo, const std::vector<BYTE> values)
	{
		if (isValidSeat(seatNo) && _players[seatNo] != nullptr)
		{
			if(_cardListBoard[seatNo] == nullptr)
			{
				addHandCardList(seatNo);
			}
            
            _cardListBoard[seatNo]->setCaiShenCard(_caiShenCard);
            _cardListBoard[seatNo]->addCards(values);
		}
    }
    
	void GameTableUI::showAllUserCard(BYTE seatNo, std::vector<BYTE> values , BYTE bankerCard){
    
        /*
		for (int i = 0; i < PLAYER_COUNT; i++)
        {
            this->showUserHandCard(i , values);
            this->showUserHandCardOut(i);
        }*/
        
        _bankerCard = bankerCard;
        _bankerSeatNo = seatNo;
        _curGameBanker = seatNo;
    }
    
#pragma mark 恢复游戏
    
    void GameTableUI::changePlayerScore(LLONG  i64TotalScore[GAME_PLAYER]){
        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            _players[i]->changeUserScore((int)i64TotalScore[i]);
        }
    }
    
    void GameTableUI::playerBackGameFreeState(std::string pwd , int zmCount){
        char roomName[20] = {0};
        sprintf(roomName, "房号 : %s", pwd.c_str());
        label_roomNum->setString(roomName);
        _lastCardCount = 112;
        
        RoomLogic()->setInRoomNum(pwd);
        RoomLogic()->setRoomZMCount(zmCount);
        
        this->changeLastCardCount(_lastCardCount);
        this->setGameZMCount(zmCount);
    }
    
    void GameTableUI::playerBackGame(BackGameData data ,std::map<BYTE , std::vector<BYTE>> cbDusCardMaps ,std::map<BYTE , std::vector<BYTE>> cbHandCardsMaps,std::map<BYTE , std::vector<CMD_WeaveItem>> cbWeaveItemMaps){
        
        clearDesk();
        //恢复游戏
        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            
            if (isValidSeat(i) && _players[i] != nullptr)
            {
                if(_cardListBoard[i] == nullptr)
                {
                    addHandCardList(i);
                }
                
                if(_cardOutList[i] == nullptr)
                {
                    addOutCardList(i);
                }
            }
        
            _cardListBoard[i]->setCaiShenCard(data.cbDaCardData);
            
            //恢复动作牌F
            _cardListBoard[i]->addAllActionCards(cbWeaveItemMaps.at(i));
            
            //恢复手牌
            _cardListBoard[i]->addAllCardOneTimes(cbHandCardsMaps.at(i));
            
            //恢复出牌列表
            _cardOutList[i]->addAllCard2OutListOneTimes(cbDusCardMaps.at(i));
            
            //恢复托管状态
            this->showUserTrustee(i, data.bTrustee[i]);
            
            if (i == data.wBankerUser) {
                _players[i]->setBanker(true);
            }else{
                _players[i]->setBanker(false);
            }
            _players[i]->changeUserScore((int)data.i64TotalScore[i]);
        }
      
        RoomLogic()->setInRoomNum(data.roomPwd);
        RoomLogic()->setRoomZMCount(data.m_cbZaCodeNum);
      
        label_roomNum->setString("房号 : " + data.roomPwd);
        this->setGameZMCount(data.m_cbZaCodeNum);
        
        _lastCardCount = data.cbLeftCardCount;
        this->changeLastCardCount(_lastCardCount);
        
        _curGameBanker = data.wBankerUser;
        this->setWaitTime(data.wCurrentUser, ErMahjong_Game_Time::OUTCARD_TIME, true, 1);
        this->showPlayerGetCard(data.wCurrentUser, data.cbSendCardData);
    }
    
    
    void GameTableUI::GameStart(){
        BYTE randomSeat = CCRANDOM_0_1()*(3-0+1);  //产生一个从start到end间的随机数

        setGameBanker(randomSeat);
    }

	void GameTableUI::setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards)
	{
		if(!isValidSeat(seatNo) || _cardListBoard[seatNo] == nullptr) return;
		//_cardListBoard[seatNo]->upCards(upCards);
	}

	void GameTableUI::getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards)
	{
		if(!isValidSeat(seatNo) || _cardListBoard[seatNo] == nullptr) return;
		//_cardListBoard[seatNo]->getTouchedCards(upCards);

	}

	void GameTableUI::showBtnType(int actCount,BYTE statues)
	{
		char strName[60];
		sprintf(strName,"erMaJiang/btn_type%d.png",statues+1);
		_btnType[actCount-1]->setVisible(true);
		_btnType[actCount-1]->loadTextureNormal(strName);
		_btnType[actCount-1]->setTag(ERMAJANG_TAG_BTNPASS+statues);
	}



	void GameTableUI::hideBtnType()
	{
		for (int i = 0; i < 6; i++)
		{
			_btnType[i]->setVisible(false);
		}
		_PanelChi->setVisible(false);
		for (int i = 0; i < 3; i++)
		{
			_btnChiType[i]->setVisible(false);
		}
		for (int i = 0; i < 3; i++)
		{
			_imgChiType[i]->setVisible(false);
		}
	}

	void GameTableUI::setChiPaiLayout(int type,BYTE cardValues)
	{
		 _index=0;
		switch (type)
		{
		case 1:
			{
				_btnChiType[0]->setVisible(true);
				_btnChiType[1]->setVisible(false);
				_btnChiType[2]->setVisible(false);
				_imgChiType[0]->setVisible(true);
				_imgChiType[1]->setVisible(false);
				_imgChiType[2]->setVisible(false);
				showCardType(1,cardValues);
			}
			break;
		case 2:
			{
				_btnChiType[0]->setVisible(false);
				_btnChiType[1]->setVisible(true);
				_btnChiType[2]->setVisible(false);
				_imgChiType[0]->setVisible(false);
				_imgChiType[1]->setVisible(true);
				_imgChiType[2]->setVisible(false);
				showCardType(2,cardValues);
			}
			break;
		case 3:
			{
				_btnChiType[0]->setVisible(false);
				_btnChiType[1]->setVisible(false);
				_btnChiType[2]->setVisible(true);
				_imgChiType[0]->setVisible(false);
				_imgChiType[1]->setVisible(false);
				_imgChiType[2]->setVisible(true);
				showCardType(3,cardValues);
			}
			break;
		case 4:
			{
				_btnChiType[0]->setVisible(true);
				_btnChiType[1]->setVisible(true);
				_btnChiType[2]->setVisible(false);
				_imgChiType[0]->setVisible(true);
				_imgChiType[1]->setVisible(true);
				_imgChiType[2]->setVisible(false);
				showCardType(1,cardValues);
				showCardType(2,cardValues);
			}
			break;
		case 5:
			{
				_btnChiType[0]->setVisible(true);
				_btnChiType[1]->setVisible(false);
				_btnChiType[2]->setVisible(true);
				_imgChiType[0]->setVisible(true);
				_imgChiType[1]->setVisible(false);
				_imgChiType[2]->setVisible(true);
				showCardType(1,cardValues);
				showCardType(3,cardValues);
			}
			break;
		case 6:
			{
				_btnChiType[0]->setVisible(false);
				_btnChiType[1]->setVisible(true);
				_btnChiType[2]->setVisible(true);
				_imgChiType[0]->setVisible(false);
				_imgChiType[1]->setVisible(true);
				_imgChiType[2]->setVisible(true);
				showCardType(2,cardValues);
				showCardType(3,cardValues);
			}
			break;
		case 7:
			{
				_btnChiType[0]->setVisible(true);
				_btnChiType[1]->setVisible(true);
				_btnChiType[2]->setVisible(true);
				_imgChiType[0]->setVisible(true);
				_imgChiType[1]->setVisible(true);
				_imgChiType[2]->setVisible(true);
				showCardType(1,cardValues);
				showCardType(2,cardValues);
				showCardType(3,cardValues);
			}
			break;
		default:
			break;
		}
		
	}

	void GameTableUI::showResultLayout(bool isVisable)
	{
        _resultWidget->setVisible(isVisable);
	}

	/*
	void GameTableUI::showUserReslutInfor(bool iWin,BYTE seatNo,bool iBanker,char name[61],LLONG iDiFen,int iLianZhuang,WzType wzType,WzHuType wzHuType, tagMaiDing::MdInfo iDiPoint,LLONG iTotalPoint)
	{

		
		char strName[60];
		if (iWin)
		{
			auto label_name = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_resultWidget,"Label_name"));
			auto label_count = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_resultWidget,"Label_count"));
			auto label_score = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_resultWidget,"Label_score"));
			auto label_cardType = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_resultWidget,"Label_cardType"));
			label_name->setString(GBKToUtf8(name));
		    sprintf(strName,"%d",iLianZhuang);
			label_count->setString(strName);
		    sprintf(strName,"%lld",iDiFen);
			label_score->setString(strName);
			auto pUser = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_resultWidget,"player1"));
			auto pName = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_nick"));
			
			pName->setString(GBKToUtf8(name));
			sprintf(strName,"%lld",iDiPoint);
			auto pMaiDi = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_maidi"));
			auto pDingDi = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_dingdi"));
			std::string winType;
			if (wzType==WzType::Wz_Rp)
			{
				winType="»Ì≈∆";
			}
			else if(wzType==WzType::Wz_Yp)
			{
		        winType="”≤≈∆";
			}
			switch (wzHuType)
			{
			case WzHuType::Wz_Not_Hu:
				{
					winType.append(" ");
				}
				break;
			case WzHuType::Wz_Ping_Hu:
				{
					winType.append(" ∆Ω∫˙");
				}
				break;
			case WzHuType::Wz_Bd_Hu:
				{
					winType.append(" ∞À∂‘");
				}
				break;
			 case WzHuType::Wz_Scs_Hu:
				{
					winType.append(" »˝≤∆…Ò");
					label_cardType->setString(GBKToUtf8("»Ì≈∆ »˝≤∆…Ò"));
				}
				break;
			 case WzHuType::Wz_Tian_Hu:
				{
					winType.append(" ÃÏ∫˙");
				}
				break;
			 case WzHuType::Wz_Di_Hu:
				{
					winType.append(" µÿ∫˙");
				}
				break;
			case WzHuType::Wz_Dd_Hu:
				{
					winType.append(" µ•µı∫˙");
				}
				break;
			case WzHuType::Wz_Pp_Hu:
				{
					winType.append(" ≈ˆ≈ˆ∫˙");
				}
				break;
			case WzHuType::Wz_Qys:
				{
					winType.append(" «Â“ª…´");
				}
				break;
			case WzHuType::Wz_Hys:
				{
					winType.append(" ªÏ“ª…´");
				}
				break;
			default:
				break;
			}
			label_cardType->setString(GBKToUtf8(winType.c_str()));
			if (iBanker)
			{
				if (iDiPoint==tagMaiDing::MdInfo::MD_ZHUANG_MAI)
				{
					pMaiDi->setString(GBKToUtf8("¬Úµ◊"));
				}
				else
				{
					pMaiDi->setString("");
				}
				pDingDi->setString("");
			}
			else
			{
				if (iDiPoint==tagMaiDing::MdInfo::MD_XIAN_DING)
				{
					pDingDi->setString(GBKToUtf8("∂•µ◊"));
				}
				else
				{
					pDingDi->setString("");
				}
                pMaiDi->setString("");
			}
			sprintf(strName,"%lld",iTotalPoint);
			auto pTotal = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_total"));
			pTotal->setString(strName);
		}
		else
		{
			auto pUser = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_resultWidget,"player2"));
			auto pName = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_nick"));
			pName->setString(GBKToUtf8(name));
			sprintf(strName,"%lld",iDiPoint);
			auto pMaiDi = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_maidi"));
			auto pDingDi = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_dingdi"));
			if (iBanker)
			{
				if (iDiPoint==tagMaiDing::MdInfo::MD_ZHUANG_MAI)
				{
					pMaiDi->setString(GBKToUtf8("¬Úµ◊"));
				}
				else
				{
					pMaiDi->setString("");
				}
				pDingDi->setString("");
			}
			else
			{
				if (iDiPoint==tagMaiDing::MdInfo::MD_XIAN_DING)
				{
					pDingDi->setString(GBKToUtf8("∂•µ◊"));
				}
				else
				{
					pDingDi->setString("");
				}
                pMaiDi->setString("");
			}
			sprintf(strName,"%lld",iTotalPoint);
			auto pTotal = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pUser,"Label_total"));
			pTotal->setString(strName);
		}
	}
	*/

	void GameTableUI::showThinkAct(int type)
	{
        /*
		if(type==0)
		{
             _tableLogic->sendUserUp();
		     _tableLogic->sendForceQuit();
		}*/
	}

	void GameTableUI::showActionResult(USHORT type,BYTE count,BYTE cardValues,bool iVisible)
	{
		if (!iVisible)
		{
			for (int i = 0; i < 5; i++)
			{
				char str[60];
				sprintf(str,"Panel_action%d",i+1);
				auto panel_action=dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget,str));
				panel_action->setVisible(iVisible);
			}
            return;
		}
        
        /*
		char name[60];
		sprintf(name,"Panel_action%d",count);
		auto panel_action=dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget,name));
		panel_action->setVisible(iVisible);
		
		ImageView* image_card[4];
		char cardName[60];
		for (int i = 0; i < 4; i++)
		{
			sprintf(cardName,"Image_card%d",i+1);
			image_card[i]=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(panel_action,cardName));
		}
		BYTE cardType=_gameLogic.GetKind(cardValues);
		BYTE cardCount=_gameLogic.GetDian(cardValues);
		switch (type)
		{
		case MjActFlag::usChi:
			{
				switch (_index)
				{
				case 1:
					{
						for (int i = 0; i < 3; i++)
						{
							char strName[60];
							switch (cardType)
							{
							case MjKind::Wan:
								sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
								break;
							case MjKind::Tiao:
								sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
								break;
							case MjKind::Bing:
								sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
								break;
							case MjKind::Feng:
								sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
								break;
							case MjKind::Hua:
								break;
							default:
								break;
							}
							image_card[i]->setVisible(true);
							image_card[i]->loadTexture(strName);
						}
					}
					break;
				case 2:
					{
						cardCount=cardCount-1;
						for (int i = 0; i < 3; i++)
						{
							char strName[60];
							switch (cardType)
							{
							case MjKind::Wan:
								sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
								break;
							case MjKind::Tiao:
								sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
								break;
							case MjKind::Bing:
								sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
								break;
							case MjKind::Feng:
								sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
								break;
							case MjKind::Hua:
								break;
							default:
								break;
							}
							image_card[i]->setVisible(true);
							image_card[i]->loadTexture(strName);
						}
					}
					break;
				case 3:
					{
						cardCount=cardCount-2;
						for (int i = 0; i < 3; i++)
						{
							char strName[60];
							switch (cardType)
							{
							case MjKind::Wan:
								sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
								break;
							case MjKind::Tiao:
								sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
								break;
							case MjKind::Bing:
								sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
								break;
							case MjKind::Feng:
								sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
								break;
							case MjKind::Hua:
								break;
							default:
								break;
							}
							image_card[i]->setVisible(true);
							image_card[i]->loadTexture(strName);
						}
					}
					break;
				default:
					break;
				}
				image_card[3]->setVisible(false);
			}
			break;
		case MjActFlag::usPeng:
			{
				for (int i = 0; i < 3; i++)
				{
					char strName[60];
					switch (cardType)
					{
					case MjKind::Wan:
						sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount);
						break;
					case MjKind::Tiao:
						sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount);
						break;
					case MjKind::Bing:
						sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount);
						break;
					case MjKind::Feng:
						sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount);
						break;
					case MjKind::Hua:
						break;
					default:
						break;
					}
					image_card[i]->setVisible(true);
					image_card[i]->loadTexture(strName);
				}
				image_card[3]->setVisible(false);
			}
			break;
		case MjActFlag::usGang:
			{
				for (int i = 0; i < 4; i++)
				{
					char strName[60];
					switch (cardType)
					{
					case MjKind::Wan:
						sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount);
						break;
					case MjKind::Tiao:
						sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount);
						break;
					case MjKind::Bing:
						sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount);
						break;
					case MjKind::Feng:
						sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount);
						break;
					case MjKind::Hua:
						break;
					default:
						break;
					}
					image_card[i]->setVisible(true);
					image_card[i]->loadTexture(strName);
				}
			}
			break;
		default:
			break;
		}
         */
	}

    
#pragma  mark 玩家出牌
	void GameTableUI::showOutCard(BYTE seatNo,BYTE cardValue)
	{
        if(_lastOutCartSeatNo != -1)
            _cardOutList[_lastOutCartSeatNo]->removeLastCardZhiShiSp();

        _lastOutCartSeatNo = seatNo;
        
        if (seatNo != 0 ) {
            
            YZAudioEngine::getInstance()->playEffect(GAME_SOUND_OUT_CARD);
            
            char strName[40];
            if (_players[seatNo]->getIsBoy()) {
                sprintf(strName, "man_%d.mp3", cardValue);
            }else{
                sprintf(strName, "woman_%d.mp3", cardValue);
            }
            
            YZAudioEngine::getInstance()->playEffect(FormatSoundFile(strName));
            
             _cardListBoard[seatNo]->playerOutCard(cardValue);
        }
        
        /*
        char strName[40];
        if (_players[seatNo]->getIsBoy()) {
            sprintf(strName, "erMaJiang/music/playmahs/man_%d.mp3", cardValue);
        }else{
            sprintf(strName, "erMaJiang/music/playmahs/woman_%d.mp3", cardValue);
        }
        
        YZAudioEngine::getInstance()->playEffect(strName);
        
        if(_lastOutCartSeatNo != -1)
            _cardOutList[_lastOutCartSeatNo]->removeLastCardZhiShiSp();
        
        _lastOutCartSeatNo = seatNo;
        
        if (seatNo == 0) {
            _cardListBoard[seatNo]->outCard(cardValue);
        }else{
            _cardListBoard[seatNo]->playerOutCard(cardValue);
        }*/
    }
    
    //出牌错误， 重新让玩家出牌
    void GameTableUI::outCardResultError(BYTE seatNo){
        _cardListBoard[seatNo]->setCanOutCard(true);
        _cardListBoard[seatNo]->resetAllCardPoint();
    }

    void GameTableUI::showGameEnd(){
        this->showResultLayout(0);
    }
    
#pragma 创建玩家出牌列表
    	void GameTableUI::showUserHandCardOut(BYTE seatNo)
	{
		if(isValidSeat(seatNo))
		{
			if(_cardListBoard[seatNo] != nullptr)
			{
				//_cardListBoard[seatNo]->removeCardAllOnce(cards);
			}

			if(_cardOutList[seatNo] == nullptr)
			{
				addOutCardList(seatNo);	
			}
		}
	}

	void GameTableUI::callEverySecond(float delta)
	{
        do
        {
            if(_timerText == nullptr)
            {
                stopTimer();
                break;
            }
            
            int count = _timerText->getTag();
            if(count <= 0)
            {
                stopTimer();
                break;
            }
            
            count--;
            _timerText->setTag(count);
            char str[20] = {0};
            sprintf(str, "%d", count);
            _timerText->setString(str);
            
            if (_cardListBoard[0]!=NULL&&_cardListBoard[0]->getCanOutCard())
            {
                //_cardListBoard[0]->setCanOutCard(false);
                //_tableLogic->sendOutCard(_cardListBoard[0]->getCanOutCardValue());
            }
            
            if (count==0)
            {
                //_cardListBoard[_nowPlaySeat]->playRandomCard();
                /*
                if (_tableLogic->_waiting)
                {
                    _tableLogic->sendUserUp();
                    _tableLogic->sendForceQuit();
                }*/
                
                if (_currentStatues==1)
                {
                    _tableLogic->GameOutCardOverTime();
                }
                else if(_currentStatues==2)
                {
                    //_tableLogic->sendActRequest(MjActFlag::usGuo,0);
                }
                
            }
        } while (0);
    }

	void GameTableUI::startTimer()
	{
		schedule(schedule_selector(GameTableUI::callEverySecond),1.0f);
	}

	void GameTableUI::stopTimer()
	{
		unschedule(schedule_selector(GameTableUI::callEverySecond));
	}

	void GameTableUI::initParams()
	{
		std::memset(_btnAction, 0, sizeof(_btnAction));
		std::memset(_btnType, 0, sizeof(_btnType));

	    std::memset(_players, 0, sizeof(_players));
	    std::memset(_cardListBoard, 0 , sizeof(_cardListBoard));
	    std::memset(_cardOutList, 0, sizeof(_cardOutList));
		std::memset(outCard, 0, sizeof(outCard));
		_currentStatues=0;
        
        waitingForAnGuang = false;
        
      
	}

	void GameTableUI::clearDesk()
	{
		for(int i = 0; i < PLAYER_COUNT; i++)
		{

			if(_cardListBoard[i] != nullptr)
			{
				_cardListBoard[i]->clear();
			}
		
			if(_cardOutList[i] != nullptr)
			{
                _cardOutList[i]->clear();
			}
            
            showUserReady(i,false,false);
            
            _players[i]->setBanker(false);
		}
        
       
        bDarkGuangSeatNo = -1;
        bDarkGuangCard = 0x00;
        _bankerFirstAction = 0;
        _bankerFirstActionCard = 0;
        _lastCardCount = 112;
        this->changeLastCardCount(_lastCardCount);
        //_btnAuto->setVisible(false);
		_cancleAutobg->setVisible(false);
        this->showChatLayout(false);
        this->showSettingLayout(false);
        this->hideAllAcitonBtn();
        this->showMahjongLayout(NULL , false);
        std::vector<BYTE> cards;
        this->showListenerCard(false, cards);
        _btnReady ->setVisible(false);
        //outIndexSp->setVisible(false);
	}

	void GameTableUI::leaveDesk()
	{
        this->clearDesk();
        
		if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
		{
			RoomLogic()->closeRoom();
            
            if (m_iNowGameCount == 0) {
                HallLayer::createScene();
            }
            
			//HallLayer::createScene();
		}
		else
		{
            if (m_iNowGameCount == 0) {
                GameDesk::createDesk(RoomLogic()->getSelectedRoom(), true);
            }
			//GameDesk::createDesk(RoomLogic()->getSelectedRoom(), true);
            //Director::getInstance()->replaceScene(Welcome::createScene());
        }
	}
    
    void GameTableUI::updateChatBarProgress(float time){
        chatProgress->setPercentage(0);
        chatProgress->setVisible(true);
        _btnChat->loadTextureNormal("erMaJiang/buttons/info.png");
        ProgressTo *progressToAction =  ProgressTo::create(time, 100.0f);
        chatProgress->runAction(Sequence::createWithTwoActions(progressToAction, CallFunc::create([=]{
            canChat = true;
            chatProgress->setVisible(false);
            _btnChat->loadTextureNormal("erMaJiang/buttons/info2.png");
        })));

    }


	void GameTableUI::loadLayout()
	{
	    Size winSize = Director::getInstance()->getVisibleSize();
		_tableWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/CYMahjong.json");
	    _tableWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
	    _tableWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        _tableWidget->setScale(winSize.width / _tableWidget->getContentSize().width , winSize.height / _tableWidget->getContentSize().height);
	    addChild(_tableWidget, 2);
        
        auto bg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "Image_BackGround"));
        //bg->setScale(winSize.width / bg->getContentSize().width , winSize.height / bg->getContentSize().height);
        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        bg->setVisible(false);
        
        auto background = Sprite::create("erMaJiang/bg.png");
        background->setScale(winSize.width / bg->getContentSize().width , winSize.height / bg->getContentSize().height);
        background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        this->addChild(background);
        
        
		//auto btnExit = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_wanFa"));
		//btnExit->setTag(ERMAJANG_TAG_BTNWANFA);
		//btnExit->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));

        //_leftPanel = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "SettingPanel"));
        //_leftPanel->setVisible(true);
        //_leftPanel->setLocalZOrder(99);
        
        //_btnRight = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"SettingBtn"));
        //_btnRight->setTag(ERMAJANG_TAG_BTNRIGHT);
        //_btnRight->setVisible(true);
        //_btnRight->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
		_btnReady = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_start"));
		_btnReady->setTag(ERMAJANG_TAG_BTNREADY);
		_btnReady->setPosition(Vec2(winSize.width/2 , _btnReady->getContentSize().height));
		_btnReady->setVisible(true);
		_btnReady->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        _btnReady->loadTextures(IMAGE_BTN_READLY, IMAGE_BTN_READLY);
        
        _btnSet= dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_set"));
        _btnSet->setTag(ERMAJANG_TAG_BTNSET);
        _btnSet->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        _btnChat= dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_chat"));
        _btnChat->setTag(ERMAJANG_TAG_BTNCHAT);
        _btnChat->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        _btnChat->loadTextureNormal("erMaJiang/buttons/info2.png");
        
        Sprite *activeSprite = Sprite::create("erMaJiang/buttons/info2.png");
        chatProgress = ProgressTimer::create(activeSprite);
        chatProgress->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        chatProgress->setType(ProgressTimer::Type::RADIAL);
        chatProgress->setPosition(_btnChat->getPosition());
        _tableWidget->addChild(chatProgress, 5);
        chatProgress->setVisible(false);

        _btnVoice= dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_sound"));
        _btnVoice->setVisible(true);
        _btnVoice->setTag(ERMAJANG_TAG_BTNVoice);
        _btnVoice->addTouchEventListener(CC_CALLBACK_2(GameTableUI::touchVoiceEvent, this));

		_cancleAutobg=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,"cancleAuto_bg"));
		_cancleAutobg->setVisible(false);
        
        auto btnCancleAuto=dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,"btn_cancleAuto"));
		btnCancleAuto->setTag(ERMAJANG_TAG_BTNCAUTO);
		btnCancleAuto->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));

		char strName[60];
		for (int i = 0; i < PLAYER_COUNT; i++)
		{
			sprintf(strName, "Image_ready%d", i + 1);
			_imgReady[i] = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,strName));
		    _imgReady[i]->setVisible(false);
		}

		for (int i = 0; i < 6; i++)
		{
			sprintf(strName, "btn_type%d", i + 1);
			_btnType[i]=dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,strName));
			_btnType[i]->setTag(ERMAJANG_TAG_BTNPASS+i);
            _btnType[i]->setLocalZOrder(5);
            _btnType[i]->setVisible(false);
			_btnType[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
		}

		_PanelChi=dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget,"Panel_Chow"));
        _PanelChi->setLocalZOrder(10);
		_PanelChi->setVisible(false);
		for (int i = 0; i < 3; i++)
		{
			sprintf(strName, "btn_chowType%d", i + 1);
			_btnChiType[i]=dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget,strName));
			_btnChiType[i]->setTag(ERMAJANG_TAG_BTNCHIT+i);
			_btnChiType[i]->setVisible(false);
            _btnChiType[i]->setLocalZOrder(5);
            _btnChiType[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
		}
		for (int i = 0; i < 3; i++)
		{
			sprintf(strName, "Image_chiType%d", i + 1);
			_imgChiType[i]=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,strName));
            _imgChiType[i]->setLocalZOrder(15);
            _imgChiType[i]->setVisible(false);
		}

		/*
		for (int i = 0; i < 5; i++)
		{
			sprintf(strName, "Panel_action%d", i + 1);
			_panelAction[i]=dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_tableWidget,strName));
			_panelAction[i]->setVisible(false);
		}*/

        
        Point playerPoint[4];
        playerPoint[0] = Point(0 , winSize.height * .2);
        playerPoint[1] = Point(winSize.width - 100 , _tableWidget->getContentSize().height * .61);
        playerPoint[2] = Point(winSize.width * .78f , _tableWidget->getContentSize().height * .83);
        playerPoint[3] = Point(0 , winSize.height * .65);
		
		char playerName[64] = {0};
		for(int i = 0; i < PLAYER_COUNT; i++)
		{
			sprintf(playerName, "Image_player%d", i + 1);
			auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,playerName));
            ptr->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            ptr->setPosition(playerPoint[i]);
			if(ptr != nullptr)
			{
				PlayerUI* player = PlayerUI::create(INVALID_USER_ID , 0);
				player->setAnchorPoint(ptr->getAnchorPoint());
				player->setPosition(ptr->getPosition());
				player->setTableUI(this);
				_players[i] = player;
				_players[i]->setVisible(false);
				ptr->getParent()->addChild(player, 20);
			}
		}
        
        label_roomNum = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_tableWidget,"label_roomNum"));
        label_roomNum->setString("房号 : 000000");
        label_niaoNum = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_tableWidget,"label_niaoCount"));
        label_GameTimes = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_tableWidget,"label_gamecount"));
        label_GameTimes->setFontSize(22);
        char num[60]={0};
        sprintf(num, "%d/%d局",0,0);
        label_GameTimes->setString(num);
        
        char zmName[5];
        sprintf(zmName, "%d", 0);
        label_niaoNum->setString(zmName);
        
        label_time = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_tableWidget,"label_time"));
        label_time->setString(DateUtils::getNowTime());
        
        _imgNetState =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,"img_netState"));
        _batteryLevel =dynamic_cast<ui::LoadingBar*>(ui::Helper::seekWidgetByName(_tableWidget,"dlProgressBar"));
        
        _roomInfoCardCountTxt = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(_tableWidget,"label_cardCount"));
        _roomInfoCardCountTxt->setVisible(true);
        this->changeLastCardCount(_lastCardCount);
        
        _settingWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/GameSet.json");
        _settingWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
        _settingWidget->setTag(ERMAJANG_TAG_LAYOUTRESULT);
        _settingWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        _settingWidget->setVisible(false);
        addChild(_settingWidget, 99);
        
        auto setBtnDiaband  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_settingWidget, "btn_disband"));
        setBtnDiaband->setTag(Tag_Disband);
        setBtnDiaband->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback,this));
        
        _btnDiaband  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableWidget, "btn_disband"));
        _btnDiaband->setTag(Tag_Disband);
        _btnDiaband->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback,this));
        
        auto btnPTLanguage  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_settingWidget, "btn_ptLanguage"));
        auto btnWHLanguage  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_settingWidget, "btnwhLanguage"));
        btnWHLanguage->setVisible(false);
        btnPTLanguage->setVisible(false);

        if(TAG_USE_TWO_LANGUAGE){
            btnWHLanguage->setVisible(true);
            btnPTLanguage->setVisible(true);

            btnPTLanguage->setTag(TAG_BTN_PTLANGUAGE);
            btnPTLanguage->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnSelcetLanguageEventCallback,this));

            btnWHLanguage->setTag(TAG_BTN_WHLANGUAGE);
            btnWHLanguage->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnSelcetLanguageEventCallback,this));
        }else{
            YZAudioEngine::getInstance()->setSwitchSoundLanguage(1);
        }


        if (YZAudioEngine::getInstance()->getSoundLanguage() == 1) {
            btnPTLanguage->loadTextureNormal(IMAGE_BTN_PTLANGUAGE_S);
            btnWHLanguage->loadTextureNormal(IMAGE_BTN_WHLANGUAGE_N);
        }else{
            btnPTLanguage->loadTextureNormal(IMAGE_BTN_PTLANGUAGE_N);
            btnWHLanguage->loadTextureNormal(IMAGE_BTN_WHLANGUAGE_S);
        }
        
        ui::Slider * bgSlider =dynamic_cast<ui::Slider*>(ui::Helper::seekWidgetByName(_settingWidget,"slider_music"));
        ui::Slider * soundSlider =dynamic_cast<ui::Slider*>(ui::Helper::seekWidgetByName(_settingWidget,"slider_sound"));
        
        bgSlider->setPercent(YZAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);
        soundSlider->setPercent(YZAudioEngine::getInstance()->getEffectsVolume() * 100);
   
        bgSlider->addEventListenerSlider(this, sliderpercentchangedselector(GameTableUI::bgSliderEvent));
        soundSlider->addEventListenerSlider(this, sliderpercentchangedselector(GameTableUI::soundSliderEvent));
        
        _chatWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/GameChat.json");
        _chatWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
        _chatWidget->setTag(ERMAJANG_TAG_LAYOUTRESULT);
        _chatWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        _chatWidget->setVisible(false);
        addChild(_chatWidget, 99);
        
        auto btnChatType1 = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_chatWidget,"btn_type1"));
        btnChatType1->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnChatClickEventCallback, this));
        
        auto btnChatType2 = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_chatWidget,"btn_type2"));
        btnChatType2->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnChatClickEventCallback, this));
        
        auto btnChatType3 = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_chatWidget,"btn_type3"));
        btnChatType3->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnChatClickEventCallback, this));

        auto Panel1 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type1"));
        auto Panel2 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type2"));
        Panel1->setVisible(true);
        Panel2->setVisible(false);
        
        auto listView = dynamic_cast<ui::ListView*>(ui::Helper::seekWidgetByName(_chatWidget,"Chat1ListView"));
        for (int i = 0; i < listView->getChildrenCount(); i++) {
            ui::Widget* label = (ui::Widget*)listView->getChildren().at(i);
            label->setTag(2000 + i + 1);
            label->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnChatItemClickEventCallback, this));
            chatWidget.insert(make_pair(i + 1, label));
        }
        
        auto imageView = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_chatWidget,"Chat2List"));
        for (int i = 0; i < imageView->getChildrenCount(); i++) {
            ui::Button* btn = (ui::Button*)imageView->getChildren().at(i);
            btn->setTag(3000 + i + 1);
            btn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnChatItemClickEventCallback, this));
        }
        
        auto Panel3 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type3"));
        Panel3->setVisible(false);

        _chatListView = dynamic_cast<ui::ListView*>(Panel3->getChildByName("OldChatListView"));
        
        Layout* pModelItem = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/ChatRecord.json"));
        _chatListView->setItemModel(pModelItem);
        _chatListView->removeAllItems();
        _chatListView->refreshView();
        chatMsgs.clear();
        
        auto _btnCloseChat = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_chatWidget,"btn_close"));
        _btnCloseChat->setTag(ERMAJANG_TAG_BTNCCHAT);
        _btnCloseChat->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        auto _btnChatSend = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_chatWidget,"btn_send"));
        _btnChatSend->setTag(ERMAJANG_TAG_BTNCHATSEND);
        _btnChatSend->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        auto _btnCloseSetting = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_settingWidget,"btn_close"));
        _btnCloseSetting->setTag(ERMAJANG_TAG_BTNCSET);
        _btnCloseSetting->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        //初始化胜利界面
        _resultWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/MaJiangResult.json");
        _resultWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
        _resultWidget->setTag(ERMAJANG_TAG_LAYOUTRESULT);
        _resultWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        _resultWidget->setVisible(false);
        addChild(_resultWidget, 90);

        auto buttonShare = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_resultWidget, "btn_share"));
        buttonShare->setTag(ERMAJANG_TAG_BTNRESULT_CHANGE);
        buttonShare->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        auto btnContinue = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_resultWidget, "btn_continue"));
        btnContinue->setTag(ERMAJANG_TAG_BTNRESULT_CONTINUE);
        btnContinue->addTouchEventListener(CC_CALLBACK_2(GameTableUI::btnClickEventCallback, this));
        
        disbandRoomLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("exitLayer/PromptLayer.json"));
        disbandRoomLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
        disbandRoomLayout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        disbandRoomLayout->setVisible(false);
        addChild(disbandRoomLayout , 99);
        
        listenerCardLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/ListenerCardLayer.json"));
        listenerCardLayout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        listenerCardLayout->setPosition(Vec2::ZERO);
        listenerCardLayout->setVisible(false);
        listenerCardLayout->setSwallowTouches(true);
        listenerCardLayout->setTouchEnabled(false);
        addChild(listenerCardLayout , 70);
        
        auto listenerCard = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(listenerCardLayout,"card"));
        listenerCard->setVisible(false);
        _listenerCardVec.push_back(listenerCard);
        Size listenerCardSize = listenerCard->getContentSize() * listenerCard->getScale();
        for (int i = 1; i < 28; i++) {
            ImageView * cardImg =  ImageView::create("erMaJiang/positonMark/w_1.png");
            cardImg->setScale(listenerCard->getScale());
            cardImg->setPosition(listenerCard->getPosition() + Vec2(listenerCardSize.width * i, 0));
            cardImg->setVisible(false);
            cardImg->setSwallowTouches(true);
            cardImg->setTouchEnabled(false);
            listenerCardLayout->addChild(cardImg);
            _listenerCardVec.push_back(cardImg);
        }
        
        voiceLayout = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "voiceBG"));
        voiceLayout->setVisible(false);
        char voiceName[60];
        for (int i = 1; i <= 4; i++) {
            sprintf(voiceName, "voice%d", i);
            auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,voiceName));
            _voiceProgressVec.push_back(ptr);
        }
        
        auto panel_gameTime = dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget, "Panel_clock"));
        panel_gameTime->setVisible(false);
        panel_gameTime->setScale(.65f);
        //panel_gameTime->setPosition(Vec2(winSize.width/2, winSize.height * .8f));
        
        gameMsgBar = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "msgBar"));
        gameMsgBar->setVisible(false);
        
        label_msg = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(gameMsgBar, "label_msg"));
        label_msg->setString("欢迎来到欢腾休闲手游，招募一级二级代理，微信号：huanteng01 ");
        //label_msg->setString("欢迎来到欢腾红中麻将,如果您有任何问题,可以联系我们的微信客服公众号：[欢腾休闲手游]。祝大家游戏愉快！");
        
        this->showGameMsgBar(true);
        
        this->hideZmLayers();
        
        voiceTime = 0;
        
        gameRecordLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("erMaJiang/GameRecord.json");
        gameRecordLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        gameRecordLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        gameRecordLayer->setVisible(false);
        addChild(gameRecordLayer, 99);
        
        auto ewmImg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(gameRecordLayer, "Image_58"));
        ewmImg->setVisible(false);

        auto buttonRecordShare = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(gameRecordLayer, "btn_share"));
        buttonRecordShare->addTouchEventListener(CC_CALLBACK_2(GameTableUI::onShareCallBack,this));
        
        auto buttonExit = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(gameRecordLayer, "btn_back"));
        buttonExit->addClickEventListener([=](Ref*) {
            this->clearDesk();
            _resultWidget->setVisible(false);
            
            
            if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
            {
                HallLayer::createScene();
            }
            else
            {
                GameDesk::createDesk(RoomLogic()->getSelectedRoom(), true);
                //Director::getInstance()->replaceScene(Welcome::createScene());
            }
            
            
            //_tableLogic->sendUserUp();
            //gameRecordLayer->setVisible(false);
           // _tableLogic->sendForceQuit();
        });
        
        char recordName[50] = {0};
        for (int i = 0; i < 4; i++) {
            
            Map<std::string , Text *> map ;
            
            sprintf(recordName, "player%d", i + 1);

            auto player =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(gameRecordLayer, recordName));
            
            map.insert("playerName", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_playerName")));
            map.insert("huTimes", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_huTimes")));
            map.insert("gangTimes", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_gangTimes")));
            map.insert("darkGangTimes", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_darkGangTimes")));
            map.insert("zmTimes", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_onZMTimes")));
            map.insert("socre", dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player, "label_score")));
            
            gameRecordTextMap.push_back(map);
            
            auto ownerImg =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player, "img_roomOwner"));
            ownerImg->setVisible(true);
            gameRecordOwner.push_back(ownerImg);

            auto headImg =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player, "headImg"));
            headImg->setVisible(true);
            gameRecordHeadImg.push_back(headImg);
            
            auto winImg =dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player, "winerImg"));
            winImg->setVisible(true);
            gameRecordWiner.push_back(winImg);
            
        }
        
        this->schedule(schedule_selector(GameTableUI::updateTime), 1.0f);
        
        
        socketErrorLayer = SocketErrorLayer::createLayer();
        addChild(socketErrorLayer, 1000);
        socketErrorLayer->setVisible(false);
        /*
        char zmLayerName[60];
        sprintf(zmLayerName, "zm%dBG", 3);
        auto zmLayer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, zmLayerName));
        zmLayer->setVisible(true);
        
        std::vector<ImageView *> showCards;
        showCards.clear();
        char zmImageName[60];
        for (int i = 0; i< 3; i++) {
            sprintf(zmImageName, "zm_img%d", i + 1);
            auto zmImg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(zmLayer, zmImageName));
            zmImg->setVisible(false);
            showCards.push_back(zmImg);
        }
        
        nowShowZmIndex = -1;
        
        vector<BYTE> cbZaCardData;
        cbZaCardData.push_back(0x01);
        cbZaCardData.push_back(0x02);
        cbZaCardData.push_back(0x03);
        
        this->showNextZmImage(cbZaCardData, showCards);
        */
    }

    void GameTableUI::showGameAllRecord(GameRecordData data,bool isShow){
        gameRecordLayer->setVisible(isShow);
        _tableLogic->sendUserUp();
        if (isShow) {
            Text * roomNum = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(gameRecordLayer, "label_roomNum"));
            //roomNum->setString(data.roomPwd);
            char roomName[20] = {0};
            sprintf(roomName, "房号 : %s", data.roomPwd.c_str());
            roomNum->setString(roomName);
            
            Text * zmCount = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(gameRecordLayer, "label_zmCount"));
            zmCount->setString(__String::createWithFormat("翻码数量 : %d " , data.m_cbZaCodeNum)->getCString());
            
            char timeName[30] = {0};
            sprintf(timeName, "结束时间 : %s", DateUtils::getNowTime().c_str());
            Text * time = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(gameRecordLayer, "label_time"));
            time->setString(timeName);
            
            for (int i = 0 ; i < 4; i++) {
                
                UserInfoStruct* userInfo = _tableLogic->getGameUserByDeskStation(i);
                std::string headUrl = "";
                std::string nickName ="";
                for (int j = 0; j< 61; j++) {
                    nickName += data.szNickName[i][j];
                }
                
                bool bBoy = true;
                bool isDeskOwner = data.dwUserID[i] == data.m_dwRoomOwner;
                if (userInfo != NULL) {
                    headUrl = userInfo->szHeadUrl;
                    nickName = userInfo->nickName;
                    bBoy = userInfo->bBoy;
                }else{
                    jzError("游戏结束 获取本地缓存的第%d个玩家信息为空 " , i);
                }
                
                JzDebug("游戏结束 头像链接为 : %s " , headUrl.c_str());
                
                auto httpSp = HttpSprite::create(headUrl, "roleInfo/roleInfo/headImg.png");
                httpSp->setImgSize( Size(88, 88));
                httpSp->setPosition(gameRecordHeadImg.at(i)->getPosition());
                gameRecordHeadImg.at(i)->getParent()->addChild(httpSp , 1);
                gameRecordOwner.at(i)->setLocalZOrder(3);
                if (isDeskOwner) {
                    gameRecordOwner.at(i)->setVisible(true);
                }else{
                    gameRecordOwner.at(i)->setVisible(false);
                }
                if (i == data.winerSeatNo) {
                    gameRecordWiner.at(i)->setVisible(true);
                }else{
                    gameRecordWiner.at(i)->setVisible(false);
                }
                
                gameRecordTextMap[i].at("playerName")->setString(GBKToUtf8(nickName.c_str()));
                gameRecordTextMap[i].at("huTimes")->setString(__String::createWithFormat("%d",data.huTimes[i])->getCString());
                gameRecordTextMap[i].at("gangTimes")->setString(__String::createWithFormat("%d",data.gangTimes[i])->getCString());
                gameRecordTextMap[i].at("darkGangTimes")->setString(__String::createWithFormat("%d",data.drakGangTimes[i])->getCString());
                gameRecordTextMap[i].at("zmTimes")->setString(__String::createWithFormat("%d",data.zmTimes[i])->getCString());
                gameRecordTextMap[i].at("socre")->setString(__String::createWithFormat("%d",data.score[i])->getCString());
            }
            
        }
    }
    
    
    void GameTableUI::onShareCallBack(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype){
        if (Widget::TouchEventType::ENDED != touchtype)	return;
        JzDebug("onShareCallBack...");
        auto shareLayer = ShareLayer::createLayer(ShareType::SCREEN);
        this->addChild(shareLayer, 999);
    }
    
    void GameTableUI::hideZmLayers(){
        auto zm1Layer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "zm1BG"));
        zm1Layer->setVisible(false);

        auto zm2Layer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "zm2BG"));
        zm2Layer->setVisible(false);
        
        auto zm4Layer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "zm4BG"));
        zm4Layer->setVisible(false);
        
        auto zm3Layer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "zm3BG"));
        zm3Layer->setVisible(false);
        
        auto zm6Layer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "zm6BG"));
        zm6Layer->setVisible(false);
    }
    
	bool GameTableUI::isValidSeat(BYTE seatNo)
	{
		return (seatNo < PLAYER_COUNT && seatNo >= 0);
	}

	void GameTableUI::showCardType(int type,BYTE cardValues)
	{
        /*
		ImageView* Image_chow[3];
		char name[60];
		BYTE cardType=_gameLogic.GetKind(cardValues);
		BYTE cardCount=_gameLogic.GetDian(cardValues);
		switch (type)
		{
		case 1:
			{
				for (int i = 0; i < 3; i++)
				{
					sprintf(name,"Image_chow%d",i+1);
					Image_chow[i]=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[0],name));
					char strName[60];
					switch (cardType)
					{
					case MjKind::Wan:
						sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
						break;
					case MjKind::Tiao:
						sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
						break;
					case MjKind::Bing:
						sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
						break;
					case MjKind::Feng:
						sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
						break;
					case MjKind::Hua:
						break;
					default:
						break;
					}
					Image_chow[i]->loadTexture(strName);
				}
			}
			break;
		case 2:
			{
				cardCount=cardCount-1;
				for (int i = 0; i < 3; i++)
				{
					sprintf(name,"Image_chow%d",i+1);
					Image_chow[i]=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[1],name));
					char strName[60];
					switch (cardType)
					{
					case MjKind::Wan:
						sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
						break;
					case MjKind::Tiao:
						sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
						break;
					case MjKind::Bing:
						sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
						break;
					case MjKind::Feng:
						sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
						break;
					case MjKind::Hua:
						break;
					default:
						break;
					}
					Image_chow[i]->loadTexture(strName);
				}
			}
			break;
		case 3:
			{
				cardCount=cardCount-2;
				for (int i = 0; i < 3; i++)
				{
					sprintf(name,"Image_chow%d",i+1);
					Image_chow[i]=dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_imgChiType[2],name));
					char strName[60];
					switch (cardType)
					{
					case MjKind::Wan:
						sprintf(strName,"erMaJiang/winCard/w_%d.png",cardCount+i);
						break;
					case MjKind::Tiao:
						sprintf(strName,"erMaJiang/winCard/tiao_%d.png",cardCount+i);
						break;
					case MjKind::Bing:
						sprintf(strName,"erMaJiang/winCard/tong_%d.png",cardCount+i);
						break;
					case MjKind::Feng:
						sprintf(strName,"erMaJiang/winCard/feng_%d.png",cardCount+i);
						break;
					case MjKind::Hua:
						break;
					default:
						break;
					}
					Image_chow[i]->loadTexture(strName);
				}
			}
			break;
		default:
			break;
		}
		*/
		
	}
    
    
    void GameTableUI::showGamePanelBtns(bool isVisable){
        
        if (isVisable == _leftShow) {
            return;
        }
        
        if (isVisable) {
            _btnRight->loadTextures("erMaJiang/buttons/btn_setup.png", "erMaJiang/buttons/btn_setup.png");
            _leftShow = true;
            _leftPanel->runAction(MoveTo::create(0.3f, Vec2(_leftPanel->getPositionX(), _tableWidget->getContentSize().height)));
            _btnRight->setTag(ERMAJANG_TAG_BTNLEFT);
        }else{
            _leftShow = false;
            _btnRight->loadTextures("erMaJiang/buttons/btn_setdown.png", "erMaJiang/buttons/btn_setdown.png");
            _leftPanel->runAction(MoveTo::create(0.3f, Vec2(_leftPanel->getPositionX(), _tableWidget->getContentSize().height + _leftPanel->getContentSize().height - 62)));
            _btnRight->setTag(ERMAJANG_TAG_BTNRIGHT);
        }
    }
    
    void GameTableUI::showChatLayout(bool isVisable){
        _chatWidget->setVisible(isVisable);
        //_editChatMsg->removeFromParentAndCleanup(true);
    }
    
    void GameTableUI::showSettingLayout(bool isVisable){
        _settingWidget->setVisible(isVisable);
    }
    
    void GameTableUI::btnChatItemClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype){
        if (Widget::TouchEventType::ENDED != touchtype)	return;
        ui::Widget* widget = dynamic_cast<ui::Widget*>(pSender);

        int tag = widget->getTag();
        
        if (tag > 2000 && tag < 2099) {
        
            int index = tag - 2000;
            
            //ui::Text* txt = dynamic_cast<ui::Text*>(widget->getChildByName("msg"));
            
            //CCLOG("Msg : %s" , txt->getString().c_str());
            //_tableLogic->sendChatTalkWithId(0,txt->getString());
            _tableLogic->sendChatTalkWithId(0,__String::createWithFormat("%d" , index)->getCString());
            
        }else if(tag > 3000 && tag < 3016){
            int index = tag - 3000;
            CCLOG("表情Index : %d " , index);
            _tableLogic->sendChatTalkWithId(1,__String::createWithFormat("%d" , index)->getCString());
        }

        canChat = false;

        this->updateChatBarProgress(5.0f);

        this->showChatLayout(false);
        YZAudioEngine::getInstance()->playEffect("erMaJiang/music/btnpress.mp3");
    }
    
    void GameTableUI::showChatMsg(std::string msg){

        chatMsgs.insert(chatMsgs.begin(), msg);
        
        for (int i = 0; i < chatMsgs.size() - _chatListView->getItems().size(); i++) {
            _chatListView->pushBackDefaultItem();
        }
        
        for (int i = 0; i < _chatListView->getItems().size(); i++) {
           
            Widget* pModelItem = _chatListView->getItems().at(i);
            
            auto msgText = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pModelItem,"Msg"));
            
            msgText->setString(chatMsgs.at(i));
        }
        
        /*
        Widget* pModelItem = _chatListView->getItem(_chatListView->getItems().size() - 1);

        auto msgText = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(pModelItem,"Msg"));
        
        msgText->setString(msg);
        */
        _chatListView->refreshView();
        
        /*
        Layout *custom_item = Layout::create();//要通过Layout添加到ListView中
        custom_item->setContentSize(_fontConSize);
        LabelTTF * label = LabelTTF::create(msg, "Marker Felt", 24);
        label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        label->setPosition(0,0);
        label->setFontSize(_fontSize);
        label->setFontName(_fontName);
        custom_item->addChild(label);
        
        _chatListView->addChild(custom_item);
        
        _chatListView->refreshView();
         */
    }

    
    void GameTableUI::btnChatClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype){
        if (Widget::TouchEventType::ENDED != touchtype)	return;
        Button* btn = dynamic_cast<Button*>(pSender);

        YZAudioEngine::getInstance()->playEffect("erMaJiang/music/btnpress.mp3");

        auto Panel1 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type1"));
        auto Panel2 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type2"));
        auto Panel3 = dynamic_cast<ui::Widget*>(ui::Helper::seekWidgetByName(_chatWidget,"Panel_type3"));

        
        switch (btn->getTag())
        {
                
            case 971:
                Panel1->setVisible(true);
                Panel2->setVisible(false);
                Panel3->setVisible(false);
                break;
                
            case 973:
                Panel1->setVisible(false);
                Panel2->setVisible(true);
                Panel3->setVisible(false);
                break;
                
            case 975:
                Panel1->setVisible(false);
                Panel2->setVisible(false);
                Panel3->setVisible(true);
                
                
                break;
        }
    }
    

    void GameTableUI::btnSelcetLanguageEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype){
        if (Widget::TouchEventType::ENDED != touchtype)	return;
        Button* btn = dynamic_cast<Button*>(pSender);

        switch (btn->getTag())
        {
            case TAG_BTN_PTLANGUAGE:
            {
                YZAudioEngine::getInstance()->setSwitchSoundLanguage(1);
            }
                break;
            case TAG_BTN_WHLANGUAGE:
            {
                YZAudioEngine::getInstance()->setSwitchSoundLanguage(2);
            }
                break;
        }
        
        auto btnPTLanguage  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_settingWidget, "btn_ptLanguage"));
        auto btnWHLanguage  = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_settingWidget, "btnwhLanguage"));
        if (YZAudioEngine::getInstance()->getSoundLanguage() == 1) {
            btnPTLanguage->loadTextureNormal(IMAGE_BTN_PTLANGUAGE_S);
            btnWHLanguage->loadTextureNormal(IMAGE_BTN_WHLANGUAGE_N);
        }else{
            btnPTLanguage->loadTextureNormal(IMAGE_BTN_PTLANGUAGE_N);
            btnWHLanguage->loadTextureNormal(IMAGE_BTN_WHLANGUAGE_S);
        }
    }
    
    
	void GameTableUI::btnClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* btn = dynamic_cast<Button*>(pSender);


		switch (btn->getTag())
		{
		case ERMAJANG_TAG_BTNRIGHT:
            {
                //this->showGamePanelBtns(true);
            }
			break;
                
        case ERMAJANG_TAG_BTNLEFT:
            {
                //this->showGamePanelBtns(false);
            }
                break;
                
            case ERMAJANG_TAG_BTNWANFA:
			{
                //this->showGamePanelBtns(false);
                auto layer = HelpLayer::create();
                addChild(layer, 1000);
            }
                break;
                
            case Tag_Disband:
            {
                if(m_iNowGameCount == 0){
                    if (_tableLogic->checkUserIsDeskOwner()) {
                        this->userWant2DisbandRoom(true);
                    }else{
                        if(!_gameStart){
                            _tableLogic->sendUserUp();
                        }
                    }
                }else{
                    this->userWant2DisbandRoom(true);
                   //_tableLogic->sendDisbandRoomResp(true);
                }
            }

			break;
            case Tag_Disband2:
            {
                //this->showSettingLayout(false);
                
                this->userWant2DisbandRoom(true);
            }
                
                break;
            case ERMAJANG_TAG_BTNREADY:
			{
                if (_isLastGame) {
                    //_tableLogic->sendUserUp();
                    //_tableLogic->sendUserUp();
                    //_tableLogic->sendForceQuit();
                }else{
                    
                    if (playerCount < PLAYER_COUNT) {
                        btn->setTag(ERMAJANG_TAG_BTNINVITE);
                        btn->loadTextures(IMAGE_BTN_INVITE, IMAGE_BTN_INVITE);
                        btn->setVisible(true);
                    }else{
                        btn->setVisible(false);
                    }
                    _tableLogic->sendShowStart();
                    _imgReady[0]->setVisible(true);
                }
            }
			break;
                
        case ERMAJANG_TAG_BTNINVITE :
            {
if(TAG_SHARE_IS_OPEN){
std::string zmMsg = "";
char strImagePath[128] = { 0 };
sprintf(strImagePath, "%d" , RoomLogic()->getRoomZMCount());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
JniMethodInfo minfo;
bool isHave = false;
                isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareLink2Friend", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
if (isHave)
{
jstring aRoomNum = minfo.env->NewStringUTF(RoomLogic()->getInRoomNum().c_str());
jstring zmRoomNum = minfo.env->NewStringUTF(strImagePath);
                    jstring roomCountNum = minfo.env->NewStringUTF(__String::createWithFormat("%d",m_iMaxGameCount)->getCString());
                    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , aRoomNum , zmRoomNum,roomCountNum);
                    minfo.env->DeleteLocalRef(aRoomNum);
                    minfo.env->DeleteLocalRef(zmRoomNum);
                    minfo.env->DeleteLocalRef(roomCountNum);
}else{
JzDebug("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                WxSDKManager::getInstance()->share2Friend(RoomLogic()->getInRoomNum().c_str() , strImagePath ,__String::createWithFormat("%d",m_iMaxGameCount)->getCString());
#endif
}else{
GamePromptLayer::create()->showPrompt(GBKToUtf8("开启微信分享功能即可正常使用..."));
}


}
            break;
                
		case ERMAJANG_TAG_BTNBUYDI:
			{
				//_tableLogic->sendMakeDI(true,true);
			}
			break;
		case ERMAJANG_TAG_BTNDINGDI:
		   {
				//_tableLogic->sendMakeDI(false,true);
		   }
		   break;
        case ERMAJANG_TAG_BTNCANCLE:
		   {
			   _tableLogic->sendMakeDI(false,false);
		   }
			break;
                
		 case ERMAJANG_TAG_BTNCAUTO:
		   {
               this->showGamePanelBtns(false);
               _tableLogic->sendAutoOutCard(false);
		   }
			break;
	
            case ERMAJANG_TAG_BTNSET:
            {
                this->showGamePanelBtns(false);
                
                this->showSettingLayout(true);
            }
                break;
            case ERMAJANG_TAG_BTNCHAT:
            {
                if(canChat){
                    this->showChatLayout(true);
                }else{
                    return;
                }

            }
                break;
                
            case ERMAJANG_TAG_BTNCSET:
            {
                this->showSettingLayout(false);
            }
                break;
            case ERMAJANG_TAG_BTNCCHAT:
            {
                this->showChatLayout(false);
            }
                break;
                
            case ERMAJANG_TAG_BTNCHATSEND:
            {
                auto chatInputText = dynamic_cast<ui::TextField*>(ui::Helper::seekWidgetByName(_chatWidget,"inputText"));

                CCLOG("发送消息 : %s" , chatInputText->getString().c_str());

                if (chatInputText->getString() != "") {

                canChat = false;

                this->updateChatBarProgress(5.0f);

                    _tableLogic->sendChatTalkWithId(3,chatInputText->getString());
                    this->showChatLayout(false);
                    chatInputText->setString("");
                }
            }
                break;
                
        case ERMAJANG_TAG_BTNPASS:
			{
                //过
                this->hideAllAcitonBtn();
			   _tableLogic->sendActRequest(ActionType::WIK_NULL,_bankerCard);
			}
			 break;
		case ERMAJANG_TAG_BTNCHI:
			 {
				 _PanelChi->setVisible(true);
                 CCLOG("显示吃Panel...");                
			 }
			 break;
		case ERMAJANG_TAG_BTNPENG:
			 {
                 //this->showActionCard(_bankerSeatNo, ActionType::WIK_PENG, _bankerCard);
				 _tableLogic->sendActRequest(ActionType::WIK_PENG,_bankerCard);
			 }
			 break;
		case ERMAJANG_TAG_BTNGANG:
			 {
                 //this->hideAllAcitonBtn();
                 //this->showActionCard(_bankerSeatNo, ActionType::WIK_GANG, _bankerCard);
                 _tableLogic->sendActRequest(ActionType::WIK_GANG,_bankerCard);
             }
			 break;
		case ERMAJANG_TAG_BTNWIN:
			 {
                //this->hideAllAcitonBtn();

                 CCLOG("胡牌...");
                 
                 _tableLogic->sendActRequest(ActionType::WIK_CHI_HU,_bankerCard);
             }
			 break;
		case ERMAJANG_TAG_BTNTING:
			 {
                 this->hideAllAcitonBtn();
                 //_tableLogic->sendActRequest(MjActFlag::usTing,0);
			 }
			 break;
		case ERMAJANG_TAG_BTNCHIT:
			 {
				 _index=1;
                 //this->showActionCard(_bankerSeatNo, ActionType::WIK_LEFT, _bankerCard);
				 _tableLogic->sendActRequest(ActionType::WIK_LEFT,_bankerCard);
			 }
			 break;
		case ERMAJANG_TAG_BTNCHIZ:
			 {
				 _index=2;
                 //this->showActionCard(_bankerSeatNo, ActionType::WIK_CENTER, _bankerCard);
                 _tableLogic->sendActRequest(ActionType::WIK_CENTER,_bankerCard);
             }
			 break;
		case ERMAJANG_TAG_BTNCHIW:
			 {
				 _index=3;
                 //this->showActionCard(_bankerSeatNo, ActionType::WIK_RIGHT, _bankerCard);
                 _tableLogic->sendActRequest(ActionType::WIK_RIGHT,_bankerCard);
			 }
			 break;
		case ERMAJANG_TAG_BTNRESULT_CONTINUE:
			 {
                 if (_isLastGame) {
                     
                     _tableLogic->sendGetAllGameRecord();
                     
                     //_tableLogic->sendUserUp();
                     //_tableLogic->sendUserUp();
                     //_tableLogic->sendForceQuit();
                 }else{
                     this->showResultLayout(false);
                     this->clearDesk();
                     _tableLogic->sendShowStart();
                     _imgReady[0]->setVisible(true);
                 }
			 }
			 break;
                
        case ERMAJANG_TAG_BTN_SENDMSG :
                this->showChatLayout(false);
                _tableLogic->sendChatTalkWithId(0,_editChatMsg->getText());
                break;
		case ERMAJANG_TAG_BTNRESULT_CHANGE:
			 {
                 //TO DO SHARE
				 // this->removeChildByTag(ERMAJANG_TAG_LAYOUTRESULT);
				 /*_tableLogic->sendThinkRequest(1);*/
				 // _tableLogic->sendShowStart();
				 // _imgReady[0]->setVisible(true);
                 
                 auto shareLayer = ShareLayer::createLayer(ShareType::SCREEN);
                 this->addChild(shareLayer, 999);
                 
			 }
			 break;
                
        case ERMAJANG_TAG_BTNGOON:
            
                break;
		default:
			break;
		}

        YZAudioEngine::getInstance()->playEffect("erMaJiang/music/btnpress.mp3");

	}

	void GameTableUI::addOutCardList(BYTE seatNo)
	{
        
        Size size  = Director::getInstance()->getWinSize();
        
        Vec2 boardPoint;
        if (seatNo == 0) {
            boardPoint =  _cardListBoard[seatNo]->getPosition() + Vec2(0, 90);
        }else if(seatNo == 1){
            boardPoint =  _cardListBoard[seatNo]->getPosition() - Vec2(80, 0);
        }
        else if(seatNo == 2){
            boardPoint =  _cardListBoard[seatNo]->getPosition() - Vec2(0, 70);
        }else{
            boardPoint = _cardListBoard[seatNo]->getPosition() + Vec2(80, 0);
        }
        
        _cardOutList[seatNo] = CardOutList::create(seatNo);
        _cardOutList[seatNo] -> setPosition(boardPoint);
        _cardOutList[seatNo] -> setCallFunction(CC_CALLBACK_1(GameTableUI::outCardFinish, this));
        _tableWidget->addChild(_cardOutList[seatNo] , 3);
	}

    
    
#pragma 创建玩家手牌列表
	void GameTableUI::addHandCardList(BYTE seatNo)
	{
        Size size  = Director::getInstance()->getWinSize();
        
        Vec2 boardPoint;
        if (seatNo == 0) {
            boardPoint = Vec2(size.width/2, 70);
        }else if(seatNo == 1){
            boardPoint = Vec2(size.width - 130  , size.height/2 - 10);
        }
        else if(seatNo == 2){
            boardPoint = Vec2(size.width/2 - 80, _tableWidget->getContentSize().height - 60);
        }else{
            boardPoint = Vec2(130  , size.height/2 - 10);
        }

		_cardListBoard[seatNo] = CardListBoard::create(seatNo == 0, seatNo);
		_cardListBoard[seatNo] -> setPosition(boardPoint);
		//_cardListBoard[seatNo]->setAnchorPoint(ptr->getAnchorPoint());
		_cardListBoard[seatNo]->setCallFunction(CC_CALLBACK_1(GameTableUI::autoOutCallFunction, this),CC_CALLBACK_1(GameTableUI::initCardFinish, this),CC_CALLBACK_1(GameTableUI::playerOutCardResp, this));
        _cardListBoard[seatNo]->setGangActionEndCallFunc(CC_CALLBACK_2(GameTableUI::gangActionEnd, this));
        _tableWidget->addChild(_cardListBoard[seatNo] , 4);
        //ptr->getParent()->addChild(_cardListBoard[seatNo], ptr->getLocalZOrder());
        
        /*
		if (seatNo!=0)
		{
			auto flip=FlipY::create(true);
			_cardListBoard[seatNo]->runAction(flip->reverse());
		}*/
    }
    
    
#pragma mark 回调函数
    void GameTableUI::initCardFinish(MJPostion _dir){
        
        _lastCardCount -= 13;
        
        this->changeLastCardCount(_lastCardCount);
        
        if (_dir == MJPostion::Left) {
            this->setGameBanker(_bankerSeatNo);
            
            YZAudioEngine::getInstance()->playEffect("erMaJiang/music/gamestart.mp3");
        }
    }

    void GameTableUI::playerOutCardResp(BYTE card){
        
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_OUT_CARD);
        
        char strName[40];
        if (_players[0]->getIsBoy()) {
            sprintf(strName, "man_%d.mp3", card);
        }else{
            sprintf(strName, "woman_%d.mp3", card);
        }

        YZAudioEngine::getInstance()->playEffect(FormatSoundFile(strName));

        //YZAudioEngine::getInstance()->playEffect(strName);
        
        _cardListBoard[0]->outCard(card);
        //_cardListBoard[0]->playerOutCard(card);
        
        _tableLogic->sendOutCard(card);
    }
    
    void GameTableUI::outCardFinish(Vec2 point){
        outIndexSp->setPosition(point);
        outIndexSp->setVisible(true);
    }

    
#pragma mark 出牌回调
    
	void GameTableUI::autoOutCallFunction(cocos2d::Node* pNode)
	{
        CCLOG("出牌回调...");
        
        MJOutCard outCardValue = static_cast<CardListBoard *>(pNode)->getCanOutCardValue();
        
       _cardOutList[outCardValue.seatNo]->addCard2OutList(outCardValue);
	}
    
    void GameTableUI::gangActionEnd(BYTE seatNo , BYTE cardValue){
        if (m_bHaveGangAction) {
            this->scheduleUpdate();
            m_bHaveGangAction = false;
            m_bGangActionEnd = true;
        }
    }

    
    void GameTableUI::update(float dt){
        
        if (m_bGangActionEnd) {
            //this->unscheduleUpdate();
            m_bGangActionEnd = false;
            if (gangActionCard.size() > 0) {
                _cardListBoard[gangActionSeatNo]->addOneCard(gangActionCard.at(0));
            }
        }
        
        if (!_isPlayVoice && voicesList.size() > 0) {
            this->doPlayUserVoice(voicesList.front());
        }
    }

    void GameTableUI::updateTime(float dt){
        label_time->setString(DateUtils::getNowTime());
        
        tempTime -= dt;
        if (tempTime <= 0) {
            tempTime = 10;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            NetState state = SystemInfoManager::getInstance()->getNetState();
            
            char name[60];
            sprintf(name,"erMaJiang/netState%d.png",state);
            _imgNetState->loadTexture(name);
            
            float batteryLevel = SystemInfoManager::getInstance()->getBatteryLeve();
            
            if (batteryLevel > 0) {
                _batteryLevel->setPercent(batteryLevel * 100.0f);
            }else{
                jzError("不能获取设备的电池信息...");
                _batteryLevel->setPercent(50);
            }
#endif
        }
    }
    
    
    void GameTableUI::onGameDisConnected(){
        if(socketErrorLayer != NULL){
            socketErrorLayer->setVisible(true);
        }
    }
    
	GameTableUI::GameTableUI()
	{
		initParams();
	}

	GameTableUI::~GameTableUI()
	{
        this->unscheduleUpdate();
        this->unschedule(schedule_selector(GameTableUI::updateTime));
        YZ_SAFE_DELETE(_tableLogic);
	}

	//========================================================//
	
    void GameTableUI::movePlayerSeatPoint(){
        Size winSize = Director::getInstance()->getVisibleSize();
        Point playerPoint[4];
        playerPoint[0] = Point(0 , winSize.height * .2);
        playerPoint[1] = Point(winSize.width - 100 , _tableWidget->getContentSize().height * .61);
        playerPoint[2] = Point(winSize.width * .78f , _tableWidget->getContentSize().height * .83);
        playerPoint[3] = Point(0 , winSize.height * .65);
        
        for(int i = 0; i < PLAYER_COUNT; i++)
        {
            _players[i]->runAction(MoveTo::create(1.5f, playerPoint[i]));
        }
    }
    
	void GameTableUI::showGameShaiZi(BYTE cbSiceCount[2]){

        _gameStart = true;

        if (m_iNowGameCount == 0) {
            this->movePlayerSeatPoint();
        }

        YZAudioEngine::getInstance()->playEffect("erMaJiang/music/throwchips.mp3");

        _btnReady->setVisible(false);
        
        int sice1Count = cbSiceCount[0];
        int sice2Count = cbSiceCount[1];
        
        CCLOG("First ShaiZi Count : %d" , sice1Count);
        CCLOG("Seoned ShaiZi Count : %d" , sice2Count);
        
        Size size = Director::getInstance()->getVisibleSize();
        
        char shaiZiName1[60];
        sprintf(shaiZiName1,"erMaJiang/anim/chip/shaizi_%d.png",sice1Count);
        
        Sprite * shaiZi1 = Sprite::create(shaiZiName1);
        shaiZi1->setPosition(size.width/2 - shaiZi1->getContentSize().width , size.height/2);
        shaiZi1->setVisible(false);
        this->addChild(shaiZi1 , 99 , MAJIANG_TAG_SHAIZI1);
        
        char shaiZiName2[60];
        sprintf(shaiZiName2,"erMaJiang/anim/chip/shaizi_%d.png",sice2Count);
        
        Sprite * shaiZi2 = Sprite::create(shaiZiName2);
        shaiZi2->setPosition(size.width/2 + shaiZi2->getContentSize().width , size.height/2);
        shaiZi2->setVisible(false);
        this->addChild(shaiZi2 , 99 , MAJIANG_TAG_SHAIZI2);
        
        Sprite* pSprite = NULL;
        Animation* pAnimation = Animation::create();
        for (int i = 1; i <= 10; i++)
        {
            char strImagePath[128] = { 0 };
            sprintf(strImagePath, "erMaJiang/anim/chip/anim_%d.png", i);
            if (1 == i)
            {
                pSprite = Sprite::create(strImagePath);
                pSprite->setPosition(size.width / 2, size.height/2);
                pSprite->setScale(1);
                pSprite->setLocalZOrder(200);
                pSprite->setName("btnAction");
                this->addChild(pSprite , 99);
            }
            pAnimation->addSpriteFrameWithFile(strImagePath);
        }
        pAnimation->setDelayPerUnit(1.0f / 10.0f);
        pAnimation->setRestoreOriginalFrame(true);
        
        Animate* pAnimate = Animate::create(pAnimation);
        if (NULL == pAnimate)
            return;
        
        auto fun = CallFunc::create([=]{
            pSprite->setVisible(false);
            pSprite->removeFromParentAndCleanup(true);
            shaiZi1->setVisible(true);
            shaiZi2->setVisible(true);
            
            for (int i = 0; i < PLAYER_COUNT; i++)
            {
                showUserReady(i,false,false);
            }
            
            shaiZi1->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]{shaiZi1->removeFromParentAndCleanup(true); shaiZi2->removeFromParentAndCleanup(true);}), NULL));
        });
        
        pSprite->runAction(Sequence::create(pAnimate , fun , NULL));
    }
    
    
    void GameTableUI::showPlayerGetCard(BYTE seatNo, BYTE value){
        
        _tableLogic->setCurrentPlaySeatNo(seatNo);

        //如果拥有杠动作，则新抓的牌暂时不加入到牌堆中
        if (m_bHaveGangAction) {
            gangActionCard.push_back(value);
        }else{
            _lastCardCount --;
            this->changeLastCardCount(_lastCardCount);
            _cardListBoard[seatNo]->addOneCard(value);
        }
    }
    
    void GameTableUI::showGameBanker(BYTE seatNo , BYTE bankerCard , BYTE bankerAciton , BYTE actionCard ,bool bankerFourHZ) {
        _bankerCard = bankerCard;
        _bankerSeatNo = seatNo;
        _bankerFirstAction = bankerAciton;
        _bankerFirstActionCard = actionCard;
        _bankerFourHZ = bankerFourHZ;
        _curGameBanker = seatNo;
    }
    
    void GameTableUI::setGameBanker(BYTE seatNo){
        this->showPlayerGetCard(seatNo, _bankerCard);
        this->setWaitTime(seatNo, ErMahjong_Game_Time::OUTCARD_TIME, true, 1);
        
        if (seatNo == 0 && _bankerFirstAction != ActionType::WIK_NULL) {
            
            if ((_bankerFirstAction & ActionType::WIK_GANG) != 0) {
                this->showActionBtn(seatNo ,_bankerFirstAction , _bankerFirstActionCard);
            }
            
            if ((_bankerFirstAction & ActionType::WIK_CHI_HU) != 0) {
                
                if (_bankerFourHZ) {
                    this->showActionBtn(seatNo ,_bankerFirstAction , _caiShenCard);
                }else{
                     this->showActionBtn(seatNo ,_bankerFirstAction , _bankerCard);
                }
                //this->sendActRequest(ActionType::WIK_CHI_HU,pData->cbCardData);
            }
            
            //this->showActionBtn(seatNo ,_bankerFirstAction , _bankerFirstActionCard);
            _bankerFirstAction = 0;
            _bankerFirstActionCard = 0;
        }
        
        //设置玩家方位
        for (int i  = 0; i< PLAYER_COUNT; i++) {
            if (i == seatNo) {
                _players[i]->setBanker(true);
            }else{
                _players[i]->setBanker(false);
            }
        }
    }
    
    
    void GameTableUI::showUserTrustee(BYTE seatNo ,bool bTrustee){
        
        if (seatNo == 0) {
            this->showCAutoOutCardBtn(bTrustee);
        }
        
        _players[seatNo]->setAutoHead(bTrustee);
    }
    
    void GameTableUI::setGameZMCount(int zmCount) {
        char zmName[5];
        sprintf(zmName, "%d", zmCount);
        label_niaoNum->setString(zmName);
    }

    
#pragma mark 游戏结束
    
    
    void GameTableUI::showNextZmImage(std::vector<BYTE> cards,std::vector<ImageView *> showCards){
        nowShowZmIndex++;
        
        if (nowShowZmIndex >= cards.size()) {
            CCLOG("扎码显示完成...");
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([=]{
                _resultWidget -> setVisible(true);
                this->hideZmLayers();
            })));
            return;
        }
        
        auto m_pCardBack  = Sprite::create("erMaJiang/positonMark/ownMark.png");
        m_pCardBack->setPosition(showCards.at(nowShowZmIndex)->getPosition());
        m_pCardBack->setLocalZOrder(showCards.at(nowShowZmIndex)->getLocalZOrder() + 1);
        showCards.at(nowShowZmIndex)->getParent()->addChild(m_pCardBack);
        // 动画序列（延时，显示，延时，隐藏）
        Sequence *pBackSeq = Sequence::create(DelayTime::create(0.2f),Show::create(),DelayTime::create(0.2f),Hide::create(),NULL);
        //持续时间、半径初始值、半径增量、仰角初始值、仰角增量、离x轴的偏移角、离x轴的偏移角的增量
        OrbitCamera *pBackCamera = OrbitCamera::create(0.5f, 1, 0, 0, 90, 0, 0);
        Spawn* pSpawnBack = Spawn::create(pBackSeq,pBackCamera,NULL);
        m_pCardBack->runAction(pSpawnBack);
        
        std::string fileName = this->getCardTextureFileName(cards.at(nowShowZmIndex),true);
        showCards.at(nowShowZmIndex)->loadTexture(fileName);
        showCards.at(nowShowZmIndex)->setVisible(true);
        
        // 动画序列（延时，隐藏，延时，显示）
        Sequence *pFrontSeq = Sequence::create(DelayTime::create(0.2f),Hide::create(),DelayTime::create(0.2f),Show::create(),NULL);
        OrbitCamera *pLandCamera = OrbitCamera::create(0.7f, 1, 0, 0, 360, 0, 0);
        Spawn* pSpawnFront = Spawn::create(pFrontSeq,pLandCamera,NULL);
        showCards.at(nowShowZmIndex)->runAction(Sequence::create(pSpawnFront , CallFunc::create([=]{
            this->showNextZmImage(cards, showCards);
            m_pCardBack->removeFromParentAndCleanup(true);
        }), NULL));

        
        /*
        showCards.at(nowShowZmIndex)->setVisible(true);
        showCards.at(nowShowZmIndex)->runAction(Sequence::create(ScaleTo::create(0.3f, 0.1f),CallFunc::create([=]{
            std::string fileName = this->getCardTextureFileName(cards.at(nowShowZmIndex),true);
            showCards.at(nowShowZmIndex)->loadTexture(fileName);
        }),ScaleTo::create(0.3, 1.0f),CallFunc::create([=]{
            this->showNextZmImage(cards, showCards);
        }), NULL));
         */
    }
    
    
    bool checkVectorHaveIntValue(std::vector<BYTE> vec , BYTE value){
        for (int i = 0; i<vec.size(); i++) {
            if (vec.at(i) == value) {
                return true;
            }
        }
        return false;
    }
    
    void GameTableUI::doGameEndResp(MJGameEndData data , std::map<BYTE, std::vector<BYTE>> actionCardInfoMap ,std::map<BYTE , std::vector<BYTE>> cardInfoMap){
        
        Size winSize = Director::getInstance()->getVisibleSize();
        _resultWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
        _resultWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        
        listenerCardLayout->setVisible(false);
        
        std::vector<BYTE> cards;
        this->showListenerCard(false, cards);
        this->hideAllAcitonBtn();
        this->setWaitTime(0, 20, false, 1);

        if(!data.bLiuJu){
            this->playActionSound(0, ActionType::WIK_CHI_HU);
        }

        //如果自己的胡牌值不为0， 表示自己胡牌了
        if (data.cbHuCard[0] != 0) {
            YZAudioEngine::getInstance()->playEffect("erMaJiang/music/mywin.mp3");
        }else{
            YZAudioEngine::getInstance()->playEffect("erMaJiang/music/sidewin.mp3");
        }
        
        for (Node * node  : clearNodes) {
            node->removeFromParentAndCleanup(true);
        }
        
       for (int i = 0; i <  GAME_PLAYER;i++) {
           if (data.cbHuCard[i] != 0 ) {
               this->showActionAnimation(i, ActionType::WIK_CHI_HU);
           }
           _cardListBoard[i]->showAllWinCard(data.cbHuCard[i] != 0, data.cbHuCard[i], cardInfoMap.at(i));
       }
        
        clearNodes.clear();
      
        char zmLayerName[60];
        sprintf(zmLayerName, "zm%dBG", (int)data.cbZaCardData.size());
        auto zmLayer = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, zmLayerName));
        zmLayer->setVisible(true);
        
        std::vector<ImageView *> showCards;
        showCards.clear();
        char zmImageName[60];
        for (int i = 0; i< data.cbZaCardData.size(); i++) {
            sprintf(zmImageName, "zm_img%d", i + 1);
            auto zmImg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(zmLayer, zmImageName));
            zmImg->setVisible(false);
            showCards.push_back(zmImg);
        }
        
        nowShowZmIndex = -1;
        
        this->showNextZmImage(data.cbZaCardData, showCards);
        
        _resultWidget -> setVisible(false);
        
        auto fun = CallFunc::create([=]{
            
            for (int i = 0; i <  GAME_PLAYER;i++) {
                
                //_cardListBoard[i]->showAllWinCard(data.winUser == i , data.cbProvideCard, cardInfoMap.at(i));
                
                char pName[60];
                sprintf(pName, "player%d", i + 1);
                auto player= dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_resultWidget,pName));
                
                if (i == 0) {
                    player->loadTexture("erMaJiang/result/win_di.png");
                }else{
                    player->loadTexture("erMaJiang/result/di.png");
                }
                
                auto headImg= dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player,"headImg"));
                headImg->setVisible(false);
                
                UserInfoStruct* userInfo = _tableLogic->getGameUserByDeskStation(i);
                std::string headUrl = "";
                std::string nickName = "Visitor";
                bool bBoy = true;
                if (userInfo != NULL) {
                    headUrl = userInfo->szHeadUrl;
                    nickName = userInfo->nickName;
                    bBoy = userInfo->bBoy;
                }
                
                JzDebug("游戏结束 头像链接为 : %s " , headUrl.c_str());
                
                //auto httpSp = HttpSprite::create(headUrl, "erMaJiang/result/normalHead.png");
                auto httpSp = HttpSprite::create(headUrl, bBoy ? Player_Normal_M : Player_Normal_W);
                httpSp->setImgSize( Size(60, 60));
                httpSp->setPosition(headImg->getPosition());
                headImg->getParent()->addChild(httpSp , 1);
                
                Text * name = dynamic_cast<ui::Text*>(ui::Helper::seekWidgetByName(player,"Label_24"));
                name->setString(GBKToUtf8(nickName.c_str()));

                auto zjImg= dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player,"img_zj"));
                if(i == _curGameBanker){
                    zjImg->setVisible(true);
                }else{
                    zjImg->setVisible(false);
                }


                /*
                 std::string headUrl =  _tableLogic->getUserByDeskStation(i)->szHeadUrl;
                 auto httpSp = HttpSprite::create(headUrl, "erMaJiang/result/normalHead.png");
                 httpSp->setImgSize( Size(headImg->getContentSize().width * headImg->getScaleX(), headImg->getContentSize().height * headImg->getScaleY()));
                 httpSp->setPosition(headImg->getPosition());
                 headImg->getParent()->addChild(httpSp,headImg->getPositionZ());
                 */
                
                CCLOG("%d 号玩家  动作牌数量 : %lu   手牌数量 ：%lu" , i , actionCardInfoMap.at(i).size() , cardInfoMap.at(i).size());
                
                auto handCard0Point = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player,"handCardPoint"));
                Size cardSize = handCard0Point->getContentSize() * handCard0Point->getScale();
                std::vector<BYTE> actionCards;
                actionCards.clear();
                actionCards = actionCardInfoMap.at(i);
                for (int j = 0; j< actionCards.size(); j++) {
                    std::string fileName = this->getCardTextureFileName(actionCards.at(j), true);
                    if (j == 0) {
                        handCard0Point->loadTexture(fileName);
                        handCard0Point->setLocalZOrder(1);
                    }else{
                        ImageView * cardImg =  ImageView::create(fileName);
                        cardImg->setScale(handCard0Point->getScale());
                        cardImg->setPosition(handCard0Point->getPosition() + Vec2(cardSize.width * j, 0));
                        player->addChild(cardImg);
                        
                        clearNodes.push_back(cardImg);
                    }
                }
                
                std::vector<BYTE> handCards;
                handCards.clear();
                handCards = cardInfoMap.at(i);
                for (int j = 0; j < handCards.size() ; j++) {
                    std::string fileName = this->getCardTextureFileName(handCards.at(j), false);
                    if (actionCards.size() == 0 && j == 0) {
                        handCard0Point->loadTexture(fileName);
                    }else{
                        
                        ImageView * cardImg =  ImageView::create(fileName);
                        cardImg->setScale(handCard0Point->getScale());
                        
                        if (data.cbHuCard[i] != 0 && j == handCards.size() - 1) {
                            ImageView * huTypeImage = NULL;
                            if (data.bZiMo) {
                                huTypeImage  =  ImageView::create("erMaJiang/result/zimo.png");
                            }else{
                                huTypeImage  =  ImageView::create("erMaJiang/result/qgh.png");
                            }
                            huTypeImage->setPosition(handCard0Point->getPosition() + Vec2(cardSize.width * (j + actionCardInfoMap.at(i).size()) + cardSize.width / 2, cardSize.height/2 + huTypeImage->getContentSize().height/2));
                            player->addChild(huTypeImage);
                            
                            clearNodes.push_back(huTypeImage);
                            
                            cardImg->setPosition(handCard0Point->getPosition() + Vec2(cardSize.width * (j + actionCardInfoMap.at(i).size()) + cardSize.width / 2, 0));
                            
                        }else{
                            cardImg->setPosition(handCard0Point->getPosition() + Vec2(cardSize.width * (j + actionCardInfoMap.at(i).size()), 0));
                        }
                        
                        player->addChild(cardImg);

                        clearNodes.push_back(cardImg);
                    }
                }
                
                auto zmCard0Point = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(player,"zmCardPoint"));
                if (data.cbHuCard[i] != 0) {
                    for (int j = 0 ; j < data.cbZaCardData.size(); j++) {
                        std::string fileName = this->getCardTextureFileName(data.cbZaCardData.at(j), false);
                        if (j == 0) {
                            ImageView * cardImg =  ImageView::create("erMaJiang/result/zhongma.png");
                            cardImg->setPosition(zmCard0Point->getPosition() + Vec2(cardSize.width /2 , cardSize.height/2 + cardImg->getContentSize().height/2));
                            player->addChild(cardImg);
                            
                            zmCard0Point->setVisible(true);
                            zmCard0Point->loadTexture(fileName);
                            
                            if (checkVectorHaveIntValue(data.cbZhongZaCardData, data.cbZaCardData.at(j))) {
                                
                                ImageView * zhongMaSp = ImageView::create("erMaJiang/result/inMa.png");
                                zhongMaSp->setPosition(Vec2(zmCard0Point->getContentSize().width/2,0) + Vec2(0, 5));
                                zhongMaSp->setScale(.3f);
                                zmCard0Point->addChild(zhongMaSp , 2);
                            
                                clearNodes.push_back(zhongMaSp);
                            }
                            
                            clearNodes.push_back(cardImg);
                        }else{
                            ImageView * cardImg =  ImageView::create(fileName);
                            cardImg->setScale(zmCard0Point->getScale());
                            cardImg->setPosition(zmCard0Point->getPosition() + Vec2(cardSize.width * j, 0));
                            player->addChild(cardImg);
                            clearNodes.push_back(cardImg);
                            
                            if (checkVectorHaveIntValue(data.cbZhongZaCardData, data.cbZaCardData.at(j))) {
                                
                                JzDebug("第 %d 个扎码牌 中码了...." , j);
                                
                                ImageView * zhongMaSp = ImageView::create("erMaJiang/result/inMa.png");
                                zhongMaSp->setScale(.3f);
                                zhongMaSp->setPosition(Vec2(cardImg->getContentSize().width/2,0) + Vec2(0, 5));
                                cardImg->addChild(zhongMaSp , 2);
                            }
                        }
                    }
                }else{
                    zmCard0Point->setVisible(false);
                }
                
                auto lastCardPoint = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_resultWidget,"lastCardPoint"));
                lastCardPoint->setVisible(false);
                Size laseCardSize = lastCardPoint->getContentSize() * lastCardPoint->getScale();
                for (int j = 0; j < data.lastCardVec.size(); j++) {
                    std::string fileName = this->getCardTextureFileName(data.lastCardVec.at(j), false);
                    if (j == 0) {
                        lastCardPoint->setVisible(true);
                        lastCardPoint->loadTexture(fileName);
                    }else{
                        
                        ImageView * cardImg =  ImageView::create(fileName);
                        cardImg->setScale(lastCardPoint->getScale());
                        if (j < 30) {
                            cardImg->setPosition(lastCardPoint->getPosition() + Vec2(laseCardSize.width * j, 0));
                        }else{
                            cardImg->setPosition(lastCardPoint->getPosition() + Vec2(laseCardSize.width * (j - 30), - laseCardSize.height));
                        }
                        _resultWidget->addChild(cardImg);
                        clearNodes.push_back(cardImg);
                    }
                }
                
                
                auto score = dynamic_cast<ui::TextBMFont*>(ui::Helper::seekWidgetByName(player,"score"));
                
                char scoreStr[60];
                sprintf(scoreStr, "%d",data.iChangeMoney[i]);
                score->setString(GBKToUtf8(scoreStr));
                
                _players[i]->changeUserScore(data.iChangeMoney[i]);
            }
        });
        
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), fun));
        
    }
    
    void GameTableUI::showUserChatFaceAnimate(BYTE seatNo, int _faceId)
    {
        int faceAnimCount[15] = {5,4,6,3,7,7,12,6,12,8,6,14,9,9,15};
        
        PlayerUI * player = _players[seatNo];
    
        Sprite* pSprite = NULL;
        Animation* pAnimation = Animation::create();
        for (int i = 1; i <= faceAnimCount[_faceId - 1]; i++)
        {
            char strImagePath[128] = { 0 };
            sprintf(strImagePath, "erMaJiang/chat/expressionanim/anim%d/anim%d_%d.png",_faceId,_faceId,i);
            if (1 == i)
            {
                pSprite = Sprite::create(strImagePath);
                pSprite->setPosition(player->getPosition() + Vec2(player->getContentSize().width/2,player->getContentSize().height/2));
                pSprite->setLocalZOrder(200);
                pSprite->setName("btnAction");
                this->addChild(pSprite , 99);
            }
            pAnimation->addSpriteFrameWithFile(strImagePath);
        }
        pAnimation->setDelayPerUnit(1.0f / faceAnimCount[_faceId - 1]);
        pAnimation->setRestoreOriginalFrame(true);
        
        Animate* pAnimate = Animate::create(pAnimation);
        
        pSprite->runAction(Sequence::create(Repeat::create(pAnimate, 3), CallFunc::create([=]{pSprite->removeFromParentAndCleanup(true);}) , NULL));
    }
    
    void GameTableUI::showUserChatTalkMsg(int type , BYTE seatNo, std::string _msg)
    {
        if (!isValidSeat(seatNo)) {
            JzLog::shareLog()->log(kLogLevelError, "显示玩家的聊天信息Error.. 错误的座位号 : %d" ,seatNo);
            return;
        }
        
        std::string msg = _msg;

        if (type == 0) {
            int index = std::atoi(_msg.c_str());
            
            std::string filename = "";
            _players[seatNo]->getIsBoy() ? filename.append("chat_boy/voice_") : filename.append("chat_girl/voice_");
            filename.append(_msg.c_str());
            filename.append(".mp3");
            
            YZAudioEngine::getInstance()->playEffect(FormatSoundFile(filename.c_str()));
            
            ui::Text* txt = dynamic_cast<ui::Text*>(chatWidget.at(index)->getChildByName("msg"));

            msg = txt->getString();
        }
        
        PlayerUI * player = _players[seatNo];
        
        //1.显示聊天信息到场景
        string fileName = CHAT_BG_LEFT;
        Vec2 anchorPoint = Vec2::ANCHOR_BOTTOM_LEFT;
        Vec2 point = Vec2(player->getContentSize().width/2,player->getContentSize().height/2);
        
        if (seatNo == 1 || seatNo == 2) {
            fileName = CHAT_BG_RIGHT;
            anchorPoint = Vec2::ANCHOR_BOTTOM_RIGHT;
            point = Vec2(-player->getContentSize().width/2,player->getContentSize().height/2);
        }
        
        point += player->getPosition();
        
        _fontSize = 20;
        
        auto sp = Sprite::create(fileName);
        sp->setAnchorPoint(anchorPoint);
        sp->setScale(msg.length() * 6 / sp->getContentSize().width + 0.25f, 0.7f);
        sp->setPosition(point.x  , point.y);
        _tableWidget->addChild(sp,20);
        
        LabelTTF * label = LabelTTF::create(msg, "Marker Felt", _fontSize);
        label->setAnchorPoint(anchorPoint);
        label->setPosition(sp->getPosition() + Vec2(8, sp->getContentSize().height/4));
        label->setColor(colorBlack);
        label->setFontSize(_fontSize);
        label->setFontName(_fontName);
        _tableWidget->addChild(label,21);
        
        sp->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
            sp->removeFromParentAndCleanup(true);
            label->removeFromParentAndCleanup(true);
        })));
        
        //2. 添加到聊天框
        
        std::string sendMsg = player->getUserName() + " : " + msg;
        this->showChatMsg(sendMsg);
    
    }
    
    void GameTableUI::bgSliderEvent(cocos2d::Ref *pSender, SliderEventType type)
    {
        if (type == SLIDER_PERCENTCHANGED) {
            Slider* slider = dynamic_cast<Slider*>(pSender);
            CCLOG("Volume : %d" , slider->getPercent());
            YZAudioEngine::getInstance()->setBackgroundMusicVolume(slider->getPercent() / 100.0);
        }
    }
 
    void GameTableUI::soundSliderEvent(cocos2d::Ref *pSender, SliderEventType type)
    {
        if (type == SLIDER_PERCENTCHANGED) {
            Slider* slider = dynamic_cast<Slider*>(pSender);
            CCLOG("Volume : %d" , slider->getPercent());
            YZAudioEngine::getInstance()->setEffectsVolume(slider->getPercent() / 100.0);
        }
    }
    
    void GameTableUI::isLastGame(int nowGameCount , int totalGameCount){
        JzDebug("显示当前游戏局数： 当前 ： %d  最大 : %d" ,nowGameCount , totalGameCount);
        
        this->changeRoomUIByPlayCount(nowGameCount);
        
        m_iNowGameCount = nowGameCount;
        m_iMaxGameCount = totalGameCount;
        _isLastGame = nowGameCount == totalGameCount;
        char num[60]={0};
        sprintf(num, "%d/%d局",nowGameCount,totalGameCount);
        label_GameTimes->setString(num);
    }
    
    void GameTableUI::userWant2DisbandRoom(bool isShow){
        Size winSize = Director::getInstance()->getWinSize();
        
        disbandRoomLayout->setVisible(isShow);
        
        if (!isShow) {
            return;
        }
        
        auto word = (ImageView*)Helper::seekWidgetByName(disbandRoomLayout, "Image_word");

        if(m_iNowGameCount == 0){
            word->loadTexture("erMaJiang/disbandRoomInfo0.png");
        }else{
            word->loadTexture("erMaJiang/disbandRoomInfo.png");
        }

        
        auto Button_OK = (Button*)Helper::seekWidgetByName(disbandRoomLayout, "btn_ok");
        Button_OK->addTouchEventListener(CC_CALLBACK_2(GameTableUI::disbandClickCallback, this));
        auto Button_Cancel = (Button*)Helper::seekWidgetByName(disbandRoomLayout, "btn_cancle");
        Button_Cancel->addTouchEventListener(CC_CALLBACK_2(GameTableUI::disbandClickCallback, this));
        
        Button_OK->setTag(TAG_TURE_DISBAND);
        Button_Cancel->setTag(TAG_CANEL_DISBAND);
    }
    
    void GameTableUI::showUserAskDisbandRoom(bool isShow){
        
        Size winSize = Director::getInstance()->getWinSize();
        
        disbandRoomLayout->setVisible(isShow);
        
        if (!isShow) {
            return;
        }
        
        auto word = (ImageView*)Helper::seekWidgetByName(disbandRoomLayout, "Image_word");
        word->loadTexture("erMaJiang/disbandRoomInfo.png");
        
        auto Button_OK = (Button*)Helper::seekWidgetByName(disbandRoomLayout, "btn_ok");
        Button_OK->addTouchEventListener(CC_CALLBACK_2(GameTableUI::disbandClickCallback, this));
        auto Button_Cancel = (Button*)Helper::seekWidgetByName(disbandRoomLayout, "btn_cancle");
        Button_Cancel->addTouchEventListener(CC_CALLBACK_2(GameTableUI::disbandClickCallback, this));
        
        Button_OK->setTag(Tag_Agree_Disband);
        Button_Cancel->setTag(Tag_NoAgree_Disband);
    }
    
    void GameTableUI::disbandClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
    {
        if (Widget::TouchEventType::ENDED != touchtype)	return;
        
        
        Button* ptr = (Button*)pSender;
        switch (ptr->getTag())
        {
            case Tag_Agree_Disband:
                this->showUserAskDisbandRoom(false);
                _tableLogic->sendAgreeDisbandRoom(true);
            break;
                
            case Tag_NoAgree_Disband:
                this->showUserAskDisbandRoom(false);
                _tableLogic->sendAgreeDisbandRoom(false);
                break;
                
            case TAG_TURE_DISBAND:
            {
                this->userWant2DisbandRoom(false);
                if (_tableLogic->checkUserIsDeskOwner()) {
                    _tableLogic->sendDisbandRoomResp(true);
                }else{
                    if (m_iNowGameCount == 0) {
                        _tableLogic->sendUserUp();
                        //_tableLogic->sendForceQuit();
                    }else{
                        _tableLogic->sendDisbandRoomResp(true);
                    }
                }
            }
                break;
                
            case TAG_CANEL_DISBAND:
                this->userWant2DisbandRoom(false);
                break;
        }
    }
    
    void GameTableUI::showListenerCard(bool isShow, std::vector<BYTE> cards){
        
        for (int i = 0; i< _listenerCardVec.size(); i++) {
            _listenerCardVec.at(i)->setVisible(false);
        }
        
        listenerCardLayout->setVisible(isShow);
        
        if (isShow && cards.size() > 0) {
            if (cards.size() > _listenerCardVec.size()) {
                jzError("listener card size is error...  size : %lu" , cards.size());
                return;
            }
            
            auto bard = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(listenerCardLayout,"bard"));
            bard->setTouchEnabled(false);
            bard->setSwallowTouches(true);
            bard->setScale((cards.size() + 1.5)/ 28.0, 1);
            
            for (int i = 0; i < cards.size(); i++) {
                _listenerCardVec.at(i)->loadTexture(getCardTextureFileName(cards.at(i), false));
                _listenerCardVec.at(i)->setVisible(true);
            }
        }
    }
    
    
    void GameTableUI::touchVoiceEvent(Ref *pSender, Widget::TouchEventType type){
        
        if (!_canRecording) {
            CCLOG("暂时不能录音...");
            return;
        }
        
        if (type == Widget::TouchEventType::BEGAN ) {
            CCLOG("开始录音...");
            this->showVoiceLayer(true);
        }else if(type == Widget::TouchEventType::ENDED){
            if (!_isVoiceRecordingTimeEnd) {
                this->voiceEnd();
            }
        }
    }
        
    //录音进度
    void GameTableUI::updateProgress(float Dt)
    {
        voiceTime += Dt;
        oneVoiceTime -= Dt;
        if (oneVoiceTime <= 0) {
            
            if (voiceShowIndex > 3) {
                voiceShowIndex = 0;
                for (int i = 0; i< _voiceProgressVec.size(); i++) {
                    _voiceProgressVec.at(i)->setVisible(false);
                }
            }
            _voiceProgressVec.at(voiceShowIndex)->setVisible(true);
            voiceShowIndex++;
            oneVoiceTime = .5f;
        }
        
        if (voiceTime >= 10)
        {
            _isVoiceRecordingTimeEnd = true;
            this->voiceEnd();
        }
    }
    
    void GameTableUI::voiceEnd(){
        _canRecording = false;
        voiceLayout->setVisible(false);
        this->unschedule(schedule_selector(GameTableUI::updateProgress));//取消定时器
        
        this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([=]{
            if (voiceTime >= 0.5f) {
                soundUtil->recordEnd();
            }else{
                soundUtil->recordEnd(false);
                _canRecording = true;
            }
        }), NULL));
    }
    
    void GameTableUI::showVoiceLayer(bool isShow){
        
        _isVoiceRecordingTimeEnd = false;
        
        voiceLayout->setVisible(isShow);
        if (isShow) {
            
            YZAudioEngine::getInstance()->pauseBackgroundMusic();
            
            //显示界面并开始录音
            voiceTime = 0.0f;
            voiceShowIndex = 0;
            oneVoiceTime = 0;
            
            for (int i = 0; i< _voiceProgressVec.size(); i++) {
                _voiceProgressVec.at(i)->setVisible(false);
            }
            
            auto filePath = FileUtils::getInstance()->getWritablePath();
            
            myVoiceName = __String::createWithFormat("Voice_%d",RoomLogic()->loginResult.pUserInfoStruct.dwUserID)->getCString() + DateUtils::getCurrentTime(false);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            this->schedule(schedule_selector(GameTableUI::updateProgress));//更加实际情况来更新进度.这里用定时器以便演示
#endif
            soundUtil->recordStart(myVoiceName);
        }
    }
    
    void GameTableUI::uploadFileEnd(std::string fileName){
        //发送录音
        _tableLogic->sendChatTalkWithId(2,__String::createWithFormat("%s-%d" , myVoiceName.c_str() , (int)ceil(voiceTime))->getCString());
    }
    
    void GameTableUI::downloadFileEnd(std::string _fileName){
        auto filePath = FileUtils::getInstance()->getWritablePath();
        
        CCLOG("播放声音 : %s" , _fileName.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

            JniMethodInfo minfo;
            bool isHave = false;

            isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/audios/AudioRecordTools","playChatRecord", "(Ljava/lang/String;Ljava/lang/String;)V");

            if (isHave)
            {
                jstring aFilePath = minfo.env->NewStringUTF(filePath.c_str());
                jstring aFileName = minfo.env->NewStringUTF(_fileName.c_str());
                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , aFilePath , aFileName);
                minfo.env->DeleteLocalRef(aFilePath);
                minfo.env->DeleteLocalRef(aFileName);
            }else{
                JzDebug("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
            }
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        WeChartManager::sharedInterstitial()->playRecorder(filePath, _fileName);
#endif
        
    }
    
    
    GameTableUI * GameTableUI::_instance = nullptr;
    
    GameTableUI*  GameTableUI::getInstance()
    {
        if (!_instance)
        {
            _instance = new GameTableUI();
        }
        return _instance;
    }
    
    string GameTableUI::getCardTextureFileName(BYTE cardValue, bool isAction){
        BYTE cbColor=(cardValue&MASK_COLOR)>>4;
        BYTE cbValue=(cardValue&MASK_VALUE);
        
        char filename[60];
        
        if (isAction)
        {
            switch (cbColor)
            {
                case MJColor::Wan:
                    sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                    break;
                case MJColor::Tiao:
                    sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                    break;
                case MJColor::Bing:
                    sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                    break;
                case MJColor::Feng:
                    sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                    break;
            }
        }else{
            switch (cbColor)
            {
                case MJColor::Wan:
                    sprintf(filename,"erMaJiang/positonMark/w_%d.png",cbValue);
                    break;
                case MJColor::Tiao:
                    sprintf(filename,"erMaJiang/positonMark/tiao_%d.png",cbValue);
                    break;
                case MJColor::Bing:
                    sprintf(filename,"erMaJiang/positonMark/tong_%d.png",cbValue);
                    break;
                case MJColor::Feng:
                    sprintf(filename,"erMaJiang/positonMark/feng_%d.png",cbValue);
                    break;
            }
        }
        
        return filename;
    }
    
    
    void GameTableUI::showGameMsgBar(bool isShow){
        
        if (gameMsgBar->isVisible() == isShow) {
            return;
        }
        
        gameMsgBar->setVisible(isShow);
        
        if (isShow) {
            label_msg->stopAllActions();
            Size ppos = gameMsgBar->getContentSize();
            label_msg->setPosition(Vec2(ppos.width*1.1f,ppos.height/2));
            
            auto seq = Sequence::create(MoveTo::create(20.0f,Vec2(-label_msg->getContentSize().width,ppos.height/2)),
                                        DelayTime::create(1.0f),
                                        CallFunc::create([=]()
                                                         {
                                                             label_msg->setPosition(Vec2(ppos.width*1.1f,ppos.height/2));
                                                             this->showGameMsgBar(false);
                                                         }),NULL);
            
            label_msg->runAction(RepeatForever::create(seq));
        }else{
            label_msg->stopAllActions();
        }
    }
    
    
    void GameTableUI::msgBar(float dt){
        if (!gameMsgBar->isVisible()) {
            this->showGameMsgBar(true);
        }
    }
    
    void GameTableUI::setGamePlayerPoint(BYTE seatNo){
        
        auto panel_gameTime = dynamic_cast<ui::Layout*>(ui::Helper::seekWidgetByName(_tableWidget, "Panel_clock"));
        panel_gameTime->setVisible(false);
        
        auto label = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(panel_gameTime, "AtlasLabel_timeCount"));
        
        switch (seatNo) {
            case 0:
                panel_gameTime->setRotation(-90);
                label->setRotation(90);
                break;
            case 1:
                panel_gameTime->setRotation(0);
                label->setRotation(0);
                break;
            case 2:
                panel_gameTime->setRotation(90);
                label->setRotation(-90);
                break;
            case 3:
                panel_gameTime->setRotation(180);
                label->setRotation(-180);
                break;

        }
    }
    
    void GameTableUI::changeLastCardCount(int count){
        _roomInfoCardCountTxt->setString(__String::createWithFormat("%d" , count)->getCString());
    }
    
    void GameTableUI::showUserVoice(BYTE seatNo, std::string voiceName , int voiceTime)
    {
        JzDebug("%d 号玩家发送了语音聊天..  文件名为 : %s 时长 : %d " , seatNo ,voiceName.c_str(),voiceTime);
        
        Voice voice;
        voice.voiceSeatNo = seatNo;
        voice.voiceName = voiceName;
        voice.voiceLength = voiceTime;
        voicesList.push_back(voice);
    }
    
    void GameTableUI::doPlayUserVoice(Voice voice){
        
        _isPlayVoice = true;
        
        PlayerUI * player = _players[voice.voiceSeatNo];
        
        YZAudioEngine::getInstance()->pauseBackgroundMusic();
        
        Sprite* pSprite =  Sprite::create("erMaJiang/qipao.png");
        pSprite->setScale(0.6f);
        pSprite->setPosition(player->getPosition() + Vec2(player->getContentSize().width/2,player->getContentSize().height / 1.5f));
        pSprite->setLocalZOrder(200);
        this->addChild(pSprite , 99);
        
        pSprite->runAction(Sequence::create(DelayTime::create(voice.voiceLength),CallFunc::create([=]{
            pSprite->setVisible(false);
        }),DelayTime::create(1.0f),CallFunc::create([=]{
            pSprite->removeFromParentAndCleanup(true);
            YZAudioEngine::getInstance()->resumeBackgroundMusic();
            voicesList.erase(voicesList.begin());
            _isPlayVoice = false;
        }), NULL));
        
        if (voice.voiceSeatNo != 0) {
            soundUtil->requestDownloadRecordFile(voice.voiceName);
           // FTPFileManager::GetInst()->downloadFile2LocalByFtp(voice.voiceName);
        }else{
            soundUtil->recordVoicePlay(voice.voiceName);
            _canRecording = true;
        }
    }
    
    void GameTableUI::showUserOffLine(BYTE seatNo , bool offLine){
        _players[seatNo]->setPlayerOffLine(offLine);
    }
    
    
    void GameTableUI::voiceStart(){
        voiceTime = 0.0f;
        voiceShowIndex = 0;
        oneVoiceTime = 0;
        
        for (int i = 0; i< _voiceProgressVec.size(); i++) {
            _voiceProgressVec.at(i)->setVisible(false);
        }
        this->schedule(schedule_selector(GameTableUI::updateProgress));//更加实际情况来更新进度.这里用定时器以便演示
    }

    void GameTableUI::systemInfoChange(int type, int value){
        //代表电量改变
        if (type == 1) {
            if (value > 0) {
                _batteryLevel->setPercent(value);
            }else{
                jzError("不能获取设备的电池信息...");
                _batteryLevel->setPercent(50);
            }
        }else{
            CCLOG("网络改变 ... 当前网络为 : %d" , value);
            char name[60];
            sprintf(name,"erMaJiang/netState%d.png",value);
            CCLOG("获取网络图片纹理... %s" , name);
            _imgNetState->loadTexture(name);
        }
    }
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    extern "C"
    {

        JNIEXPORT void JNICALL Java_com_dada_audios_AudioRecordTools_nativeRecorderStart() {

            CCLOG("Java_com_dada_tools_MediaTools_nativeRecorderStart");
            
            GameTableUI::getInstance()->voiceStart();
        }
        
        
        JNIEXPORT void JNICALL Java_com_dada_AppActivity_SystemInfoNative(JNIEnv *env, jobject thiz, jint type,jint value) {
            CCLOG("Java_com_dada_AppActivity_SystemInfoNative");
            GameTableUI::getInstance()->systemInfoChange(type , value);
        }

    }
    
#endif
    
}
