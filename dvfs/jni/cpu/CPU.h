/*
 * CPU.h
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */


#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#include <cstring>
#define CPU_CPU_H_
#define NUM_CORES 4

#define FILE_CPU_UTIL = "/proc/stat"
#define FILE_CPU_SCALING_FREQ  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define FILE_CPU_SCALING_GOVERNER  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

class CPU {
public:
	CPU();
	virtual ~CPU();
	void setCPUFreq(int position);
	long * getCPUFreqs();
	int getNumCPUFreqs();

protected:
	double prevCoreLoad[NUM_CORES];
	double prevCoreTotal[NUM_CORES];
	long * cpuFreqs = NULL;
	int cpuFreqPosition;
	int numCPUFreqs;

	void initCPUFreqs();
	void setGovernorToUserspace();
	void initCPUFreqValues();
	void afterSetGovernorToUserspace();
};



#endif /* CPU_CPU_H_ */
