#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include "cpu/CPUOdroid.h"
#include "gpu/GPU.h"

#define CLASSNAME "DVFS-ndk"
#define POLL_RATE_IN_MICROSECOND 1000000  //1 second


int fpsLowbound;
int fpsHighBound;
int slidingWindowLength;
bool dvfsInProgress;
pthread_t threadTask;

void startDVFS(int _fpsLowBound, int _fpsHighBound, int _slidingWindowLength, bool _isPhoneNexus5);
void * threadFunction(void *arg);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);

void runThisRegularly(CPUOdroid * cpu, GPU * gpu);

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

//From http://stackoverflow.com/questions/1468596/calculating-elapsed-time-in-a-c-program-in-milliseconds
/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1){
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void * threadFunction(void *arg){

	CPUOdroid cpu;
	GPU gpu;

	struct timeval tvBegin, tvEnd, tvDiff;

	while(dvfsInProgress){

		gettimeofday(&tvBegin, NULL);

		runThisRegularly(&cpu, &gpu);

		gettimeofday(&tvEnd, NULL);

		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

		int elapsedTime = tvDiff.tv_usec;

		//This is to calculate how much time to sleep before the next poll
		if(elapsedTime < POLL_RATE_IN_MICROSECOND){
			int timeToSleep = POLL_RATE_IN_MICROSECOND - elapsedTime;
			usleep(timeToSleep);
		}


	}


	return NULL;

}


void runThisRegularly(CPUOdroid * cpu, GPU * gpu){
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Thread run");

//	float util[NUM_CORES];
//	cpu->getCPUUtil(util);
//	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Util %f %f %f %f", util[0], util[1], util[2], util[3]);

	 int fps = gpu->getFPS();
	 __android_log_print(ANDROID_LOG_INFO, CLASSNAME, "FPS %d", fps);

}






