/*
 * CPUOdroid.cpp
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#include <cstdlib>
#include <stack>
#include <android/log.h>
#include "CPUOdroid.h"
#include "IOStuff.h"

#define FILE_CPU_AVAILABLE_FREQS "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table"
#define LOWEST_FREQ_POSITION 7 //0.6 GHz
#define HIGHEST_FREQ_POSITION 16 //1.6 GHz

#define CLASSNAME "CPUOdroid"

CPUOdroid::CPUOdroid(){
	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPUOdroid Start");

	initCPUFreqValues();
	setCPUFreq(0);
}

CPUOdroid::~CPUOdroid(){
}

void CPUOdroid::initCPUFreqValues(){

	char freqLongString[FILE_BUFFER_SIZE];

	getStringFromFile(FILE_CPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Freqs available: %s", freqLongString);

	char * freqString;

	long currentFreq;

    freqString = strtok (freqLongString," ");
    currentFreq = atol(freqString);


    //We use a stack as the frequencies in the file are in descending order
    std::stack<long> cpuFreqsStack;

    cpuFreqsStack.push(currentFreq);

	while ((freqString = strtok (NULL, " ")) != NULL){
	   currentFreq = atol(freqString);
	   if(currentFreq != 0){
		   cpuFreqsStack.push(currentFreq);
	   }
	 }




	cpuFreqs.clear();

	for(int i = 0; !cpuFreqsStack.empty(); i++){
		long freq = cpuFreqsStack.top();
		cpuFreqsStack.pop();
		if(i >= LOWEST_FREQ_POSITION && i <= HIGHEST_FREQ_POSITION){
			__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "CPU Freqs used %ld", freq);
			cpuFreqs.push_back(freq);
		}
	}



}

