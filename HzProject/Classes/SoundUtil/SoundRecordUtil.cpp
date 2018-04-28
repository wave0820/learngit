#include "SoundRecordUtil.h"
#include "../Sound/YZAudioEngine.h"
#include "../Util/DateUtils.h"
#include "../WeChatManager.h"
#include "../FTPFileManager.h"


SoundRecordUtil::SoundRecordUtil(){
    //_loadingAudioThread = new std::thread(&SoundRecordUtil::loadingAudio,this);
    _uploadCallBack = nullptr;
    _downloadCallBack = nullptr;
}

SoundRecordUtil::~SoundRecordUtil(){
    //_loadingAudioThread->join();
    //CC_SAFE_DELETE(_loadingAudioThread);
}

bool SoundRecordUtil::init()
{
    
	return true;
}


void SoundRecordUtil::InitSoundRecordUtil(std::function<void(std::string fileName)> uploadCallBack, std::function<void(std::string fileName)> downloadCallBack){
    this->_uploadCallBack = uploadCallBack;
    this->_downloadCallBack = downloadCallBack;
}




std::string SoundRecordUtil::recordStart(std::string _fileName){
    
    voiceName = _fileName;
    
    CCLOG("本次录音 : %s" , voiceName.c_str());
    
    callRecordByPlatform();
    
    return voiceName;
}

    
void SoundRecordUtil::callRecordByPlatform(){
    
    auto filePath = FileUtils::getInstance()->getWritablePath();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WeChartManager::sharedInterstitial()->startRecorder(filePath, voiceName);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    bool isHave = false;
    isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/audios/AudioRecordTools","startRecord", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (isHave)
    {
        jstring aFilePath = minfo.env->NewStringUTF(filePath.c_str());
        jstring aFileName = minfo.env->NewStringUTF(voiceName.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , aFilePath , aFileName);
        minfo.env->DeleteLocalRef(aFilePath);
        minfo.env->DeleteLocalRef(aFileName);
    }else{
        CCLOG("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
    }
#endif
}

void SoundRecordUtil::recordEnd(bool isUpLoad){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WeChartManager::sharedInterstitial()->stopRecorder();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo minfo;
    bool  isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/audios/AudioRecordTools","stopRecord", "()V");
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    
#endif
    
    
    if (isUpLoad) {
        FTPFileManager::GetInst()->uploadFile2ServerByFtp(voiceName + ".amr" , CC_CALLBACK_1(SoundRecordUtil::upLoadRecordFileEnd, this));
    }
}

void SoundRecordUtil::recordVoicePlay(std::string _fileName){
    
    auto filePath = FileUtils::getInstance()->getWritablePath();
    
    CCLOG("播放声音 : %s" , _fileName.c_str());

    //目前录音结束以后，Android设置为自动播放。
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo minfo;
    bool isHave = false;
    
    isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/audios/AudioRecordTools","playChatRecord", "(Ljava/lang/String;Ljava/lang/String;)V");
    
    if (isHave)
    {
        jstring aFilePath = minfo.env->NewStringUTF(filePath.c_str());
        jstring aFileName = minfo.env->NewStringUTF(_fileName.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID , aFilePath , aFileName);
        minfo.env->DeleteLocalRef(aFilePath);
        minfo.env->DeleteLocalRef(aFileName);
    }else{
        JzDebug("Jni error.. not seach method... %d , %d" , minfo.classID ,  minfo.methodID);
    }
#endif
     */
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WeChartManager::sharedInterstitial()->playRecorder(filePath, _fileName);
#endif
}


//FTP上传文件完成
void SoundRecordUtil::upLoadRecordFileEnd(std::string fileName){
    _uploadCallBack(fileName);
}

//FTP下载文件完成
void SoundRecordUtil::downloadRecordFile(std::string fileName){
    _downloadCallBack(fileName);
    
    if (autoPlay) {
        recordVoicePlay(fileName);
    }
}

void SoundRecordUtil::requestDownloadRecordFile(std::string fileName , bool autoPlay){
    this->autoPlay = autoPlay;
    FTPFileManager::GetInst()->downloadFile2LocalByFtp(fileName , CC_CALLBACK_1(SoundRecordUtil::downloadRecordFile, this));
}

