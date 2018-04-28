//
//  WeChatManager.cpp
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//
#include "SystemInfoManager.h"
#import "Reachability.h"



@interface SystemInfoObj : UIResponder < UIApplicationDelegate>
{
}

- (float)getBatteryLeve;

- (NetState) getNetState;

@end

NSString* testPage = @"www.baidu.com";

@implementation SystemInfoObj

+ (SystemInfoObj *)sharedInterstitialObj {
    static SystemInfoObj *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
    });
    
    return instance;
}

- (float) getBatteryLeve
{
    [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    return [[UIDevice currentDevice] batteryLevel];
}

-(NetState)getNetState
{
    Reachability *r = [Reachability reachabilityWithHostname:testPage];
    switch ([r currentReachabilityStatus]) {
        case NotReachable:
            // 没有网络连接
            return NoNetWork;
            break;
        case ReachableViaWWAN:
            // 使用3G网络
            return Conn3G;
            break;
        case ReachableViaWiFi:
            // 使用WiFi网络
            return ConnWiFi;
            break;
    }

}


@end

//-----------------------------------------------------

SystemInfoManager * SystemInfoManager ::getInstance(){
    static SystemInfoManager * wechat=NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        wechat= new SystemInfoManager;
    });
    return wechat;
}

float SystemInfoManager::getBatteryLeve(){
       return [[SystemInfoObj sharedInterstitialObj] getBatteryLeve];

}

NetState SystemInfoManager::getNetState(){
    return [[SystemInfoObj sharedInterstitialObj]getNetState];
}
