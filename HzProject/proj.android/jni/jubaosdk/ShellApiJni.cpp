#include "ShellApiJni.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "ShellApiJni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void ShellApiJni::pay(PayOrder payOrder) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org.cocos2dx.jubaosdk.ShellApiHelper", "pay", 
		// "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring appId = t.env->NewStringUTF(payOrder.M_appId_.c_str());
		jstring amount = t.env->NewStringUTF(payOrder.M_amount_.c_str());
		jstring goodsName = t.env->NewStringUTF(payOrder.M_goodsName_.c_str());
		jstring remark = t.env->NewStringUTF(payOrder.M_remark_.c_str());
		jstring payId = t.env->NewStringUTF(payOrder.M_payId_.c_str());
		jstring playerId = t.env->NewStringUTF(payOrder.M_playerId_.c_str());
		// jstring channelId = t.env->NewStringUTF(payOrder.M_channelId_.c_str());
		
		// t.env->CallStaticVoidMethod(t.classID, t.methodID, appId, amount, goodsName, remark, payId, playerId, channelId);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, appId, amount, goodsName, remark, payId, playerId);
		t.env->DeleteLocalRef(t.classID);		
	}
}

std::string ShellApiJni::srand() {
	std::string ret("");
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com.fanwei.jubaosdk.common.util.CommonUtils", "setRand", "()Ljava/lang/String;")) {
		jstring rand = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(rand);
		t.env->DeleteLocalRef(rand);
	}
	return ret;
}

static ShellApiJni::Callback *G_Callback = nullptr;
void ShellApiJni::regCallback(Callback* callback) {
	if (callback != nullptr) {
		G_Callback = callback;
	}
}

extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_jubaosdk_ShellApiHelper_nativeDispatch(JNIEnv *env, jobject thiz, jint code, jstring message) {
		if (G_Callback != nullptr)	{
			G_Callback->call(code, JniHelper::jstring2string(message));
		}
	}
}