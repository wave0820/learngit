#include "ShareLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"
#include "HallLayer.h"
#include "Util/JzLog.h"
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


#define BTN_SHARECLOSE 10001
#define BTN_SHAREFRIEND  10002
#define BTN_SHARELINE  10003

#define TAG_SHARE_IS_OPEN 1


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;
ShareLayer*ShareLayer::_GameShareLayer = nullptr;

Layer* ShareLayer::createLayer(ShareType shareType)
{
    auto layer = ShareLayer::create();
    layer->shareType = shareType;
    return layer;
}

bool ShareLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    uiShareLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("shareLayer/ShareLayer.json");
    uiShareLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    uiShareLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(uiShareLayer);
    
    
    Button* btn_close = (Button*)(Helper::seekWidgetByName(uiShareLayer,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(ShareLayer::onBtnClicked,this));
    btn_close->setTag(BTN_SHARECLOSE);
    
    
    Button* btn_share2Frind = (Button*)(Helper::seekWidgetByName(uiShareLayer,"share2FriendBtn"));
    btn_share2Frind->addTouchEventListener(CC_CALLBACK_2(ShareLayer::onBtnClicked,this));
    btn_share2Frind->setTag(BTN_SHAREFRIEND);
    
    Button* btn_share2Line = (Button*)(Helper::seekWidgetByName(uiShareLayer,"share2LineBtn"));
    btn_share2Line->addTouchEventListener(CC_CALLBACK_2(ShareLayer::onBtnClicked,this));
    btn_share2Line->setTag(BTN_SHARELINE);
    
    return true;
}

void ShareLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        
        switch (btn->getTag())
        {
            case BTN_SHARECLOSE:
            {
                removeFromParentAndCleanup(true);
            }
                break;
                
            case BTN_SHAREFRIEND:
            {
                if (TAG_SHARE_IS_OPEN) {
                    //分享给朋友
                    this->share2Friend();
                }else{
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("开启微信分享功能即可正常使用..."));
                }
            }
                break;
                
            case BTN_SHARELINE:
            {
                if (TAG_SHARE_IS_OPEN) {
                    this->share2Line();
                }else{
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("开启微信分享功能即可正常使用..."));
                }
            }
                break;
            default:
                break;
        }
    }
}

void ShareLayer::share2Friend(){
    
    shareAddr = SceneSession;
    
    //分享截图给朋友
    if (shareType == ShareType::SCREEN) {
        shareShot();
    }
    
    //分享邀请信息给朋友
    if (shareType == ShareType::SHAREGAME) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;
        bool isHave = false;
        isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareGame2Friend", "()V");
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }else{
            JzDebug("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
        }
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        WxSDKManager::getInstance()->shareGame2Friend();
#endif
    }
    
}


void ShareLayer::share2Line(){
    
    shareAddr = Timeline;

    //分享截图到朋友圈
    if (shareType == ShareType::SCREEN) {
        shareShot();
    }
    
    //分享邀请信息到朋友圈
    if (shareType == ShareType::SHAREGAME) {
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;
        bool isHave = false;
        isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareGame2Line", "()V");
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }else{
            JzDebug("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
        }
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        WxSDKManager::getInstance()->shareGame2Line();
#endif
    }
    
}


ShareLayer*  ShareLayer::getInstance()
{
    if (!_GameShareLayer)
    {
        _GameShareLayer = new ShareLayer();
    }
    return _GameShareLayer;
}

void ShareLayer::shareCallBackIOS(int errorCode){
    removeFromParentAndCleanup(true);
}


void ShareLayer::shareShot()
{
    uiShareLayer->setVisible(false);
    //ScreenShoot();
    //this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]{
    //    screenShotCallback(true , "share");
    //})));
    
    std::string  fileName = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    fileName = FileUtils::getInstance()->getWritablePath() + "share.jpg";
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    fileName = "/mnt/sdcard/share.png";
#endif
    utils::captureScreen(CC_CALLBACK_2(ShareLayer::screenShotCallback, this), fileName);
}

//分享截图
void ShareLayer::screenShotCallback(bool b, const std::string& name){
    //截屏成功， 调用微信分享
    CCLOG("success %s, file name:%s", b?"true":"false", name.c_str());
    
    std::string  fileName = name;//FileUtils::getInstance()->getWritablePath() + "share.jpg";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if (shareAddr == ShareAddr::SceneSession) {
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareImg2Friend", "(Ljava/lang/String;)V");
        
        if (isHave)
        {
            jstring aFilePath = minfo.env->NewStringUTF(fileName.c_str());
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID ,aFilePath);
            minfo.env->DeleteLocalRef(aFilePath);
        }
    }
    
    if (shareAddr == ShareAddr::Timeline) {
        JniMethodInfo minfo;
        
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareImg2Timeline", "(Ljava/lang/String;)V");
        
        if (isHave)
        {
            jstring aFilePath = minfo.env->NewStringUTF(fileName.c_str());
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID ,aFilePath);
            minfo.env->DeleteLocalRef(aFilePath);
        }
    }
    
   
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    if (shareAddr == ShareAddr::SceneSession) {
        WxSDKManager::getInstance()->shareScreen2Friend();
    }

    if (shareAddr == ShareAddr::Timeline) {
        WxSDKManager::getInstance()->shareScreen2Line();
    }
#endif
    
}

