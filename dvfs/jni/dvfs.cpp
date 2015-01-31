#include <jni.h>
#include <android/log.h>

#define APPNAME "DVFS"


void
Java_com_example_dvfs_DVFSNdk_startDVFS( JNIEnv* env,
                                                  jobject thiz ){
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "input %d", 2);
}

