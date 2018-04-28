//
//  WeChatManager.h
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//

#ifndef __AppFunTest__WxSDKManager__
#define __AppFunTest__WxSDKManager__

#include <stdio.h>

class WxSDKManager {
public:
    static WxSDKManager *getInstance();
       
    void shareScreen2Line();    //分享图片到朋友圈
    
    void shareScreen2Friend();  //分享图片到回话
    
    void share2Friend(std::string roomNum , std::string zmCount, std::string gameCount);
    
    void shareGame2Line();
    
    void shareGame2Friend();
    
    void loginWithWx();
};

#endif /* defined(__AppFunTest__WeChatManager__) */
