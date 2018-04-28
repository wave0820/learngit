#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;


class Welcome : public cocos2d::Layer
{


public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	

    // implement the "static create()" method manually
    CREATE_FUNC(Welcome);

private: 
	void actionEnd();
};



#endif // __WELCOME_SCENE_H__
