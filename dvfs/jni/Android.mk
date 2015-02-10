LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvfs
LOCAL_SRC_FILES := dvfs.cpp RetrieveModel.cpp IOStuff.cpp cpu/CPU.cpp cpu/CPUOdroid.cpp cpu/CPUNexus5.cpp gpu/GPU.cpp gpu/GPUOdroid.cpp gpu/GPUNexus5.cpp
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS := -std=gnu++0x -Wall  

include $(BUILD_SHARED_LIBRARY)
