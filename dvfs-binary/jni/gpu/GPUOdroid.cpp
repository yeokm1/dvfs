/*
 * GPUOdroid.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPUOdroid.h>
#include <cstdlib>
#include <stack>
#include <android/log.h>
#include "IOStuff.h"


#define FILE_GPU_UTIL "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization"
#define FILE_GPU_CURRENT_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_cur_clk"
#define FILE_GPU_MIN_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_min_lock"
#define FILE_GPU_MAX_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_max_lock"
#define FILE_GPU_AVAILABLE_FREQS "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_table"

#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView"
#define UTIL_RESULT_LENGTH 50

#define CLASSNAME "GPUOdroid"

GPUOdroid::GPUOdroid() {
	initGPUFreqValues();
	setGPUFreq(0);
}

GPUOdroid::~GPUOdroid() {
}


void GPUOdroid::initGPUFreqValues(){
	char freqLongString[FILE_BUFFER_SIZE];

	getStringFromFileByCat(FILE_GPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "GPU Freqs available: %s", freqLongString);

	char * freqString;

	long currentFreq;

    freqString = strtok (freqLongString," ");
    currentFreq = atol(freqString);


    //We use a stack as the frequencies in the file are in descending order
    std::stack<long> gpuFreqsStack;

    gpuFreqsStack.push(currentFreq);

	while ((freqString = strtok (NULL, " ")) != NULL){
	   currentFreq = atol(freqString);

	   if(currentFreq != 0){
		   gpuFreqsStack.push(currentFreq);
	   }
	 }


	gpuFreqs.clear();

	while(!gpuFreqsStack.empty()){
		long freq = gpuFreqsStack.top();
		gpuFreqsStack.pop();
		gpuFreqs.push_back(freq);
	}

}

void GPUOdroid::setGPUFreq(int position){
	gpuFreqPosition = position;
	long newFrequency = gpuFreqs[position];
	writeValueToFile(FILE_GPU_MIN_FREQ, newFrequency);
	writeValueToFile(FILE_GPU_MAX_FREQ, newFrequency);
}


int GPUOdroid::getFPS(){
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

float GPUOdroid::getUtilisation(){

	char buff[UTIL_RESULT_LENGTH];
	getStringFromFile(FILE_GPU_UTIL, buff, UTIL_RESULT_LENGTH);

	float numOutput = atof(buff);
	numOutput /= 2.56;

	return numOutput;
}
