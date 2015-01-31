/*
 * CPUOdroid.cpp
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#include <cstdlib>
#include <android/log.h>
#include "CPUOdroid.h"
#include "IOStuff.h"

#define FILE_CPU_AVAILABLE_FREQS "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table"
#define LOWEST_FREQ_POSITION 7 //0.6 GHz
#define HIGHEST_FREQ_POSITION 17 //1.6 GHz

#define FILE_BUFFER_SIZE 300

#define CLASSNAME "CPUOdroid"

CPUOdroid::CPUOdroid(){
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPUOdroid Start");

	initCPUFreqValues();
	setGovernorToUserspace();
	afterSetGovernorToUserspace();
	setCPUFreq(0);
}

CPUOdroid::~CPUOdroid(){
}

void CPUOdroid::initCPUFreqValues(){

	char freqLongString[FILE_BUFFER_SIZE];

	getStringFromFile(FILE_CPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Frequencies available: %s", freqLongString);

	char * freqString;

	long currentFreq;

    freqString = strtok (freqLongString," ");
    currentFreq = atol(freqString);

    cpuFreqs.clear();
    cpuFreqs.push_back(currentFreq);

	while ((freqString = strtok (NULL, " ")) != NULL){
		__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Frequencies available: %s", freqString);
	   currentFreq = atol(freqString);
	   cpuFreqs.push_back(currentFreq);
	 }


	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Frequencies init end");

}

