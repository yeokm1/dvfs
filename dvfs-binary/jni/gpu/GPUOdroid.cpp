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
	initGPUFreqValues(FILE_GPU_AVAILABLE_FREQS);
}

GPUOdroid::~GPUOdroid() {
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

void GPUOdroid::initGPUForModification(){
	setGPUFreq(0);
}
void GPUOdroid::revertGPUToOriginal(){
	writeValueToFile(FILE_GPU_MIN_FREQ, gpuFreqs[0]);
	writeValueToFile(FILE_GPU_MAX_FREQ, gpuFreqs[gpuFreqs.size() - 1]);
}
