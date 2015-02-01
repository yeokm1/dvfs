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

using std::vector;

class CPU {

public:
	CPU();
	virtual ~CPU();
	void setCPUFreq(int position);
	vector<long> getCPUFreqs(){ return cpuFreqs; };
	int getCpuFreqPosition(){ return cpuFreqPosition; }
	void getCPUUtil(float * util);
	float getUtilisationOfHighestCore();

protected:
	long prevCoreLoad[NUM_CORES];
	long prevCoreTotal[NUM_CORES];
	vector<long> cpuFreqs;
	int cpuFreqPosition;
	void setGovernorToUserspace();
	virtual void initCPUFreqValues();
};



#endif /* CPU_CPU_H_ */
