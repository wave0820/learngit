//
//  WeChatManager.cpp
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//
#include "WeChatManager.h"
#include "Voice/MediaRecorderUtils.h"
#include "FTPFileManager.h"

@interface WeChatObj : UIResponder < UIApplicationDelegate>
{
}

- (void) startRecorder:(NSString *)filePath AndFileName:(NSString *)fileName;

- (void) playRecorder:(NSString *)filePath AndFileName:(NSString *)fileName;

- (void) stopRecorder;

- (void) share2Wx;

- (bool) canRecorder;

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

- (void) startRecorder:(NSString *)filePath AndFileName:(NSString *)fileName
{
    NSLog(@"startRecorder");
    
    //NSString *str= [NSString stringWithCString:DateUtils::getCurrentTime(false).c_str() encoding:[NSString defaultCStringEncoding]];
    
    //NSString * newFileName =  [fileName stringByAppendingString:str];
    
    [MediaRecorderUtils init:filePath AndFileName:fileName];
    
    sfileName=  [fileName UTF8String];
    
    [MediaRecorderUtils startRecord];
}

- (void) playRecorder:(NSString *)filePath AndFileName:(NSString *)fileName
{
    NSLog(@"playRecorder");
    NSLog(@"filePath Path:%@",filePath);
    NSLog(@"fileName Path:%@",fileName);
    
    [MediaRecorderUtils playRecord:filePath AndFileName:fileName];
}


-(void)stopRecorder
{
    NSLog(@"stopRecorder");
    
    [MediaRecorderUtils stopRecord:@"http://www.dadagame.cn"];
    
    NSLog(@"上传到服务器...");
}

-(void)share2Wx
{

}

- (bool) canRecorder
{
    return [MediaRecorderUtils canRecord];
}


@end

//-----------------------------------------------------



WeChartManager * WeChartManager ::sharedInterstitial(){
    static WeChartManager * wechat=NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        wechat= new WeChartManager;
    });
    return wechat;
}

void WeChartManager::startRecorder(std::string filePath , std::string sfileName){
    
    NSString *filePathStr= [NSString stringWithCString:filePath.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *fileName = [NSString stringWithCString:sfileName.c_str() encoding:[NSString defaultCStringEncoding]];
    [[WeChatObj sharedInterstitialObj] startRecorder:filePathStr AndFileName:fileName];

}

void WeChartManager::stopRecorder(){
    [[WeChatObj sharedInterstitialObj]stopRecorder];
}

void WeChartManager::playRecorder(std::string filePath , std::string sfileName){
    NSString *filePathStr= [NSString stringWithCString:filePath.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *fileName = [NSString stringWithCString:sfileName.c_str() encoding:[NSString defaultCStringEncoding]];
    [[WeChatObj sharedInterstitialObj] playRecorder:filePathStr AndFileName:fileName];
}


bool WeChartManager::canRecorder(){
    return [[WeChatObj sharedInterstitialObj]canRecorder];
}

void WeChartManager::share2Wx(){
    [[WeChatObj sharedInterstitialObj]share2Wx];
}
