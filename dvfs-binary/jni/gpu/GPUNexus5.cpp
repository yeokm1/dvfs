/*
 * GPUNexus5.cpp
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPUNexus5.h>
#include <stdio.h>
#include "IOStuff.h"

#define POWER_SCALE_POLICY "/sys/class/kgsl/kgsl-3d0/pwrscale/policy"
#define FILE_GPU_MAX_FREQ "/sys/class/kgsl/kgsl-3d0/max_gpuclk"
#define FILE_GPU_FREQ "/sys/class/kgsl/kgsl-3d0/gpuclk"
#define FILE_GPU_AVAILABLE_FREQS "/sys/class/kgsl/kgsl-3d0/gpu_available_frequencies"
#define FILE_GPU_UTIL "/sys/class/kgsl/kgsl-3d0/gpubusy"

#define CLASSNAME "GPUNexus5"

GPUNexus5::GPUNexus5() {
	initGPUFreqValues(FILE_GPU_AVAILABLE_FREQS);
}

GPUNexus5::~GPUNexus5() {
}

void GPUNexus5::setGPUFreq(int position){

	gpuFreqPosition = position;
	long newFrequency = gpuFreqs[position];
	writeValueToFile(FILE_GPU_FREQ, newFrequency);

}

void GPUNexus5::setToNoGPUPolicy(){
	writeStringToFile(POWER_SCALE_POLICY, "none");
}

void GPUNexus5::setToDefaultGPUPolicy(){
	writeStringToFile(POWER_SCALE_POLICY, "trustzone");
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

void GPUNexus5::initGPUForModification(){
	setToNoGPUPolicy();
	writeValueToFile(FILE_GPU_MAX_FREQ, gpuFreqs[gpuFreqs.size() - 1]);
	setGPUFreq(0);
}
void GPUNexus5::revertGPUToOriginal(){
	//setToDefaultGPUPolicy();
}



