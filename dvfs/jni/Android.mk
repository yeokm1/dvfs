LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvfs
LOCAL_SRC_FILES := dvfs.cpp
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
