#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include "cpu/CPUOdroid.h"

#define CLASSNAME "DVFS-ndk"


int fpsLowbound;
int fpsHighBound;
int slidingWindowLength;
bool dvfsInProgress;
pthread_t threadTask;

void startDVFS(int _fpsLowBound, int _fpsHighBound, int _slidingWindowLength, bool _isPhoneNexus5);
void * threadFunction(void *arg);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_dvfs_DVFSNdk_startDVFS( JNIEnv* env, jobject thiz, jint fpsLowbound, jint fpsHighBound, jint slidingWindowLength, jboolean isPhoneNexus5){
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "lowbound: %d, highbound: %d, slidingWindow: %d, isPhoneNexus5: %d",  fpsLowbound,  fpsHighBound, slidingWindowLength, isPhoneNexus5);
	startDVFS(fpsLowbound, fpsHighBound, slidingWindowLength,  isPhoneNexus5);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_dvfs_DVFSNdk_stopDVFS( JNIEnv* env, jobject thiz ){
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Stop DVFS");
	dvfsInProgress = false;
}



void startDVFS(int _fpsLowBound, int _fpsHighBound, int _slidingWindowLength, bool _isPhoneNexus5){

	fpsLowbound = _fpsLowBound;
	fpsHighBound = _fpsHighBound;
	slidingWindowLength = _slidingWindowLength;
	dvfsInProgress = true;
	pthread_create(&threadTask, NULL, threadFunction, NULL);


}

void * threadFunction(void *arg){

	CPUOdroid cpu;



	while(dvfsInProgress){
		__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Thread regular");
		sleep(1);
	}


	return NULL;

}






