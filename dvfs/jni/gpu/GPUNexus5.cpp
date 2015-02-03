/*
 * GPUNexus5.cpp
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPUNexus5.h>
#include <android/log.h>
#include "IOStuff.h"

#define POWER_SCALE_POLICY "/sys/class/kgsl/kgsl-3d0/pwrscale/policy"
#define FILE_GPU_MAX_FREQ "/sys/class/kgsl/kgsl-3d0/max_gpuclk"
#define FILE_GPU_AVAILABLE_FREQS "/sys/class/kgsl/kgsl-3d0/gpu_available_frequencies"

#define CLASSNAME "GPUNexus5"

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

