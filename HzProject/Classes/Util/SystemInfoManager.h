//
//  WeChatManager.h
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//

#ifndef __AppFunTest__SystemInfoManager__
#define __AppFunTest__SystemInfoManager__

#include <stdio.h>

typedef enum
{
    // Apple NetworkStatus Compatible Names.
    NoNetWork    = 0,
    ConnWiFi     = 1,
    Conn3G       = 3
} NetState;


class SystemInfoManager {
    
public:
    
    static SystemInfoManager *getInstance();
    
    // 获取电池电量，范围0到1.0。－1表示电量未知。
    float getBatteryLeve();
    
    // 检测WIFI是否可用
    //bool isWIFIEnabled();
    
    
    // 检测3G网络是否可用
    //bool is3GEnabled();
    
    //取得网络连接状况
    NetState getNetState();

};

#endif /* defined(__AppFunTest__WeChatManager__) */
