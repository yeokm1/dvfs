/*
 * CPU.cpp
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */

#include <cpu/CPU.h>
#include "IOStuff.h"

CPU::CPU() {

	prevCoreLoad = new double[NUM_CORES];
	prevCoreTotal = new double[NUM_CORES];

	for(int i = 0; i < NUM_CORES; i++){
		prevCoreLoad[i] = 0;
		prevCoreTotal[i] = 0;
	}

	initCPUFreqs();

	priorToSetGovernorToUserspace();
	setGovernorToUserspace();
	setCPUFreq(0);
}

CPU::~CPU() {
	free(prevCoreLoad);
	free(prevCoreTotal);
	free(cpuFreqs);
}

int CPU::getNumCPUFreqs(){
	return numCPUFreqs;
}

long * CPU::getCPUFreqs(){
	return cpuFreqs;
}

void CPU::setCPUFreq(int position){
	cpuFreqPosition = position;
	long newFrequency = cpuFreqs[position];
	writeValueToFile(FILE_CPU_SCALING_FREQ,  newFrequency);
}

void CPU::setGovernorToUserspace(){
	writeStringToFile(FILE_CPU_SCALING_GOVERNER, "userspace");
}

