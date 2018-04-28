#include "LoadingLayer2.h"
#include "Base/YZLayerColor.h"

namespace YZ 
{
	static const int LOADING_LAYER_TAG		= 99999999;
	static const int LOADING_LAYER_ZORDER	= 99999999;

	LoadingLayer2* LoadingLayer2::createLoading(Node* parent, const std::string& text, const std::string& image)
	{
		auto loading = (LoadingLayer2*)parent->getChildByTag(LOADING_LAYER_TAG);;
		if (nullptr == loading)
		{
			loading = LoadingLayer2::create();
			loading->setPosition(Size::ZERO);
			loading->createDialog(text, image);
			parent->addChild(loading, LOADING_LAYER_ZORDER, LOADING_LAYER_TAG);
		}
		//loading->setText(text);
		return loading;
	}

	void LoadingLayer2::removeLoading(Node* parent)
	{
		CCAssert(nullptr != parent, "parent is null");
		parent->removeChildByTag(LOADING_LAYER_TAG);
	}

	//////////////////////////////////////////////////////////////////////////

	LoadingLayer2::LoadingLayer2()
	{

	}

	LoadingLayer2::~LoadingLayer2()
	{

	}

	void LoadingLayer2::createDialog(const std::string& text, const std::string& image)
	{
		// 转圈动画
        
        auto winsize=Director::getInstance()->getWinSize();
        
        auto welcome_bg = Sprite::create("Loading/MjLoadingBg.png");
        welcome_bg->setPosition(winsize/2);
        this->addChild(welcome_bg);
        
        Sprite* sp = Sprite::create("Loading/MjLoading1.png");
        sp->setPosition(Vec2(welcome_bg->getContentSize().width/2 , welcome_bg -> getContentSize().height * .65f));
        welcome_bg->addChild(sp , 1);
        
        Label * label = Label::createWithTTF(text, "fonts/DroidSansFallback.ttf", 25);
        label->setColor(Color3B::BLACK);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(welcome_bg->getContentSize().width/2 , welcome_bg -> getContentSize().height * .25f));
        welcome_bg->addChild(label);
        
        Animation* pAnimation = Animation::create();
        for (int i = 1; i <= 8; i++)
        {
            char fileName[60];
            sprintf(fileName, "Loading/MjLoading%d.png", i);
            pAnimation->addSpriteFrameWithFile(fileName);
        }
        pAnimation->setDelayPerUnit(1.5f / 8);
        pAnimation->setRestoreOriginalFrame(true);
        Animate* pAnimate = Animate::create(pAnimation);
        sp->runAction(RepeatForever::create(pAnimate));
      
        //quicklyShade(50);
    }

	void LoadingLayer2::autoSetOpacity(float dt)
	{
		_opacity = _opacity + 3;
		Layer* colorLayer = (Layer*)this->getChildByTag(10);
		colorLayer->setOpacity(_opacity);
		if (_opacity >= 200) unschedule(schedule_selector(LoadingLayer2::autoSetOpacity));
	}

	void LoadingLayer2::setText(const std::string& text)
	{
		_label->setString(text);
	}

	bool LoadingLayer2::init()
	{
		if (!YZLayer::init())
		{
			return false;
		}
		_opacity = 0;
		// 监听点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		// 阻止触摸向下传递
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());      
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = Director::getInstance()->getWinSize();
			Rect rect = Rect(0, 0, s.width, s.height); 
			return rect.containsPoint(locationInNode);
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		return true;
	}
}
