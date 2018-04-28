//
//  WeChatManager.h
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//

#ifndef __AppFunTest__WeChatManager__
#define __AppFunTest__WeChatManager__

#include <stdio.h>

class WxPayManager {
public:
    static WxPayManager *getInstance();
    
    void request2WXPay();
 };

#endif /* defined(__AppFunTest__WeChatManager__) */
