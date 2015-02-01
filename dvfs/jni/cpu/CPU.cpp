/*
 * CPU.cpp
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */
#include <cstdlib>
#include <stdlib.h>
#include <android/log.h>
#include "CPU.h"
#include "IOStuff.c"




#define CLASSNAME "CPU"
#define USERSPACE "userspace"
#define SIZE_PROC_STAT_BUFF 4000

CPU::CPU() {
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Start");

	for(int i = 0; i < NUM_CORES; i++){
		prevCoreLoad[i] = 0;
		prevCoreTotal[i] = 0;
	}
	setGovernorToUserspace();
	cpuFreqPosition = 0;
}

CPU::~CPU() {
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

void CPU::getCPUUtil(float * util){
	//String[] toks = io.getAvailableOptionsFromFile(FILE_CPU_UTIL, false);

	char utilBuffString[SIZE_PROC_STAT_BUFF];
	getStringFromFile(FILE_CPU_UTIL, utilBuffString, SIZE_PROC_STAT_BUFF);

	int initialOffset = 10; //The initial offset is to skip the all cores fields
	int subsequentOffset = 10;

	char * valueString = strtok(utilBuffString, " ");

	int currentOffset = 1;

	for(; currentOffset < initialOffset; currentOffset++){
		valueString = strtok (NULL, " ");
	}

	for(int coreNumber = 0; coreNumber < NUM_CORES; coreNumber++){

		strtok (NULL, " "); //Skip the cpuN label

		long user = atol(strtok (NULL, " "));
		long nice = atol(strtok (NULL, " "));
		long system = atol(strtok (NULL, " "));
		long currentIdle = atol(strtok (NULL, " "));
		long iowait = atol(strtok (NULL, " "));
		long irq = atol(strtok (NULL, " "));
		long softirq = atol(strtok (NULL, " "));

		//Clear last few values
		strtok (NULL, " ");
		strtok (NULL, " ");

		long currentLoad = user + nice + system + iowait + irq + softirq;
		long currentTotal = currentLoad + currentIdle;

		util[coreNumber] = ((((float) (currentLoad - prevCoreLoad[coreNumber])) / (currentTotal - prevCoreTotal[coreNumber]))) * 100;

		prevCoreLoad[coreNumber] = currentLoad;
		prevCoreTotal[coreNumber] = currentTotal;
	}

}

