//
//  WeChatManager.cpp
//  AppFunTest
//
//  Created by Wxkj on 14/12/19.
//
//
#include "WxSDKManager.h"
#include "Voice/MediaRecorderUtils.h"
#include "FTPFileManager.h"
#import "WXApiManager.h"
#import "WXApiRequestHandler.h"
#import "Constant.h"
#import "DataMD5.h"
#import "XMLDictionary.h"
#include "LoginLayer.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../StoreLayer.h"
#include "../ShareLayer.h"
using namespace std;
using namespace cocos2d::network;

std::function<void (std::string error_code , std::string error_str)> _payCallBackFunc;

@interface WxSDK : UIViewController < UIApplicationDelegate , WXApiManagerDelegate>
{
}

@property (nonatomic) enum WXScene currentScene;

- (void) shareScreen2Line;

- (void) shareScreen2Friend;

- (void) shareGame2Line;

- (void) shareGame2Friend;

- (void) share2Friend:(NSString *)title ContentStr:(NSString *)content;

- (void) loginWithWx;

- (void) requestForWxPay:(int) price;

@end


@implementation WxSDK

@synthesize currentScene = _currentScene;

+ (WxSDK *)sharedInterstitialObj {
    static WxSDK *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [self new];
        [WXApiManager sharedManager].delegate = instance;
    });
    
    return instance;
}

- (void)dealloc {
    [WXApiManager sharedManager].delegate = nil;
    [super dealloc];
}


- (NSData *)checkThumbImageSize:(NSData *)thumbImageData
{
    static NSInteger maxThumbImageDataLen = 200*200;
    static CGFloat thumbImageCompressionQuality = 0.5;
    
    UIImage *image = [[UIImage alloc] initWithData:thumbImageData];
    UIGraphicsBeginImageContext(CGSizeMake(300, 169));
    [image drawInRect:CGRectMake(0, 0, 300, 169)];
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    NSData *data = UIImageJPEGRepresentation(image, 0.15);
    
    return data;
    
        if (thumbImageData.length > maxThumbImageDataLen)
        {
            UIImage *image = [[UIImage alloc] initWithData:thumbImageData];
            NSData *data = UIImageJPEGRepresentation(image, 0.5);
    
            if (data.length > maxThumbImageDataLen)
            {
                if(image.size.width > 400 || image.size.height > 400)
                {
                    //尺寸减到400＊400
                    //image = [UIImage scaleImage:image withSize:CGSizeMake(400, 400)];
                    data = UIImageJPEGRepresentation(image, 0.5);
                }
    
                if (data.length > maxThumbImageDataLen)
                {
                    //尺寸减到250＊250
                    if (image.size.width > 250 || image.size.height > 250)
                    {
                        //image = [UIImage scaleImage:image withSize:CGSizeMake(250, 250)];
                        data = UIImageJPEGRepresentation(image, thumbImageCompressionQuality);
                    }
    
                    if(data.length > maxThumbImageDataLen)
                    {
                        //尺寸减到200＊200
                        if (image.size.width > 150 || image.size.height > 150)
                        {
                            //image = [UIImage scaleImage:image withSize:CGSizeMake(150, 150)];
                            data = UIImageJPEGRepresentation(image, thumbImageCompressionQuality);
                        }
    
                        if (data.length > maxThumbImageDataLen)
                        {
                            if (image.size.width > 100 || image.size.height > 100)
                            {
                                //尺寸减到100*100
                                //image = [UIImage scaleImage:image withSize:CGSizeMake(100, 100)];
                                data = UIImageJPEGRepresentation(image, thumbImageCompressionQuality);
                            }
                        }
                    }
                }
            }
            
            thumbImageData = data;
        }
    return thumbImageData;
}

-(void)shareScreen2Line
{
    _currentScene = WXSceneTimeline;
    
    std::string  fileName = FileUtils::getInstance()->getWritablePath() + "share.jpg";
    NSData *imgData =[NSData dataWithContentsOfFile:[NSString stringWithUTF8String:fileName.c_str()]];
    [WXApiRequestHandler sendImageData:imgData
                               TagName:kImageTagName
                            MessageExt:kMessageExt
                                Action:kMessageAction
                            ThumbImage:[UIImage imageNamed:@"logo.png"]
                               InScene:_currentScene];
}


-(void)shareScreen2Friend
{
    _currentScene = WXSceneSession;
    
    std::string fileName = FileUtils::getInstance()->getWritablePath() + "share.jpg";
    
    NSData *imgData =[NSData dataWithContentsOfFile:[NSString stringWithUTF8String:fileName.c_str()]];
    UIImage *image = [[UIImage alloc] initWithData:imgData];
    
    UIGraphicsBeginImageContext(CGSizeMake(300, 169));
    [image drawInRect:CGRectMake(0, 0, 300, 169)];
    UIImage *thumbImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSData *data = UIImageJPEGRepresentation(image, 0.3);
    //ThumbImage:[UIImage imageWithData:compImg]
    //ThumbImage:[UIImage imageNamed:@"logo.png"]
    [WXApiRequestHandler sendImageData:data
                               TagName:kImageTagName
                            MessageExt:kMessageExt
                                Action:kMessageAction
                            ThumbImage:thumbImage
                               InScene:_currentScene];
}


-(void)shareGame2Line{
    _currentScene = WXSceneTimeline;
    
    //NSData *compImg = [self checkThumbImageSize:imgData];
    NSString *content =  [NSString stringWithFormat:@"全国人民都爱玩的麻将游戏,最地道的红中癞子玩法！随时随地组局,亲！快快加入吧！猛戳下载！"];
    [WXApiRequestHandler sendLinkURL:kShareLinkURL
                             TagName:kImageTagName
                               Title:content
                         Description:content
                          ThumbImage:[UIImage imageNamed:@"Icon-50.png"]
                             InScene:_currentScene];
}

-(void)shareGame2Friend{
    _currentScene = WXSceneSession;
    
    NSString *content =  [NSString stringWithFormat:@"全国人民都爱玩的麻将游戏,最地道的红中癞子玩法！随时随地组局,亲！快快加入吧！猛戳下载！"];
    [WXApiRequestHandler sendLinkURL:kShareLinkURL
                             TagName:kImageTagName
                               Title:@"欢腾红中麻将"
                         Description:content
                          ThumbImage:[UIImage imageNamed:@"Icon-50.png"]
                             InScene:_currentScene];
}

-(void)share2Friend:(NSString *)url Tittle:(NSString *)title ContentStr:(NSString *)content;
{
    _currentScene = WXSceneSession;
       
    //NSLog(@"分享标题 : %@" , title);
    //NSLog(@"分享内容 : %@" , content);
    
    [WXApiRequestHandler sendLinkURL:url
                             TagName:kImageTagName
                               Title:title
                         Description:content
                          ThumbImage:[UIImage imageNamed:@"Icon-50.png"]
                             InScene:_currentScene];
}

-(void)loginWithWx
{
    [WXApiRequestHandler sendAuthRequestScope: kAuthScope
                                        State:kAuthState
                                       OpenID:kAuthOpenID
                             InViewController:[WxSDK sharedInterstitialObj]];
}


- (void) requestForWxPay:(int) price{
    
    [[WxSDK sharedInterstitialObj] jumpToBizPay : price];
   }

#pragma mark - WXApiManagerDelegate
- (void)managerDidRecvGetMessageReq:(GetMessageFromWXReq *)req {
    // 微信请求App提供内容， 需要app提供内容后使用sendRsp返回
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App提供内容"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@", req.openID];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    alert.tag = kRecvGetMessageReqAlertTag;
    [alert show];
    [alert release];
}

- (void)managerDidRecvShowMessageReq:(ShowMessageFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //显示微信传过来的内容
    WXAppExtendObject *obj = msg.mediaObject;
    
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App显示内容"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n内容：%@ \n附带信息：%@ \n缩略图:%lu bytes\n附加消息:%@\n", req.openID, msg.title, msg.description, obj.extInfo, (unsigned long)msg.thumbData.length, msg.messageExt];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvLaunchFromWXReq:(LaunchFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //从微信启动App
    NSString *strTitle = [NSString stringWithFormat:@"从微信启动"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@, messageExt:%@", req.openID, msg.messageExt];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvMessageResponse:(SendMessageToWXResp *)response {
   
    ShareLayer::getInstance()->shareCallBackIOS(response.errCode);
    
    /*
    NSString *strTitle = [NSString stringWithFormat:@"分享结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d", response.errCode];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
     */
}

- (void)managerDidRecvAddCardResponse:(AddCardToWXCardPackageResp *)response {
    NSMutableString* cardStr = [[[NSMutableString alloc] init] autorelease];
    for (WXCardItem* cardItem in response.cardAry) {
        [cardStr appendString:[NSString stringWithFormat:@"cardid:%@ cardext:%@ cardstate:%u\n",cardItem.cardId,cardItem.extMsg,(unsigned int)cardItem.cardState]];
    }
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"add card resp"
                                                    message:cardStr
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response {
    
    NSLog(@"微信登录回调....");
    
    NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",kAPP_ID,kAPP_SECRET,response.code];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        NSLog(@"发送登录请求！！！！！");
        
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data) {
                
                NSLog(@"请求成功！！！！！");
                
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                
                NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/userinfo?access_token=%@&openid=%@",[dic objectForKey:@"access_token"],[dic objectForKey:@"openid"]];
                
                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                    NSURL *zoneUrl = [NSURL URLWithString:url];
                    NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
                    NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
                    dispatch_async(dispatch_get_main_queue(), ^{
                        if (data) {
                            NSDictionary *ndic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                            /*
                             {
                             city = Haidian;
                             country = CN;
                             headimgurl = "http://wx.qlogo.cn/mmopen/FrdAUicrPIibcpGzxuD0kjfnvc2klwzQ62a1brlWq1sjNfWREia6W8Cf8kNCbErowsSUcGSIltXTqrhQgPEibYakpl5EokGMibMPU/0";
                             language = "zh_CN";
                             nickname = "xxx";
                             openid = oyAaTjsDx7pl4xxxxxxx;
                             privilege =     (
                             );
                             province = Beijing;
                             sex = 1;
                             unionid = oyAaTjsxxxxxxQ42O3xxxxxxs;
                             }
                             */
                            
                            //NSString *headU = [ndic objectForKey:@"nickname"];
                            const char* headUrl = [[ndic objectForKey:@"headimgurl"] UTF8String];
                            const char* unionId = [[ndic objectForKey:@"unionid"] UTF8String];
                            const char* nickname = [[ndic objectForKey:@"nickname"] UTF8String];
                            int sex =[[ndic objectForKey:@"sex"] intValue] ;
                            //const char* sex = [[ndic objectForKey:@"sex"] UTF8String];
                            //Welcome::getInstance()->getWxinfo(headUrl, unionId, nickname, sex);
                            LoginLayer::getInstance()->getWxinfoByIOS(headUrl, unionId, nickname, sex);
                        }
                    });
                });
            }
        });
    });
}


#pragma mark - 拿到转换好的xml发送请求
- (void)http:(NSString *)xml {
    
   
}

#pragma mark - 产生随机订单号
- (NSString *)generateTradeNO {
    static int kNumber = 15;
    
    NSString *sourceStr = @"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    NSMutableString *resultStr = [[NSMutableString alloc] init];
    srand((unsigned)time(0));
    for (int i = 0; i < kNumber; i++) {
        unsigned index = rand() % [sourceStr length];
        NSString *oneStr = [sourceStr substringWithRange:NSMakeRange(index, 1)];
        [resultStr appendString:oneStr];
    }
    return resultStr;
}

- (void)managerDidRecvPayResponse:(BaseResp *)resp{
    NSLog(@"微信支付回调....");
    
    NSString *strMsg,*strTitle = [NSString stringWithFormat:@"支付结果"];
    std::string errorCode = "0";
    switch (resp.errCode) {
        case WXSuccess:
            strMsg = @"支付成功!!!";
            NSLog(@"支付成功－PaySuccess，retcode = %d", resp.errCode);
            break;
            
        default:
            strMsg = [NSString stringWithFormat:@"支付结果：失败！retcode = %d, retstr = %@", resp.errCode,resp.errStr];
            strMsg = @"支付失败 !!!";
            NSLog(@"错误，retcode = %d, retstr = %@", resp.errCode,resp.errStr);
            break;
    }
    
    StoreLayer::getInstance()->getPayInfoByIOS([[NSString stringWithFormat:@"%d",resp.errCode] UTF8String], [strMsg UTF8String]);

    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
    
}


@end

//-----------------------------------------------------



WxSDKManager * WxSDKManager ::getInstance(){
    static WxSDKManager * wechat=NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        wechat= new WxSDKManager;
    });
    return wechat;
}

void WxSDKManager::shareScreen2Line(){
    [[WxSDK sharedInterstitialObj]shareScreen2Line];
}

void WxSDKManager::shareScreen2Friend(){
    [[WxSDK sharedInterstitialObj]shareScreen2Friend];
}


void WxSDKManager::share2Friend(std::string roomNum, std::string zmCount, std::string gameCount){
    
    std::string title =  __String::createWithFormat("房间号 [ %s ]" ,roomNum.c_str())->getCString();
    std::string url  =__String::createWithFormat("https://a.mlinks.cc/AcAZ?roomId=%s",roomNum.c_str())->getCString();
    std::string content =  __String::createWithFormat("我在【欢腾红中麻将】开了%s局,%s个码的房间,天胡.清一色.红中癞子玩法,快来一起玩吧！" ,gameCount.c_str(), zmCount.c_str())->getCString();
    NSString *nsUrl = [NSString stringWithUTF8String:url.c_str()];
    NSString *nsTitle = [NSString stringWithUTF8String:title.c_str()];
    NSString *nsContent = [NSString stringWithUTF8String:content.c_str()];
    [[WxSDK sharedInterstitialObj]share2Friend:nsUrl Tittle:nsTitle ContentStr:nsContent];
}

void WxSDKManager::shareGame2Line(){
    [[WxSDK sharedInterstitialObj]shareGame2Line];
}

void WxSDKManager::shareGame2Friend(){
    [[WxSDK sharedInterstitialObj]shareGame2Friend];
}

void WxSDKManager::loginWithWx(){
    [[WxSDK sharedInterstitialObj]loginWithWx];
}

