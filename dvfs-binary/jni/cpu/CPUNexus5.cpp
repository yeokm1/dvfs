/*
 * CPUNexus5.cpp
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#include <cpu/CPUNexus5.h>
#include <stdlib.h>
#include "IOStuff.h"

#define FILE_CPU_AVAILABLE_FREQS "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"
#define FILE_CPU_SET_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define FILE_CPU_SET_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

//#define COMMAND_ENABLE_FILE_SYSTEM_WRITE "mount -o remount,rw /system"

#define COMMMAND_STOP_MPDECISION "stop mpdecision"
//#define COMMAND_RENAME_MPDECISON "mv /system/bin/mpdecision /system/bin/mpdecision-rm"

#define COMMMAND_START_MPDECISION "start mpdecision"
//#define COMMAND_REVERT_MPDECISON "mv /system/bin/mpdecision-rm /system/bin/mpdecision"

#define LOWEST_FREQ_POSITION 0 //0.3 Ghz
#define HIGHEST_FREQ_POSITION 7 //1.2 Ghz

#define SLEEP_BEFORE_ENABLE_MPDECISION 5000000 //3s

#define CLASSNAME "CPUNexus5"

CPUNexus5::CPUNexus5() {
	initCPUFreqValues();
}

CPUNexus5::~CPUNexus5() {
}


void CPUNexus5::initCPUFreqValues(){

	char freqLongString[FILE_BUFFER_SIZE];
	getStringFromFile(FILE_CPU_AVAILABLE_FREQS, freqLongString, FILE_BUFFER_SIZE);

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
			cpuFreqs.push_back(currentFreq);
		}
	}


}

void CPUNexus5::setGovernorToUserspaceAndInit(){
	disableMPDecision();
	writeStringToFile(FILE_CPU_SCALING_GOVERNER, USERSPACE);
	writeValueToFile(FILE_CPU_SET_MIN_FREQ, cpuFreqs[0]);
	writeValueToFile(FILE_CPU_SET_MAX_FREQ, cpuFreqs[cpuFreqs.size() - 1]);
	enableAllCPUCores();
	setCPUFreq(0);
}
void CPUNexus5::setGovernorToOndemandAndRevert(){
//	writeValueToFile(FILE_CPU_SET_MIN_FREQ, cpuFreqs[0]);
//	writeValueToFile(FILE_CPU_SET_MAX_FREQ, cpuFreqs[cpuFreqs.size() - 1]);
//	writeStringToFile(FILE_CPU_SCALING_GOVERNER, ONDEMAND);
//	enableMPDecision();
}

void CPUNexus5::enableMPDecision(){
	runCommand(COMMMAND_START_MPDECISION);
}

void CPUNexus5::disableMPDecision(){
	runCommand(COMMMAND_STOP_MPDECISION);
}



