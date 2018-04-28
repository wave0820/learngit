#include "GamePrompt2.h"
#include <string>
#include "FontConfig.h"
#include "LoginLayer.h"
//#include "SDK1.6.2/CDShareInstance.h"
static const char* PROMPT_JSON		= "PromptBoxUI/PromptLayer2.json";

static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// ◊”Ω⁄µ„µØ≥ˆøÚ≤„º∂

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// ◊”Ω⁄µ„µØ≥ˆøÚ≤„º∂

static Color3B TEXT_COLOR = Color3B(251, 251, 72);

GamePromptLayer2::GamePromptLayer2() 
	: _layout(nullptr)
	, _Label_Prompt(nullptr)
	, _Button_OK(nullptr)
	, _sure(nullptr)
{

}

GamePromptLayer2::~GamePromptLayer2()
{

}

void GamePromptLayer2::showPrompt(const std::string& prompt)
{
	Node* root = Director::getInstance()->getRunningScene();
	CCAssert(nullptr != root, "root is null");

	root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
	setPrompt(prompt);
}

void GamePromptLayer2::closePrompt()
{
	//HSAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	_layout->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));

	if (nullptr != _sure)
	{
		_sure();
	}
}

void GamePromptLayer2::setCallBack(std::function<void ()> sure)
{
	_sure = sure;
}

bool GamePromptLayer2::init()
{
    if ( !YZLayer::init()) return false;

	auto MyListener = EventListenerTouchOneByOne::create();
	MyListener->setSwallowTouches(true);//◊Ë÷π¥•√˛œÚœ¬¥´µ›
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<GamePromptLayer2*>(event->getCurrentTarget());
		CCPoint locationInNode = target->convertToNodeSpace(touch->getLocation());
		CCSize s = target->getContentSize();
		CCRect rect = CCRect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			if (_userRect.containsPoint(locationInNode)) return true;
			closePrompt();
			return true;
		}
		else
		{
			return false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

	CCSize winSize = Director::getInstance()->getWinSize();

	_layout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(PROMPT_JSON));
	_layout->setAnchorPoint(Vec2(0.5f, 0.5f));
	_layout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	addChild(_layout, 2, 3);

    auto Label_Prompt = (Text*)Helper::seekWidgetByName(_layout, "Label_Prompt");
    Label_Prompt->setContentSize(Size(450,60));
    Label_Prompt->setString(GBKToUtf8("低于最小版本必需升级，点击确定即可跳转到下载地址"));
	_Label_Prompt = (Text*)Helper::seekWidgetByName(_layout, "Label_url");
	_Label_Prompt->setColor(Color3B::BLUE);
	_Label_Prompt->setVisible(false);

//	auto btnOpen = (Button*)Helper::seekWidgetByName(_layout, "btn_open");
//	btnOpen->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer2::openUrlClicked, this));
	
	_Button_OK = (Button*)Helper::seekWidgetByName(_layout, "btn_ok");
	_Button_OK->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer2::openUrlClicked, this));

	_layout->setScale(0.3f);

	float xScale = winSize.width / Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width;
	float yScale = winSize.height / Director::getInstance()->getOpenGLView()->getDesignResolutionSize().height;
	_layout->runAction(Sequence::create(ScaleTo::create(0.1f, MIN(xScale, yScale)), nullptr));

	return true;
}

void GamePromptLayer2::setPrompt(const std::string& prompt)
{
	if (nullptr != _Label_Prompt)
	{
		_Label_Prompt->setString(prompt);
	}
}

void GamePromptLayer2::setPrompt(const std::string& prompt, const std::string& font, int size)
{
	if (nullptr != _Label_Prompt)
	{
		_Label_Prompt->setFontName(font);
		_Label_Prompt->setFontSize(0);
		_Label_Prompt->setString(prompt);
	}
}

void GamePromptLayer2::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;

    std::string myUrl=_Label_Prompt->getString();
    Application::sharedApplication()->openURL(myUrl);
	closePrompt();
}

void GamePromptLayer2::openUrlClicked(Ref * pSender, Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if (type == Widget::TouchEventType::ENDED && btn)
	{

        closePrompt();
        //CDShareInstance::GetInstance()->openURL();
        //std::string myUrl="www.baidu.com";
		std::string myUrl=_Label_Prompt->getString();
		//Application::getInstance()->openURL(myUrl);
        Application::sharedApplication()->openURL(myUrl);
	}
}


