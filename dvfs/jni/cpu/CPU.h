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

#define FILE_CPU_UTIL "/proc/stat"
#define FILE_CPU_SCALING_FREQ  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define FILE_CPU_SCALING_GOVERNER  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

using std::vector;

class CPU {

public:
	CPU();
	virtual ~CPU();
	void setCPUFreq(int position);
	vector<long> getCPUFreqs();
	void getCPUUtil(float * util);

protected:
	long prevCoreLoad[NUM_CORES];
	long prevCoreTotal[NUM_CORES];
	vector<long> cpuFreqs;
	int cpuFreqPosition;

	int getCpuFreqPosition(){return cpuFreqPosition;}
	void setGovernorToUserspace();
	virtual void initCPUFreqValues();
};



#endif /* CPU_CPU_H_ */
