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

#define CLASSNAME "CPUOdroid"

CPUOdroid::CPUOdroid(){
	CPU();
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Start");

	initCPUFreqValues();
	setGovernorToUserspace();
	afterSetGovernorToUserspace();
	setCPUFreq(0);
}

CPUOdroid::~CPUOdroid(){
}

void CPUOdroid::initCPUFreqValues(){


	char * freqLongString = getStringFromFile(FILE_CPU_AVAILABLE_FREQS);

	char * freqString;

	long currentFreq;

    freqString = strtok (freqLongString," ");
    currentFreq = atol(freqString);


    numCPUFreqs = 1;
    cpuFreqs[0] = currentFreq;

	while (freqString != NULL){
	   numCPUFreqs++;

	   freqString = strtok (NULL, " ");
	   currentFreq = atol(freqString);
	   realloc (cpuFreqs, numCPUFreqs * sizeof(int));
	   cpuFreqs[numCPUFreqs - 1] = currentFreq;
	 }


	 free(freqLongString);

}

