#  Created on: 27 Aug, 2014
#  Author: Alexandru Irimiea, alexandru.irim@gmail.com


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -std=c99
LOCAL_MODULE    := dvfs-alex
LOCAL_SRC_FILES :=  PowerManagementCore.c PowerManagementTest.c PowerManagementUtil.c DroidPwMgmtCpu.c CommonUtilities.c DroidPwMgmtCommon.c DroidPwMgmtGpu.c
#LOCAL_CPPFLAGS := -std=gnu++0x -Wall         # whatever g++ flags you like

include $(BUILD_EXECUTABLE)    # <-- Use this to build an executable.
