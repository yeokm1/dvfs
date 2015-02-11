/*
 * GPUOdroid.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPUOdroid.h>
#include <cstdlib>
#include <stack>
#include "IOStuff.h"


#define FILE_GPU_UTIL "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization"
#define FILE_GPU_CURRENT_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_cur_clk"
#define FILE_GPU_MIN_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_min_lock"
#define FILE_GPU_MAX_FREQ "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_max_lock"
#define FILE_GPU_AVAILABLE_FREQS "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_table"

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

float GPUOdroid::getUtilisation(){

	char buff[UTIL_RESULT_LENGTH];
	getStringFromFile(FILE_GPU_UTIL, buff, UTIL_RESULT_LENGTH);

	float numOutput = atof(buff);
	numOutput /= 2.56;

	return numOutput;
}
