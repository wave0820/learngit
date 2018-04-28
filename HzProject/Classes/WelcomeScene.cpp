#include "WelcomeScene.h"
#include "LoginLayer.h"
#include "UI/LoadingLayer2.h"
USING_NS_CC;

Scene* Welcome::createScene()
{
    auto scene = Scene::create();
    auto layer = Welcome::create();
    scene->addChild(layer);
    return scene;
}


bool Welcome::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    auto winsize=Director::getInstance()->getVisibleSize();
    auto welcome_bg = Sprite::create("Welcome/welcomBg.png");
    welcome_bg->setPosition(winsize/2);
    float _xScale = winsize.width / welcome_bg->getContentSize().width;
    float _yScale = winsize.height/ welcome_bg->getContentSize().height;
    welcome_bg->setScaleX(_xScale);
    welcome_bg->setScaleY(_yScale);
    addChild(welcome_bg);
    
    auto action_end = CallFunc::create( CC_CALLBACK_0(Welcome::actionEnd,this));
    welcome_bg->runAction(Sequence::create(DelayTime::create(1.0f),action_end,NULL));
   
    return true;
    
}


void Welcome::actionEnd()
{
    //g_GlobalUnits.serverip = "192.168.199.250";
    //g_GlobalUnits.isOpenRoom = true;
    //g_GlobalUnits.serverip = "211.149.160.248";
    //g_GlobalUnits.serverip = "2001:470:18:803::2";
    //g_GlobalUnits.serverip = "43.239.157.76";
    auto scene = TransitionFade::create(1.0f,LoginLayer::createScene());
    Director::getInstance()->replaceScene(scene);
}
