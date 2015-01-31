/*
 * CPU.h
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */


#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#include <cstring>
#include <vector>
#define CPU_CPU_H_
#define NUM_CORES 4

#define FILE_CPU_UTIL = "/proc/stat"
#define FILE_CPU_SCALING_FREQ  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define FILE_CPU_SCALING_GOVERNER  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

using std::vector;

class CPU {
public:
	CPU();
	virtual ~CPU();
	void setCPUFreq(int position);
	vector<long> getCPUFreqs();

protected:
	double prevCoreLoad[NUM_CORES];
	double prevCoreTotal[NUM_CORES];
	vector<long> cpuFreqs;
	int cpuFreqPosition;

	void initCPUFreqs();
	void setGovernorToUserspace();
	void initCPUFreqValues();
	void afterSetGovernorToUserspace();
};



#endif /* CPU_CPU_H_ */
