#ifndef SetLayer_h__
#define SetLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"


USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif


// btn tag
#define SET_SOUND 1001
#define SET_MUSIC 1002

#define SET_BTN_OUTLOGIN		1006
#define SET_BTN_CLOSE	1007

#define SET_MUSIC_SILDER 1008
#define SET_SOUND_SILDER 1009



// image tag
//#define RMBGOLD_1 2001
//#define RMBGOLD_2 2002

// panel tag


// Layer tag
#define SETLAYER_LAYER	5001
//#define SETPW_LAYER		5002
//#define SETEMAIL_LAYER	5003
//#define SETPHONE_LAYER	5004
//#define HELP_LAYER	5005

class SetLayer : public Layer, public EditBoxDelegate
{
private:
    
    ui::Button * _btnMusic;
    ui::Button * _btnSound;

    bool _musicOn;
    bool _soundOn;
    
public:
	static cocos2d::Layer* createLayer();
	virtual bool init();

	void onImageClicked(Ref * pSender,Widget::TouchEventType type);
	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void sliderEvent(Ref* pSender, Slider::EventType type);

	void editBoxReturn(EditBox* editBox);
	void editBoxTextChanged(EditBox* editBox, const std::string& text);

	CREATE_FUNC(SetLayer);
    
private:
    void changeMusicBtnImage();
    
    void changeSoundBtnImage();

};


#endif // SetLayer_h__
