/*
 * CPUNexus5.cpp
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#include <cpu/CPUNexus5.h>
#include <android/log.h>
#include "IOStuff.h"

#define FILE_CPU_AVAILABLE_FREQS "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"
#define FILE_CPU_SET_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define FILE_CPU_SET_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define LOWEST_FREQ_POSITION 0 //0.3 Ghz
#define HIGHEST_FREQ_POSITION 7 //1.2 Ghz

#define CLASSNAME "CPUNexus5"

CPUNexus5::CPUNexus5() {
	initCPUFreqValues();
	afterSetGovernorToUserspace();
	setCPUFreq(0);
}

CPUNexus5::~CPUNexus5() {
}


void CPUNexus5::initCPUFreqValues(){

	char freqLongString[FILE_BUFFER_SIZE];
	getStringFromFile(FILE_CPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Freqs available: %s", freqLongString);


	char * freqString;
	long currentFreq;

	freqString = strtok (freqLongString," ");


	if(LOWEST_FREQ_POSITION == 0){
		currentFreq = atol(freqString);
		cpuFreqs.push_back(currentFreq);
	}

	for(int i = 1; (freqString = strtok (NULL, " ")) != NULL; i++){
		if(i >= LOWEST_FREQ_POSITION && i <= HIGHEST_FREQ_POSITION){
			currentFreq = atol(freqString);
			__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Freqs used %ld", currentFreq);
			cpuFreqs.push_back(currentFreq);
		}
	}




}

void CPUNexus5::afterSetGovernorToUserspace(){
	writeValueToFile(FILE_CPU_SET_MIN_FREQ, cpuFreqs[0]);
	writeValueToFile(FILE_CPU_SET_MAX_FREQ, cpuFreqs[cpuFreqs.size() - 1]);
}

