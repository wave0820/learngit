//
//  WeChatManager.cpp
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//
#include "WxPayManager.h"


@interface WeChatObj : UIResponder < UIApplicationDelegate>
{
}

@end

std::string sfileName;

@implementation WeChatObj

+ (WeChatObj *)sharedInterstitialObj {
    static WeChatObj *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
    });
    
    return instance;
}

@end

//-----------------------------------------------------



WxPayManager * WxPayManager ::getInstance(){
    static WxPayManager * wechat=NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        wechat= new WxPayManager;
    });
    return wechat;
}

void WxPayManager::request2WXPay(){
    [[WeChatObj sharedInterstitialObj]stopRecorder];
}
