#ifndef StoreLayer_h__
#define StoreLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "PlatformDefine.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;

//遇乐道具购买
struct MSG_YL_TAG_PROP_BUY
{
	int iUserID;
	int iPropID;
	int iPropCnt;

	MSG_YL_TAG_PROP_BUY()
	{
		memset(this,0,sizeof(MSG_YL_TAG_PROP_BUY));
	}
};


typedef struct
{
	long		dwUserID;
	int 		nPropID;
	int			nHoldCount;
	int			iProce;
}MSG_YL_PROP_S_GETPROP;

/////////////////////////////////
struct MSG_YL_TAG_GIFT_HOCK
{
	int dwUserID;
	int iGiftID;
	int iGiftCount;

	MSG_YL_TAG_GIFT_HOCK()
	{
		memset(this,0,sizeof(MSG_YL_TAG_PROP_BUY));
	}
};
typedef struct
{
	int		dwUserID;
	int		nGiftID;
	int		nHoldCount;
	int		iProce;
}MSG_YL_GIFT_S_GETPROP;


// btn tag
#define STORE_BTN_RMB5              5
#define STORE_BTN_RMB48             48


#define STORE_BTN_TYPE1		        1001
#define STORE_BTN_TYPE2			    1002
#define STORE_BTN_TYPE3		        1003
#define STORE_BTN_EXIT				1004

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#define  LOG_TAG    "StoreLayer"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#include "jubaosdk/ShellApiJni.h"
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class StoreLayer : public cocos2d::Layer, public ShellApiJni::Callback ,public IPlatformNotifyDelegate
#else
class StoreLayer : public cocos2d::Layer,public IPlatformNotifyDelegate
#endif
{
private:
	Widget* _uiStoreLayer;
	Widget* _uipromptDepositLayer;
	Widget* _uipropptDrawLayer;

	Layout* panel_type1;
	Layout* panel_type2;
	Layout* panel_type3;

	Button* btn_type1;
	Button* btn_type2;
	Button* btn_type3;

	bool iBool;


public:
	static StoreLayer* createLayer(MoneyChangeNotify* delegate , int propId);
    
    static StoreLayer * _StoreLayer;
    static StoreLayer* getInstance();
    
    int     m_iBuyCardCount;
    int     m_iPropId;
    
    void getPayInfoByIOS(std::string errorCode, std::string errorStr);
public:

    bool storeSendPayInforSelector( YZSocketMessage* socketMessage );
    bool storePaySelector( YZSocketMessage* socketMessage );
    void payMoney(int mount,std::string payID,int num);
    void sendPayInfor(std::string playerID,std::string payID,int mount);

    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    virtual void call(int code, std::string message) override;
#endif
public:
	//static cocos2d::Layer* createLayer();
	virtual bool init();
	void refreshGold(int change);
	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void onFcBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void openStore(Node* parent, Vec2 vec, int zorder, int tag);
	void setChangeDelegate(MoneyChangeNotify* delegate);
private:
	// 商店物品处理函数;
	bool storeEventSelector(YZSocketMessage* socketMessage);

	bool isDigitStr(const char* cstr);//判断是否是纯数字
private:
	MoneyChangeNotify*				_delegate;
};




#endif // StoreLayer_h__
