LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvfs
LOCAL_SRC_FILES := dvfs.cpp IOStuff.cpp cpu/CPU.cpp cpu/CPUOdroid.cpp cpu/CPUNexus5.cpp gpu/GPU.cpp gpu/GPUOdroid.cpp
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
