#ifndef ShareLayer_h__
#define ShareLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"

#define BTN_SHARECLOSE_TAG 1001
USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;

enum ShareType{
    SCREEN,         //分享截屏
    INVATE,         //邀请好友
    SHAREGAME,      //分享游戏
};

enum ShareAddr{
    Timeline,           //朋友圈
    SceneSession,       //回话
};

class ShareLayer : public Layer
{
    public:
    static cocos2d::Layer* createLayer(ShareType shareType = ShareType::SCREEN);
    virtual bool init();
    
    void shareShot();
    
    static ShareLayer * _GameShareLayer;
    static ShareLayer* getInstance();
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);

    void shareCallBackIOS(int errorCode);
    
    void screenShotCallback(bool b, const std::string& name);
    
    void ScreenShoot();
    
    CREATE_FUNC(ShareLayer);
    
private:
    
    Widget* uiShareLayer;
    
    void share2Line();

    void share2Friend();

    ShareAddr shareAddr;
    ShareType shareType;
};


#endif // ShareLayer_h__
