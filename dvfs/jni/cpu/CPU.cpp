/*
 * CPU.cpp
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */
#include <stdlib.h>
#include <android/log.h>
#include "CPU.h"
#include "IOStuff.c"



#define CLASSNAME "CPU"
#define USERSPACE "userspace"

CPU::CPU() {
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Start");

	for(int i = 0; i < NUM_CORES; i++){
		prevCoreLoad[i] = 0;
		prevCoreTotal[i] = 0;
	}

	cpuFreqPosition = -1;
}

CPU::~CPU() {
	free(prevCoreLoad);
	free(prevCoreTotal);
}


vector<long> CPU::getCPUFreqs(){
	return cpuFreqs;
}

void CPU::setCPUFreq(int position){
	cpuFreqPosition = position;
	long newFrequency = cpuFreqs[position];
	writeValueToFile(FILE_CPU_SCALING_FREQ,  newFrequency);
}

void CPU::setGovernorToUserspace(){
	writeStringToFile(FILE_CPU_SCALING_GOVERNER, USERSPACE);
}

