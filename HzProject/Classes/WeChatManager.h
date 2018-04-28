//
//  WeChatManager.h
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//

#ifndef __AppFunTest__WeChatManager__
#define __AppFunTest__WeChatManager__

class WeChartManager {
public:
    static WeChartManager *sharedInterstitial();
       
    void startRecorder(std::string filePath , std::string sfileName);
    
    void stopRecorder();
    
    void playRecorder(std::string filePath , std::string sfileName);
    
    bool canRecorder();
    
    void share2Wx();
};

#endif /* defined(__AppFunTest__WeChatManager__) */
