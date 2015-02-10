/*
 * GPUNexus5.cpp
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPUNexus5.h>
#include <android/log.h>
#include <stdio.h>
#include "IOStuff.h"

#define POWER_SCALE_POLICY "/sys/class/kgsl/kgsl-3d0/pwrscale/policy"
#define FILE_GPU_MAX_FREQ "/sys/class/kgsl/kgsl-3d0/max_gpuclk"
#define FILE_GPU_AVAILABLE_FREQS "/sys/class/kgsl/kgsl-3d0/gpu_available_frequencies"
#define FILE_GPU_UTIL "/sys/class/kgsl/kgsl-3d0/gpubusy"

#define CLASSNAME "GPUNexus5"

#define FPS_COMMAND "su -c dumpsys SurfaceFlinger --latency SurfaceView"

GPUNexus5::GPUNexus5() {
	initGPUFreqValues();
	setToNoGPUPolicy();
	setGPUFreq(0);
}

GPUNexus5::~GPUNexus5() {
}


void GPUNexus5::initGPUFreqValues(){
	char freqLongString[FILE_BUFFER_SIZE];

	getStringFromFileByCat(FILE_GPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "GPU Freqs available: %s", freqLongString);

	char * freqString;
	long currentFreq;

	freqString = strtok (freqLongString," ");
	currentFreq = atol(freqString);
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "GPU Freqs used %ld", currentFreq);
	gpuFreqs.push_back(currentFreq);


	for(int i = 1; (freqString = strtok (NULL, " ")) != NULL; i++){
		currentFreq = atol(freqString);
		__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "GPU Freqs used %ld", currentFreq);
		gpuFreqs.push_back(currentFreq);
	}

}

void GPUNexus5::setGPUFreq(int position){
	gpuFreqPosition = position;
	long newFrequency = gpuFreqs[position];
	writeValueToFile(FILE_GPU_MAX_FREQ, newFrequency);
}

void GPUNexus5::setToNoGPUPolicy(){
	writeStringToFile(POWER_SCALE_POLICY, "none");
}


int GPUNexus5::getFPS(){
	FILE *pp = popen(FPS_COMMAND, "r");

	char line[FPS_RESULT_LINE_LENGTH];
	//Ignore the first line
	fgets(line, sizeof(line), pp);

	if(strlen(line) == 0){
		pclose(pp);
		return NO_FPS_CALCULATED;
	}


	char *finishFrameTime;
	stack<long long> values;

	int linesEncountered = 0;
	while(fgets(line, sizeof (line), pp) != NULL){
    	linesEncountered++;

    	if(linesEncountered < LINES_IN_DUMPSYS_TO_IGNORE){
    		continue;
    	}

		//Ignore the first two values
		strtok(line, "\t");
		strtok(NULL, "\t");
		finishFrameTime = strtok(NULL, "\t");

		if(finishFrameTime != NULL){
			long long parsed = atoll(finishFrameTime);
			values.push(parsed);
		}

	}

	pclose(pp);

	//Final check to ensure there is data collected in the first place
	if(values.size() == 0){
		return NO_FPS_CALCULATED;
	}

	int frameCount = 0;
	long long lastFrameFinishedTime = values.top();

	while(!values.empty()){
		long long currentValue = values.top();
		values.pop();

		if ((lastFrameFinishedTime - currentValue) <= FPS_INTERVAL_NANOS)	{
			frameCount++;
		}
	}

	if(frameCount == 1){
		return NO_FPS_CALCULATED;
	}


	//Cap to Max FPS
	return (frameCount <= MAX_FPS) ? frameCount : MAX_FPS;
}

float GPUNexus5::getUtilisation(){
	FILE *filePtr = fopen(FILE_GPU_UTIL, "r");
	int busyCycles;
	int totalCycles;

	fscanf(filePtr, "%d %d",&busyCycles, &totalCycles);
	fclose(filePtr);

	if(totalCycles == 0){
		return 0;
	} else {
		float util = (((float) busyCycles) / totalCycles) * 100;
		return util;
	}
}



