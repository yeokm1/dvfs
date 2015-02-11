#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "cpu/CPUOdroid.h"
#include "cpu/CPUNexus5.h"
#include "gpu/GPUOdroid.h"
#include "gpu/GPUNexus5.h"
#include "IOStuff.h"
#include "RetrieveModel.h"


//#define DEBUG

#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif


#define CLASSNAME "DVFS-ndk"
#define POLL_RATE_IN_MICROSECOND 1000000  //1 second
#define DO_NOT_PURSUE_FPS_VALUE -1
#define TARGET_CPU_UTILISATION 80
#define SLIDING_WINDOW_LENGTH 5

int fpsLowBound;
int fpsHighBound;
bool dvfsInProgress;
int currentSlidingWindowPosition;
pthread_t threadTask;

void startDVFS(int _fpsLowBound, int _fpsHighBound);
void * threadFunction(void *arg);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
int shouldPursueFPSRecalculationToThisFPS(int fps);
void processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu);
void runThisRegularly(CPU * cpu, GPU * gpu);
int findLowestFreqPositionThatMeetsThisCost(double costToMeet, vector<long> availableFrequencies, float factor);
void makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu);
void makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu);


int main( int argc, char* argv[]){
	if(argc != 3){
		printf("Not enough arguments\n");
		return 1;
	}

	char * lowBoundStr = argv[1];
	char * highBoundStr = argv[2];

	int lowBound = atoi(lowBoundStr);
	int highBound = atoi(highBoundStr);

	if(lowBound > highBound){
		printf("Low Bound cannot be higher that High bound\n");
		return 1;
	}

    startDVFS(lowBound, highBound);
    return 0;
}




void startDVFS(int _fpsLowBound, int _fpsHighBound){

	fpsLowBound = _fpsLowBound;
	fpsHighBound = _fpsHighBound;
	dvfsInProgress = true;

	//pthread_create(&threadTask, NULL, threadFunction, NULL);
	threadFunction(NULL);

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

	D(printf("Thread function start\n"));

	CPU * cpu;
	GPU * gpu;
	if(isPhoneNexus5()){
		D(printf("Model Nexus 5\n"));
		cpu = new CPUNexus5();
		gpu = new GPUNexus5();
	} else {
		D(printf("Model not Nexus 5\n"));
		cpu = new CPUOdroid();
		gpu = new GPUOdroid();
	}

	struct timeval tvBegin, tvEnd, tvDiff;

	while(dvfsInProgress){

		gettimeofday(&tvBegin, NULL);

		runThisRegularly(cpu, gpu);

		gettimeofday(&tvEnd, NULL);

		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

		int elapsedTime = tvDiff.tv_usec;

		//This is to calculate how much time to sleep before the next poll
		if(elapsedTime < POLL_RATE_IN_MICROSECOND){
			int timeToSleep = POLL_RATE_IN_MICROSECOND - elapsedTime;
			usleep(timeToSleep);
		}


	}

	free(gpu);
	free(cpu);

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

void runThisRegularly(CPU * cpu, GPU * gpu){

	int currentFPS = gpu->getFPS();
	D(printf("FPS %d\n", currentFPS));

	if(currentFPS == NO_FPS_CALCULATED){
		//Recalculate GPU immediately on next try if cannot get FPS
		currentSlidingWindowPosition = SLIDING_WINDOW_LENGTH;
	} else {

		int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);


		if(newValueFPS == DO_NOT_PURSUE_FPS_VALUE){
			processInputs(currentFPS, currentFPS, true, cpu, gpu);
		} else {
			processInputs(currentFPS, newValueFPS, false, cpu , gpu);
		}
	}

}

int findLowestFreqPositionThatMeetsThisCost(double costToMeet, vector<long> availableFrequencies, float factor){

	for(unsigned int position = 0; position < availableFrequencies.size(); position++){
		long chosenFreq = availableFrequencies[position];
		double calculatedCost = chosenFreq * factor;
		if(calculatedCost >= costToMeet){
			return position;
		}
	}

	return availableFrequencies.size() - 1;
}


void makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu){
	D(printf("CPU meet this FPS: %d\n", targetFPS));

	int currentCPUFreqPosition = cpu->getCpuFreqPosition();

	vector<long> cpuFreqs = cpu->getCPUFreqs();

	float cpuUtil = cpu->getUtilisationOfHighestCore();
	D(printf("CPU Highest Util %f\n", cpuUtil));


	long currentCPUFrequency = cpuFreqs[currentCPUFreqPosition];


	double currentCost = (cpuUtil * currentCPUFrequency)
					* (((double)targetFPS)
							/ currentFPS);

	float targetCPUUtil;

	if(cpuUtil > TARGET_CPU_UTILISATION){
		targetCPUUtil = cpuUtil;
	} else {
		targetCPUUtil = TARGET_CPU_UTILISATION;
	}


	int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, cpuFreqs, targetCPUUtil);


	if(currentCPUFreqPosition != newCPUFreqPosition){
		cpu->setCPUFreq(newCPUFreqPosition);
	}

	D(printf("New CPU Freq Pos: %d\n", newCPUFreqPosition));
}

void makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu){
	D(printf("GPU meet this FPS: %d\n", targetFPS));

	int currentGPUFreqPosition = gpu->getGpuFreqPosition();

	vector<long> gpuFreqs = gpu->getGPUFreqs();

	long currentGPUFrequency = gpuFreqs[currentGPUFreqPosition];

	double currentCost = currentGPUFrequency
					* (((double)targetFPS)
							/ currentFPS);


	int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, gpuFreqs, 1);


	if(currentGPUFreqPosition != newGPUFreqPosition){
		gpu->setGPUFreq(newGPUFreqPosition);
	}

	D(printf("New GPU Freq Pos: %d\n", newGPUFreqPosition));
}

void processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu){


	D(printf("Current FPS: %d, target FPS: %d\n", currentFPS, newFPSValue));
	makeCPUMeetThisFPS(newFPSValue, currentFPS, cpu);

	if(fpsInRange){
		return;
	}

	currentSlidingWindowPosition++;

	if(currentSlidingWindowPosition > SLIDING_WINDOW_LENGTH){
		currentSlidingWindowPosition = 0;
		makeGPUMeetThisFPS(newFPSValue, currentFPS, gpu);
	}


}
