LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvfs
LOCAL_SRC_FILES := dvfs.cpp IOStuff.c cpu/CPU.cpp cpu/CPUOdroid.cpp gpu/GPU.cpp
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
