#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include "cpu/CPUOdroid.h"
#include "gpu/GPUOdroid.h"

#define CLASSNAME "DVFS-ndk"
#define POLL_RATE_IN_MICROSECOND 1000000  //1 second
#define DO_NOT_PURSUE_FPS_VALUE -1


int fpsLowBound;
int fpsHighBound;
int slidingWindowLength;
bool dvfsInProgress;
int currentSlidingWindowPosition;
pthread_t threadTask;

void startDVFS(int _fpsLowBound, int _fpsHighBound, int _slidingWindowLength, bool _isPhoneNexus5);
void * threadFunction(void *arg);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
int shouldPursueFPSRecalculationToThisFPS(int fps);
void processInputs(int currentFPS, int newFPSValue, bool fpsInRange);
void runThisRegularly(CPUOdroid * cpu, GPUOdroid * gpu);

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

	fpsLowBound = _fpsLowBound;
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
	GPUOdroid gpu;

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

int shouldPursueFPSRecalculationToThisFPS(int fps){
	if(fps > fpsHighBound){
		//We need to decrease FPS
		return fpsLowBound;
	} else if(fps < fpsLowBound){
		//We need to increase FPS
		return fpsHighBound;
	} else {
		currentSlidingWindowPosition = 0;
		return DO_NOT_PURSUE_FPS_VALUE;
	}

}

void runThisRegularly(CPUOdroid * cpu, GPUOdroid * gpu){

	//	float util[NUM_CORES];
	//	cpu->getCPUUtil(util);
	//	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Util %f %f %f %f", util[0], util[1], util[2], util[3]);

	int currentFPS = gpu->getFPS();
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "FPS %d", currentFPS);

	if(currentFPS != NO_FPS_CALCULATED){

		int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);


		if(newValueFPS == DO_NOT_PURSUE_FPS_VALUE){
			processInputs(currentFPS, currentFPS, true);
		} else {
			processInputs(currentFPS, newValueFPS, false);
		}
	}

}

void processInputs(int currentFPS, int newFPSValue, bool fpsInRange){


	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Current FPS: %d, target FPS: %d", currentFPS, newFPSValue);
	//makeCPUMeetThisFPS(newFPSValue, currentFPS);

	if(fpsInRange){
		return;
	}

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Outside range, sliding window increased");

	currentSlidingWindowPosition++;

	if(currentSlidingWindowPosition > slidingWindowLength){

		currentSlidingWindowPosition = 0;

		//makeGPUMeetThisFPS(newFPSValue, currentFPS);
	}


}






