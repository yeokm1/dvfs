LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvfs-binary
LOCAL_SRC_FILES := dvfs-binary.cpp IOStuff.cpp dvfs/DVFS.cpp dvfs/DVFSNew.cpp dvfs/DVFSOriginal.cpp dvfs/DVFSAlex.cpp cpu/CPU.cpp cpu/CPUOdroid.cpp cpu/CPUNexus5.cpp gpu/GPU.cpp gpu/GPUOdroid.cpp gpu/GPUNexus5.cpp
LOCAL_CPPFLAGS := -std=gnu++0x -Wall         # whatever g++ flags you like

include $(BUILD_EXECUTABLE)    # <-- Use this to build an executable.