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

#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView && echo e"

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

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Get FPS start");


	redi::pstream * proc = getShellInstance();
	* proc << FPS_COMMAND << std::endl;

	string intermediate;
	char *finishFrameTime;

	//Grab the first line. It is usually in the form 16666... and useless
	std::getline(*proc, intermediate);

	if(intermediate.compare(FPS_COMMAND_ENDING) == 0){
		return NO_FPS_CALCULATED;
	}

	stack<long long> values;


	int linesEncountered = 0;
    while (true) {

    	linesEncountered++;
    	std::getline(*proc, intermediate);
    	if(intermediate.compare(FPS_COMMAND_ENDING) == 0){
    		break;
    	}

    	if(linesEncountered < LINES_IN_DUMPSYS_TO_IGNORE){
    		continue;
    	}

		const char * lineConst = intermediate.c_str();
		char line[intermediate.length()];
		std::strcpy(line, lineConst);

		strtok(line, "\t");
		strtok(NULL, "\t");
		finishFrameTime = strtok(NULL, "\t");

		if(finishFrameTime != NULL){
			long long parsed = atoll(finishFrameTime);
			values.push(parsed);
		}

    }

	if(values.size() == 0){
		return NO_FPS_CALCULATED;
	}


	int frameCount = 0;
	int fps = 0;
	long long lastFrameFinishedTime = values.top();

	while(!values.empty()){
		long long currentValue = values.top();
		values.pop();

		if ((lastFrameFinishedTime - currentValue) <= FPS_INTERVAL_NANOS)	{
			frameCount++;
		}
	}


	//Cap to Max FPS
	return (frameCount <= MAX_FPS) ? frameCount : MAX_FPS;
}
