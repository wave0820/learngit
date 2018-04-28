#include "SetLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "GamePrompt.h"
#include "LoginLayer.h"
#include <regex>
#include "PlatformDefine.h"
#include "CommonLayer.h"
#include "HelpLayer.h"
#include "Sound/YZAudioEngine.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;

Layer* SetLayer::createLayer()
{
	auto layer = SetLayer::create();
	return layer;
}

bool SetLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}


	Size winSize = Director::getInstance()->getWinSize();
	auto _uiSetLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("SetLayer/SetLayer.json");
	_uiSetLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiSetLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiSetLayer);

    
    //auto setBg = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_uiSetLayer, "Image_bg"));
    
    /*
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [winSize, setBg, _uiSetLayer](Touch* t, Event* e){
        if ((setBg->getBoundingBox().containsPoint(t->getLocation())))
        {
            return true;
        }
        else
        {
            _uiSetLayer->removeFromParentAndCleanup(true);
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, setBg);
    */
    // -- btn
   
    
    _btnMusic= (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_music"));
    _btnMusic->setVisible(true);
    _btnMusic->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnMusic->setTag(SET_MUSIC);
    
    _btnSound = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_sound"));
    _btnSound->setVisible(true);
    _btnSound->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnSound->setTag(SET_SOUND);
    
    _musicOn = YZAudioEngine::getInstance()->getSwitchOfMusic();
    _soundOn = YZAudioEngine::getInstance()->getSwitcjofEffect();
    
    this->changeMusicBtnImage();
    this->changeSoundBtnImage();
    
    auto _btnOutLogin = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_outLogin"));
    _btnOutLogin->setVisible(true);
    _btnOutLogin->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnOutLogin->setTag(SET_BTN_OUTLOGIN);
   	
    auto _btnExit = (Button*)(Helper::seekWidgetByName(_uiSetLayer,"btn_close"));
    _btnExit->setVisible(true);
    _btnExit->addTouchEventListener(CC_CALLBACK_2(SetLayer::onBtnClicked,this));
    _btnExit->setTag(SET_BTN_CLOSE);
    
    ui::Slider * musicSlider = (Slider*)(Helper::seekWidgetByName(_uiSetLayer,"musicSlider"));
    musicSlider->setTag(SET_MUSIC_SILDER);
    musicSlider->addEventListener(CC_CALLBACK_2(SetLayer::sliderEvent, this));
    musicSlider->setPercent(YZAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);
    
    ui::Slider * soundSlider = (Slider*)(Helper::seekWidgetByName(_uiSetLayer,"soundSlider"));
    soundSlider->setTag(SET_SOUND_SILDER);
    soundSlider->addEventListener(CC_CALLBACK_2(SetLayer::sliderEvent, this));
    soundSlider->setPercent(YZAudioEngine::getInstance()->getEffectsVolume() * 100);

	return true;
}

void SetLayer::changeMusicBtnImage(){
    if (_musicOn) {
        _btnMusic->loadTextureNormal("SetLayer/set_res/musicOn.png");
    }else{
        _btnMusic->loadTextureNormal("SetLayer/set_res/musicOff.png");
    }

}

void SetLayer::changeSoundBtnImage(){
    if (_soundOn) {
        _btnSound->loadTextureNormal("SetLayer/set_res/soundOn.png");
    }else{
        _btnSound->loadTextureNormal("SetLayer/set_res/soundOff.png");
    }
}


void SetLayer::onImageClicked(Ref * pSender,Widget::TouchEventType type)
{
}

void SetLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
		switch (btn->getTag())
		{
		case SET_MUSIC:
			{
                _musicOn = !_musicOn;
                this->changeMusicBtnImage();
                YZAudioEngine::getInstance()->setSwitchOfMusic(_musicOn);
			}
			break;
        case SET_SOUND:
            {
                _soundOn = !_soundOn;
                this->changeSoundBtnImage();
                YZAudioEngine::getInstance()->setSwitcjofEffect(_soundOn);
            }
            break;
        case SET_BTN_OUTLOGIN:
            {
                RoomLogic()->closeRoom();
                PlatformLogic()->closePlatform();
                SaveIntegerToXML(SAVE_USERID, 0);
                auto scene =LoginLayer::createScene();
                Director::getInstance()->replaceScene(scene);
            }
            break;
        case SET_BTN_CLOSE:
            {
                removeFromParentAndCleanup(true);
            }
            break;
      
            default:
			break;
		}
	}
}

void SetLayer::sliderEvent(Ref *pSender, Slider::EventType type)
{
	switch (type)
	{
	case Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
			Slider* slider = static_cast<Slider*>(pSender);
			switch (slider->getTag())
			{
			case SET_MUSIC_SILDER:
				{
                    auto mvol  = slider->getPercent()/100.0;
                    YZAudioEngine::getInstance()->setBackgroundMusicVolume(mvol);
				}
				break;
                    
			case SET_SOUND_SILDER:
				{
					auto mvol  = slider->getPercent()/100.0;
                    YZAudioEngine::getInstance()->setEffectsVolume(mvol);
				}
				break;
			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}



// --mail
void SetLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{

}

void SetLayer::editBoxReturn(EditBox* editBox)
{
}

