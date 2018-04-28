//
//  SystemInfo.h
//  zhe
//
//  Created by Fsy on 13-5-14.
//  Copyright (c) 2013年 Fsy. All rights reserved.
//

#import "Foundation/Foundation.h"
#import "UIKit/UIKit.h"

typedef enum
{
	// Apple NetworkStatus Compatible Names.
	NoNetWork    = 0,
	ConnWiFi     = 2,
	Conn3G       = 1
} NetState;


// 获取电池电量，范围0到1.0。－1表示电量未知。
float getBatteryLeve();

// 检测WIFI是否可用
bool isWIFIEnabled();


// 检测3G网络是否可用
bool is3GEnabled();

//取得网络连接状况
NetState getNetState();

