#include "DeclarationLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

Layer* DeclarationLayer::createLayer()
{
	auto layer = DeclarationLayer::create();
	return layer;
}

bool DeclarationLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	auto uiDeclarationLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("declaration/Declaration.json");
	uiDeclarationLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiDeclarationLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiDeclarationLayer);

	Button* btn_close = (Button*)(Helper::seekWidgetByName(uiDeclarationLayer,"btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(DeclarationLayer::onBtnClicked,this));
	btn_close->setTag(BTN_HELPCLOSE);


	return true;
}

void DeclarationLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		switch (btn->getTag())
		{
		case BTN_HELPCLOSE:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		default:
			break;
		}
	}
}





