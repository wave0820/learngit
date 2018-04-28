#include "StoreLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "YZNetExport.h"
#include "HallLayer.h"
#include "GamePrompt.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "WxSDK/WxSDKManager.h"
#endif

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

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;


StoreLayer * StoreLayer::_StoreLayer = nullptr;
StoreLayer*  StoreLayer::getInstance()
{
    if (!_StoreLayer)
    {
        _StoreLayer = new StoreLayer();
    }
    return _StoreLayer;
}

StoreLayer* StoreLayer::createLayer(MoneyChangeNotify* delegate , int propId)
{
	StoreLayer *pRet = new StoreLayer();
    pRet->m_iPropId = propId;
	if (pRet && pRet->init()) 
	{ 
		pRet->setChangeDelegate(delegate);
		pRet->autorelease(); 
		return pRet; 
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool StoreLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
    _StoreLayer = this;
    
    iBool=true;

	Size winSize = Director::getInstance()->getWinSize();
	_uiStoreLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("storeLayer/ShopLayer.json");
	_uiStoreLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiStoreLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiStoreLayer);

	auto label_wx1 = (Text*)(Helper::seekWidgetByName(_uiStoreLayer,"label_wx1"));
	label_wx1->setString("请联系群主购买");

	auto label_wx2 = (Text*)(Helper::seekWidgetByName(_uiStoreLayer,"label_wx2"));
	label_wx2->setString("huanteng01   【个人微信号】");

	auto label_wx3 = (Text*)(Helper::seekWidgetByName(_uiStoreLayer,"label_wx3"));
	label_wx3->setString("欢腾休闲手游 【微信公众号】");

    Button * btn_ok = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,"btn_ok"));
    btn_ok->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onFcBtnClicked,this));
    btn_ok->setTag(STORE_BTN_EXIT);
    
	Button * btn_close = (Button*)(Helper::seekWidgetByName(_uiStoreLayer,"btn_close"));
	btn_close->setPressedActionEnabled(true);
	btn_close->addTouchEventListener(CC_CALLBACK_2(StoreLayer::onFcBtnClicked,this));
	btn_close->setTag(STORE_BTN_EXIT);

	return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void StoreLayer::call(int code, std::string message) {
    LOGD("=====> call : %d, %s", code, message.c_str());
    std::ostringstream oss;
    oss << "return value: " << code;
    auto lab = this->getChildByTag(1);
    ((Label*)lab)->setString(oss.str());
}
#endif

void StoreLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
        
        if (btn->getTag() == STORE_BTN_RMB5) {
            m_iBuyCardCount =1;
        }else if(btn->getTag() == STORE_BTN_RMB48){
            m_iBuyCardCount =10;
        }
        
        CCLOG("PropId : %d  --  BuyCount : %d " , m_iPropId , m_iBuyCardCount);
        
        int price = btn->getTag();
        

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","doPay", "(I)V");
        
        if (isHave)
        {
            jint totalPrice = (jint)price;
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , totalPrice);
        }
#endif

        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      
#endif

    }
}
void StoreLayer::onFcBtnClicked( Ref * pSender,Widget::TouchEventType type )
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		switch (btn->getTag())
		{
		case STORE_BTN_EXIT:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		default:
			break;
		}
	}
}


void StoreLayer::openStore(Node* parent, Vec2 vec, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");

	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	/*parent->runAction(EaseSineOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))));*/
	parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void StoreLayer::setChangeDelegate(MoneyChangeNotify* delegate)
{
	_delegate = delegate;
}

// refresh gold
void StoreLayer::refreshGold(int change)
{

}


bool StoreLayer::isDigitStr( const char* cstr )
{
	if (NULL == cstr || cstr[0] == 0)  
	{  
		return false;  
	}  

	int len = strlen(cstr);  
	int pos = 0;  
	if (cstr[0] == '-' || cstr[0] == '+')  
	{  
		if (len <= 1)   
		{  
			return false;  
		}  

		pos++;          
	}  

	while (pos < len)  
	{  
		if (cstr[pos] < '0' || cstr[pos] > '9')  
		{  
			return false;  
		}  
		pos++;  
	}  

	return true;  
}

bool StoreLayer::storeEventSelector( YZSocketMessage* socketMessage )
{
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	 
	return true;
}

void StoreLayer::getPayInfoByIOS(std::string errorCode, std::string errorStr){
    CCLOG("GetPayInfo -- errorCode : %s  errorStr : %s " , errorCode.c_str(), errorStr.c_str());
    if (std::atoi(errorCode.c_str()) == 0) {
        MSG_PROP_C_BUY_ROOMCARD data;
        memset(&data, 0, sizeof(data));
        data.dwUserID = PlatformLogic()->loginResult.dwUserID;
        data.nPropID =StoreLayer::getInstance()->m_iPropId;
        data.nPropCount = StoreLayer::getInstance()->m_iBuyCardCount;
        
        YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_BUY, &data, sizeof(data));
    }
}

void getPayInfo(std::string errorCode, std::string errorStr)
{
    CCLOG("GetPayInfo -- errorCode : %s  errorStr : %s " , errorCode.c_str(), errorStr.c_str());
  
    if (std::atoi(errorCode.c_str()) == 0) {
        MSG_PROP_C_BUY_ROOMCARD data;
        memset(&data, 0, sizeof(data));
        data.dwUserID = PlatformLogic()->loginResult.dwUserID;
        data.nPropID =StoreLayer::getInstance()->m_iPropId;
        data.nPropCount = StoreLayer::getInstance()->m_iBuyCardCount;
        
        YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_BUY, &data, sizeof(data));
    }
}

void StoreLayer::payMoney(int mount,std::string payID,int num)
{
	log("-------------支付金额%d",mount);
/*
	log("--------666-+%d",num);
	    TMSG_GP_BankNetDeposit bankNetDeposit;
	        bankNetDeposit._user_id=PlatformLogic()->loginResult.dwUserID;
	        bankNetDeposit._money=mount;
	        bankNetDeposit._pay_type=0;
	        memset(bankNetDeposit._order_id, 0, sizeof(bankNetDeposit._order_id));
	        strcpy(bankNetDeposit._order_id,payID.c_str());
	        bankNetDeposit._purchase_type=1;
	        bankNetDeposit._room_id=0;
	        bankNetDeposit._rsp_code=num;
	        log("---------+%d",bankNetDeposit._rsp_code);
	        memset(bankNetDeposit._type_info, 0, sizeof(bankNetDeposit._type_info));
	        PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_NET_DEPOSIT, &bankNetDeposit, sizeof(bankNetDeposit), YZ_SOCKET_CALLBACK(StoreLayer::storePaySelector,this));
*/
}

void StoreLayer::sendPayInfor(std::string playerID,std::string payID,int mount)
{
    /*
	log("-------------%d",mount);
	TMSG_GP_BankNetDeposit_Pre bankNetDeposit_Pre;
	    bankNetDeposit_Pre._user_id=PlatformLogic()->loginResult.dwUserID;
	    bankNetDeposit_Pre._money=mount;
	    memset(bankNetDeposit_Pre._order_id, 0, sizeof(bankNetDeposit_Pre._order_id));
	    strcpy(bankNetDeposit_Pre._order_id,payID.c_str());
	    bankNetDeposit_Pre._pay_type=2;
	    memset(bankNetDeposit_Pre._phone, 0, sizeof(bankNetDeposit_Pre._phone));
	    memset(bankNetDeposit_Pre._type_info, 0, sizeof(bankNetDeposit_Pre._type_info));
	    strcpy(bankNetDeposit_Pre._type_info,GBKToUtf8("Й█▒днк│Сох"));
	    bankNetDeposit_Pre._purchase_type=1;
	    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_NET_PRE_DEPOSIT ,&bankNetDeposit_Pre, sizeof(bankNetDeposit_Pre), YZ_SOCKET_CALLBACK(StoreLayer::storeSendPayInforSelector,this));

*/
}

bool StoreLayer::storeSendPayInforSelector( YZSocketMessage* socketMessage )
{
    /*
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    if (HC_GP_BANK_NET_PRE_DEPOSIT_SUC  == bHandleCode)
    {
        //payMoney(5,"111111111111111111",0);
    }*/
    return true;
}





bool StoreLayer::storePaySelector( YZSocketMessage* socketMessage )
{
    /*
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    log("-------------111111111111111112");
    CCAssert(sizeof (TMSG_GP_BankNetDeposit) == socketMessage->objectSize, "TMSG_GP_BankNetDeposit size is error.");
    if (HC_GP_BANK_NET_DEPOSIT_SUC  == bHandleCode)
    {
        TMSG_GP_BankNetDeposit* bankNetDeposit = (TMSG_GP_BankNetDeposit*)socketMessage->object;
        if (bankNetDeposit->_rsp_code==0)
        {
        	log("-------------111111111111111113");
            MSG_PROP_C_BUY_ROOMCARD data;
            memset(&data, 0, sizeof(data));
            data.dwUserID = PlatformLogic()->loginResult.dwUserID;
            data.nPropID =StoreLayer::getInstance()->m_iPropId;
            data.nPropCount = StoreLayer::getInstance()->m_iBuyCardCount;

            YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_BUY, &data, sizeof(data));
        }
    }*/
    return true;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_com_mj958_chwh_wxapi_WXPayEntryActivity_buyRoomCardCallBack(JNIEnv *env, jobject thiz, jstring _errorCode,jstring _errorStr) {
        const char* errorCode = env->GetStringUTFChars(_errorCode, nullptr);
        const char* errorStr = env->GetStringUTFChars(_errorStr, nullptr);
        getPayInfo(errorCode, errorStr);
    }

    JNIEXPORT void JNICALL Java_com_dada_AppActivity_payResult(JNIEnv *env, jobject thiz, jint p_id,jstring payID,jint num)
    {
    	log("3333333333333333333333");
    	  const char *orderStr=env->GetStringUTFChars(payID,NULL);

    	  StoreLayer::getInstance()->payMoney(p_id,orderStr,num);

    }

    JNIEXPORT void JNICALL Java_com_dada_AppActivity_sendPayInfor(JNIEnv *env, jobject thiz, jstring playerID,jstring payID,jint mount)
    {
    	log("2222222222222222");
    	const char *nameStr=env->GetStringUTFChars(playerID,NULL);
    	const char *orderStr=env->GetStringUTFChars(payID,NULL);
    	StoreLayer::getInstance()->sendPayInfor(nameStr,orderStr,mount);
    }
}
#endif
