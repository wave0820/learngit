#ifndef SoundRecordUtil_h__
#define SoundRecordUtil_h__

#include "cocos2d.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif

#endif

class SoundRecordUtil : public Node
{
    
private:
    std::function<void (std::string fileName)> _uploadCallBack;
    std::function<void (std::string fileName)> _downloadCallBack;
public:
  
    /**
     *  开始录音
     */
    std::string recordStart(std::string _fileName);
    
    /**
     *  结束录音
     *  isUpLoad 录音结束后，是否自动上传。
     */
    void recordEnd(bool isUpLoad = true);

    /**
     *  录音播放
     */
    void recordVoicePlay(std::string _fileName);
    
private:
    
    /**
     * 根据平台开始录音
     */
    void callRecordByPlatform();
    
private:

    std::string voiceName;  //本次录音名称
    
    bool autoPlay;
public:
    
    virtual bool init();
    
    SoundRecordUtil();
    ~SoundRecordUtil();
    
    /**
     * 上传录音文件到FTP服务器完成
     */
    void upLoadRecordFileEnd(std::string fileName);         //上传文件完成
    
    /**
     * 下载录音文件到FTP服务器完成
     */
    void downloadRecordFile(std::string fileName);      //下载文件完成
    
    /**
     * 请求下载录音文件
     * fileName 文件名
     * autoPlay 下载完成后，是否自动播放
     */
    void requestDownloadRecordFile(std::string fileName, bool autoPlay = true);   //请求下载文件
    
    /**
     * 初始化录音工具类
     * uploadCallBack 上传文件完成的回调
     * downloadCallBack 下载文件结束的回调
     */
    void InitSoundRecordUtil(std::function<void(std::string fileName)> uploadCallBack, std::function<void(std::string fileName)> downloadCallBack);
    
    CREATE_FUNC(SoundRecordUtil);
};


#endif // FriendLayer_h__
