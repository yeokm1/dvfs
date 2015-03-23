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
#define FILE_BUFFER_SIZE 300

#define ONDEMAND "ondemand"
#define USERSPACE "userspace"

#define FILE_CPU_SCALING_GOVERNER  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

using std::vector;

class CPU {

public:
	CPU();
	virtual ~CPU();
	void setCPUFreq(int position);
	vector<long> getCPUFreqs(){ return cpuFreqs; };
	int getCpuFreqPosition(){ return cpuFreqPosition; }
	void getCPUUtil(float * util);
	float getAverageCPUUtilisation();
	float getUtilisationOfHighestCore();
	virtual void setGovernorToUserspaceAndInit();
	virtual void setGovernorToOndemandAndRevert();
	void enableAllCPUCores();

protected:
	long prevLoad;
	long prevTotal;
	long prevCoreLoad[NUM_CORES];
	long prevCoreTotal[NUM_CORES];
	vector<long> cpuFreqs;
	int cpuFreqPosition;
	virtual void initCPUFreqValues();
};



#endif /* CPU_CPU_H_ */
