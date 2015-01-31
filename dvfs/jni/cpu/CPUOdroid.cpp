/*
 * CPUOdroid.cpp
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#include <cstdlib>
#include <cpu/CPUOdroid.h>

#define FILE_CPU_AVAILABLE_FREQS "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table"
#define LOWEST_FREQ_POSITION 7 //0.6 GHz
#define HIGHEST_FREQ_POSITION 17 //1.6 GHz

CPUOdroid::CPUOdroid() {
	CPU();
}

CPUOdroid::~CPUOdroid() {
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
	   cpuFreqs = realloc (cpuFreqs, numCPUFreqs * sizeof(int));
	   cpuFreqs[numCPUFreqs - 1] = currentFreq;
	 }


	 free(freeLongString);

}

