//
//  RecordLayer.cpp
//  PDKGame
//
//  Created by Dada on 16/7/12.
//
//

#include "RecordLayer.h"
#include "ShareLayer.h"
#include "Sound/YZAudioEngine.h"
static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// Â≠êËäÇÁÇπÂºπÂá∫Ê°ÜÂ±ÇÁ∫ß

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// Â≠êËäÇÁÇπÂºπÂá∫Ê°ÜÂ±ÇÁ∫ß

static const char * RECORD_LAYER = "platform/openRoom/Common/RecordLayer.json";     //ÊâìÂºÄËÆ∞ÂΩï

static const char * RECORD_ROOM_LAYER = "platform/openRoom/Common/RecordRoomCell.json";     //ÊâìÂºÄËÆ∞ÂΩï

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

RecordLayer::RecordLayer()
{
    
}

RecordLayer::~RecordLayer()
{
    YZRoomLogic::getInstance()->removeEventSelector(MDM_GR_ROOM, ASS_GR_QUERY_GAMERECORD_PDK);
}

bool RecordLayer::init()
{
    if ( !YZLayer::init() )
    {
        return false;
    }
    m_list_count=0;
    allRecordDatas.clear();
    curRecordIndex = 0;
    recordIndex = 0;
    //memset(&m_item_record, 0, sizeof(m_item_record));
    
    Size winSize = Director::getInstance()->getVisibleSize();
    _recordLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(RECORD_LAYER));
    _recordLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
    _recordLayout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(_recordLayout);
    
    listView_record =(ListView*)Helper::seekWidgetByName(_recordLayout, "ListView_record");
    Layout* pModelItem = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(RECORD_ROOM_LAYER));
    listView_record->setItemModel(pModelItem);
    
    auto bg = (ImageView*)Helper::seekWidgetByName(_recordLayout, "Image_bg");
    bg->setScale(winSize.width / bg->getContentSize().width , winSize.height / bg->getContentSize().height);
    bg->setAnchorPoint(Vec2(0.5f, 0.5f));
    bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    bg->setVisible(true);
    
    noRecordImg = (ImageView*)Helper::seekWidgetByName(_recordLayout, "null_recordImg");
    noRecordImg->setVisible(false);
    
    auto btn_share = (Button*)Helper::seekWidgetByName(_recordLayout, "btn_share");
    btn_share->setTag(RRCORDLAYER_BTNSHARE_TAG);
    btn_share->addTouchEventListener(CC_CALLBACK_2(RecordLayer::menuEventCallBack, this));
    
    auto btn_exit = (Button*)Helper::seekWidgetByName(_recordLayout, "btn_exit");
    btn_exit->setTag(RRCORDLAYER_BTNEXIT_TAG);
    btn_exit->addTouchEventListener(CC_CALLBACK_2(RecordLayer::menuEventCallBack, this));
    
    MSG_GM_C_QUERY_GAMERECORD_PDK record_list;
    record_list.dwRoomID=RoomLogic()->getSelectedRoom()->uRoomID;
    record_list.dwUserID=PlatformLogic()->getInstance()->loginResult.dwUserID;
    record_list.dwDeskIndex=0;
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_QUERY_GAMERECORD_PDK, &record_list, sizeof(record_list));
    YZRoomLogic::getInstance()->addEventSelector(MDM_GR_ROOM, ASS_GR_QUERY_GAMERECORD_PDK, YZ_SOCKET_CALLBACK(RecordLayer::recordrankingEvent, this));
    
    return true;
}

int getRoleInVectorIndex(std::vector<RoleInfo> roleInfos, int userId){

    for (int i = 0; i< roleInfos.size(); i++) {
        if (roleInfos.at(i).dwUserID == userId) {
            return i;
        }
    }
    
    return -1;
}

bool RecordLayer::recordrankingEvent(YZSocketMessage* socketMessage)
{
    //UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    if (bHandleCode==0)
    {
        //CCLOG("==============================");
        
        MSG_GM_S_QUERY_GAMERECORD_PDK* record_list = (MSG_GM_S_QUERY_GAMERECORD_PDK*)socketMessage->object;
        
        //CCLOG("Room Num  : %s" , record_list->szVirRoomID);
        
        //如果房间记录存在， 则更新值
        if (allRecordDatas.find(record_list->szVirRoomID) == allRecordDatas.end()) {
            //不存在则创建记录
            RecordData *data = new RecordData();
            data->roleInfos.clear();
            //memset(data->roleInfos, 0, sizeof( data->roleInfos));
            //memset(data->roomPlayerScore, 0, sizeof( data->roomPlayerScore));
            memset(data->singleRecordData, 0, sizeof( data->singleRecordData));
            //log("重置新记录数据...");
            data->roomNum = record_list->szVirRoomID;
            data->dwGameRounds = record_list->dwGameRounds;
            data->szEndTime = record_list->szEndTime;
            recordIndex++;
            data->ID = recordIndex;
            
            //data->singleRecordData.clear();
            //log("添加用户记录信息...");
            for (int i=0; i<record_list->rec.wUserCount; i++)
            {
            	std::string pName = "";

                RoleInfo roleInfo;
                roleInfo.roomPlayerScore = 0;
                memset(roleInfo.roomPlayerName, 0, sizeof(roleInfo.roomPlayerName));
                roleInfo.dwUserID = record_list->rec.item[i].dwUserID;
            	for (int k = 0; k < 50; k++) {
            		pName+= record_list->rec.item[i].sNickName[k];
                    
                    roleInfo.roomPlayerName[i][k] = record_list->rec.item[i].sNickName[k];
                    
            		//data->roomPlayerName[i][k] = record_list->rec.item[i].sNickName[k];
				}
                
                data->roleInfos.push_back(roleInfo);
                //CCLOG("[%s]房间  %d号玩家的名字是 ： %s" , record_list->szVirRoomID , i , GBKToUtf8(pName.c_str()));
                
            }
            sortRoomNum.push_back(data->roomNum);
            //log("加入Map表...");
            allRecordDatas.insert(std::make_pair(data->roomNum , data));
        }
        
        //RecordData data = allRecordDatas.at(record_list->szVirRoomID);
        
        if (record_list->rec.wUserCount != PLAYER_COUNT) {
            CCLOG("Error get record user count is %d " ,record_list->rec.wUserCount);
        }
        
        allRecordDatas.at(record_list->szVirRoomID)->singleRecordCount++;
        
        SingleRecordData sData;
        for (int i=0; i<record_list->rec.wUserCount; i++)
        {
            //CCLOG("[%s]房间  %d号玩家的 得分 ： %lld" , record_list->szVirRoomID , i , record_list->rec.item[i].i64ChangeMoney);
            //allRecordDatas.at(record_list->szVirRoomID)->singleRecordData.insert(std::make_pair(record_list->dwRoundNo , sData));
            int index = getRoleInVectorIndex(allRecordDatas.at(record_list->szVirRoomID)->roleInfos, record_list->rec.item[i].dwUserID);
            
            //CCLOG("找到玩家的索引为  : %d" , index);
            
            if (index != -1 ) {
                sData.roomPlayerScore[index] = (int)record_list->rec.item[i].i64ChangeMoney;
                
                //int score = allRecordDatas.at(record_list->szVirRoomID)->roleInfos.at(index).roomPlayerScore;
                
                allRecordDatas.at(record_list->szVirRoomID)->roleInfos.at(index).roomPlayerScore += (int)record_list->rec.item[i].i64ChangeMoney;
            }else{
                sData.roomPlayerScore[index] = -1;
                allRecordDatas.at(record_list->szVirRoomID)->roleInfos.at(index).roomPlayerScore = -1;
                CCLOG("Error  没有扎到该玩家...");
            }
            
            //allRecordDatas.at(record_list->szVirRoomID)->roomPlayerScore[i] += (int)record_list->rec.item[i].i64ChangeMoney;
        }
        
        //CCLOG("==============================");
        
        //log("添加子记录... %d" , allRecordDatas.at(record_list->szVirRoomID)->singleRecordCount);
        allRecordDatas.at(record_list->szVirRoomID)->singleRecordData[allRecordDatas.at(record_list->szVirRoomID)->singleRecordCount - 1] = sData;
        
        //allRecordDatas.at(record_list->szVirRoomID)->singleRecordData.push_back(sData);
    }
    else if (bHandleCode==1)
    {
    	//log("recordrankingEvent  end....");
        load_record();
    }
    return true;
}

void RecordLayer::load_singele_record(std::string roomNum){

    if (allRecordDatas.find(roomNum) == allRecordDatas.end()) {
        return;
    }
    
    RecordData * data = allRecordDatas.at(roomNum);
    
    if (data->singleRecordCount == 0) {
        return;
    }
    
    std::vector<SingleRecordData> singleData;
    singleData.clear();
    for (int i = 0; i < data->singleRecordCount; i++) {
        singleData.push_back(data->singleRecordData[i]);
    }
    
    //std::map<int,SingleRecordData> singleData = data->singleRecordData;
    //std::vector<SingleRecordData> singleData = data->singleRecordData;
    curRecordIndex = 1;
    
    listView_record->removeAllItems();
    listView_record->refreshView();
    
    //std::map<int,SingleRecordData>::iterator it;
    //for(it=singleData.begin();it!=singleData.end();++it)
    
    for(int i = 0;i < singleData.size();i++)
    {
        listView_record->pushBackDefaultItem();
        Widget* pModelItem = listView_record->getItem(listView_record->getItems().size() - 1);
        
        //ImageView *recordDi =(ImageView*)Helper::seekWidgetByName(pModelItem, "Image_di");
        //recordDi->addTouchEventListener(CC_CALLBACK_2(RecordLayer::btnClickEventCallback, this));
        SingleRecordData sData = singleData.at(i);
        int gameRound = i + 1;
        //SingleRecordData sData = it->second;
        Text* Label_GameRound = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_GameRound"));
        Label_GameRound->setString(__String::createWithFormat("%d",gameRound)->getCString());
        Label_GameRound->setVisible(true);
        
        Button* btn_replayBtn = static_cast<Button*>(Helper::seekWidgetByName(pModelItem, "ReplayBtn"));
        btn_replayBtn->setVisible(true);
        
        Text* Label_time = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_time"));
        Label_time->setString(__String::createWithFormat("时间 : %s",data->szEndTime.c_str())->getCString());
        Text* Label_roomNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_roomNu"));
        Label_roomNu->setString(data->roomNum);
        Text* Label_playNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_playNu"));
        Label_playNu->setString(__String::createWithFormat("%d局",data->dwGameRounds)->getCString());
        
        for (int  i = 0; i < PLAYER_COUNT ; i++) {
            Text* playerName = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dName",i + 1)->getCString()));
            
            RoleInfo roleInfo = data->roleInfos.at(i);
            
                      //playerName->setString(data->roomPlayerName[i]);
            std::string nickName ="";
            for (int j = 0; j< 51; j++) {
                //nickName += data->roomPlayerName[i][j];
                nickName += roleInfo.roomPlayerName[i][j];
            }
            playerName->setString(GBKToUtf8(nickName.c_str()));
            
            Text* playerRecord = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dRecord",i + 1)->getCString()));
            playerRecord->setString(__String::createWithFormat("%d",sData.roomPlayerScore[i])->getCString());
        }
    }
    
    listView_record->refreshView();
}


void RecordLayer::load_record()
{

	//log("load_record ....");

    curRecordIndex = 0;
    
    listView_record->removeAllItems();
    listView_record->refreshView();

    if (allRecordDatas.size() == 0) {
        noRecordImg->setVisible(true);
        return;
    }
    
    for (std::string roomNum : sortRoomNum) {
        listView_record->pushBackDefaultItem();
        Widget* pModelItem = listView_record->getItem(listView_record->getItems().size() - 1);
        
        ImageView *recordDi =(ImageView*)Helper::seekWidgetByName(pModelItem, "Image_di");
        recordDi->addTouchEventListener(CC_CALLBACK_2(RecordLayer::btnClickEventCallback, this));
        
        RecordData *data = allRecordDatas.at(roomNum);
        
        recordDi->setUserData(data);
        //recordDi->setName(data->roomNum);
        Text* Label_GameRound = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_GameRound"));
        Label_GameRound->setVisible(false);
        
        Button* btn_replayBtn = static_cast<Button*>(Helper::seekWidgetByName(pModelItem, "ReplayBtn"));
        btn_replayBtn->setVisible(false);
        
        Text* Label_time = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_time"));
        Label_time->setString(__String::createWithFormat("时间 : %s",data->szEndTime.c_str())->getCString());
        
        Text* Label_roomNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_roomNu"));
        Label_roomNu->setString(data->roomNum);
        Text* Label_playNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_playNu"));
        Label_playNu->setString(__String::createWithFormat("%d局",data->dwGameRounds)->getCString());
        
        for (int  i = 0; i < data->roleInfos.size() ; i++) {
            Text* playerName = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dName",i + 1)->getCString()));
            //playerName->setString(data->roomPlayerName[i]);
            
            RoleInfo roleInfo = data->roleInfos.at(i);

            std::string nickName ="";
            for (int j = 0; j< 51; j++) {
                nickName += roleInfo.roomPlayerName[i][j];
            }
            //log("nickName : %s" , nickName.c_str());
            playerName->setString(GBKToUtf8(nickName.c_str()));
            
            Text* playerRecord = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dRecord",i + 1)->getCString()));
            playerRecord->setString(__String::createWithFormat("%d",roleInfo.roomPlayerScore)->getCString());
        }

    }
 
    /*
    //std::sort(allRecordDatas.begin()->second, allRecordDatas.end()->second, sortS());
    std::map<std::string,RecordData*>::iterator it;
    for(it=allRecordDatas.begin();it!=allRecordDatas.end();++it)
    {
        listView_record->pushBackDefaultItem();
        Widget* pModelItem = listView_record->getItem(listView_record->getItems().size() - 1);

        ImageView *recordDi =(ImageView*)Helper::seekWidgetByName(pModelItem, "Image_di");
        recordDi->addTouchEventListener(CC_CALLBACK_2(RecordLayer::btnClickEventCallback, this));
        
        RecordData *data = it->second;
        recordDi->setUserData(it->second);
        //recordDi->setName(data->roomNum);
        Text* Label_GameRound = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_GameRound"));
        Label_GameRound->setVisible(false);
        
        Button* btn_replayBtn = static_cast<Button*>(Helper::seekWidgetByName(pModelItem, "ReplayBtn"));
        btn_replayBtn->setVisible(false);
        
        Text* Label_time = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_time"));
        Label_time->setString(__String::createWithFormat("时间 : %s",data->szEndTime.c_str())->getCString());

        Text* Label_roomNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_roomNu"));
        Label_roomNu->setString(data->roomNum);
        Text* Label_playNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_playNu"));
        Label_playNu->setString(__String::createWithFormat("%d局",data->dwGameRounds)->getCString());
        
        for (int  i = 0; i < PLAYER_COUNT ; i++) {
            Text* playerName = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dName",i + 1)->getCString()));
            //playerName->setString(data->roomPlayerName[i]);
            
            std::string nickName ="";
            for (int j = 0; j< 51; j++) {
                 nickName += data->roomPlayerName[i][j];
            }
            //log("nickName : %s" , nickName.c_str());
            playerName->setString(GBKToUtf8(nickName.c_str()));

            Text* playerRecord = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, __String::createWithFormat("player%dRecord",i + 1)->getCString()));
            playerRecord->setString(__String::createWithFormat("%d",data->roomPlayerScore[i])->getCString());
        }
    }
    */
    listView_record->refreshView();
    
    /*
    std::vector<StatusItem_Record>& items = getItems_record();
    std::vector<StatusItem_Record>::iterator itor = items.begin();
    std::vector<StatusItem_Record>::iterator itor2 = items.begin();
    int count=0;
    LLONG totalScore=0;
    for (int i=0; i<m_list_count; i++)
    {
        for (int j=0; j<itor->rec.wUserCount; j++)
        {
            if (itor->rec.item[j].dwUserID==PlatformLogic()->loginResult.dwUserID)
            {
                log("%d",PlatformLogic()->loginResult.dwUserID);
                totalScore=totalScore+itor->rec.item[j].i64ChangePoint;
                break;
            }
        }
        itor++;
    }
    itor = items.begin();

    char buttfer[60];
    auto Label_score = (Text*)Helper::seekWidgetByName(_recordLayout, "Label_score");
    sprintf(buttfer,"%s%lld","总积分:",totalScore);
    Label_score->setString(buttfer);
    while (&items && itor != items.end() && count <10)
    {
        count++;
        listView_record->pushBackDefaultItem();
        Widget* pModelItem = listView_record->getItem(listView_record->getItems().size() - 1);
        pModelItem->setUserData(&(*itor));
        char name[128];
        ImageView* Image_result = static_cast<ImageView*>(Helper::seekWidgetByName(pModelItem, "Image_result"));
        Text* Label_time = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_time"));
        Label_time->setString(itor->szEndTime);
        Text* Label_roomNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_roomNu"));
        sprintf(name, "%s%s","房号:",itor->szVirRoomID);
        Label_roomNu->setString(GBKToUtf8(name));
        Text* Label_playNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_playNu"));
        sprintf(name, "%s%d%s","第",itor->dwRoundNo,"局");
        Label_playNu->setString(GBKToUtf8(name));
        for (int i=0; i<itor->rec.wUserCount; i++)
        {
            if (i >= 4) {
                break;
            }

            if (itor->rec.item[i].dwUserID==PlatformLogic()->loginResult.dwUserID)
            {
                if (itor->rec.item[i].i64ChangePoint>=0)
                {
                    Image_result->loadTexture("platform/openRoom/Common/record_bg/win.png");
                }
                else
                {
                    Image_result->loadTexture("platform/openRoom/Common/record_bg/lose.png");
                }
            }
            char str[60];
            sprintf(name, "Panel_player%d",i+1);
            Layout* Panel_player = static_cast<Layout*>(Helper::seekWidgetByName(pModelItem, name));
            Text* Label_name = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_name"));
            Label_name->setString(GBKToUtf8(itor->rec.item[i].sNickName));
            Text* Label_uid = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_uid"));
            sprintf(str, "%d",itor->rec.item[i].dwUserID);
            Label_uid->setString(str);
            Text* Label_currentScore = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_currentScore"));
            sprintf(str, "%lld",itor->rec.item[i].i64ChangePoint);
            Label_currentScore->setString(str);
            Text* Label_totalScore = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_totalScore"));
            LLONG score=0;
            
            for (int j=0; j<m_list_count; j++)
            {
                if(strcmp(itor->szVirRoomID, itor2->szVirRoomID) == 0)
                {
                    for (int z=0; z<itor->rec.wUserCount; z++)
                    {
                        if (itor2->rec.item[z].dwUserID==itor->rec.item[i].dwUserID)
                        {
                            score+=itor2->rec.item[z].i64ChangePoint;
                        }
                    }

                }
                itor2++;
            }
            itor2= items.begin();
            sprintf(str,"%s%lld","累计:",score);
            Label_totalScore->setString(str);
        }
        itor++;
    }*/

}

void RecordLayer::btnClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType type){
    //CCLOG("点击记录...");
    if (curRecordIndex != 0) {
        return;
    }
    
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn=(ImageView*)pSender;
    RecordData *data = (RecordData*)btn->getUserData();
    load_singele_record(data->roomNum);
}


void RecordLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    YZAudioEngine::getInstance()->playEffect("platform/sound/sound_click.mp3");
    auto btn=(Button*)pSender;
    switch (btn->getTag())
    {
        case RRCORDLAYER_BTNSHARE_TAG:
        {
            auto layer = ShareLayer::createLayer(ShareType::SCREEN);
            addChild(layer, 1000);
        }
            break;
        case RRCORDLAYER_BTNEXIT_TAG:
            
            if (curRecordIndex == 0) {
                removeFromParentAndCleanup(true);
            }else{
                load_record();
            }
            break;
        default:
            break;
    }
}

void RecordLayer::showPrompt()
{
    Node* root = Director::getInstance()->getRunningScene();
    CCAssert(nullptr != root, "root is null");
    
    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
}
