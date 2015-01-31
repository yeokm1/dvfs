#include <jni.h>
#include <android/log.h>

#define APPNAME "DVFS-ndk"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_dvfs_DVFSNdk_startDVFS( JNIEnv* env, jobject thiz, jint fpsLowbound, jint fpsHighBound, jint slidingWindowLength, jboolean isPhoneNexus5){
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "lowbound: %d, highbound: %d, slidingWindow: %d, isPhoneNexus5: %d",  fpsLowbound,  fpsHighBound, slidingWindowLength, isPhoneNexus5);
	//startDVFS(fpsLowbound, fpsHighBound, slidingWindowLength, isPhoneNexus5);
}

void
Java_com_example_dvfs_DVFSNdk_stopDVFS( JNIEnv* env, jobject thiz ){
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Stop DVFS");
}


void startDVFS(int fpsLowBound, int fpsHighBound, int slidingWindowLength, bool isPhoneNexus5){

}


