LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared
LOCAL_CFLAGS += -DCOCOS2D_DEBUG=1
LOCAL_MODULE_FILENAME := libcocos2dcpp

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
 

ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes)
 
FILE_LIST := hellocpp/main.cpp \
			jubaosdk/ShellApiJni.cpp \

FILE_LIST += $(filter %.cpp, $(ALLFILES))
 
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += libiconv_static
 
include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,external/libiconv)
