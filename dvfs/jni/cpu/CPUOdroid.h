/*
 * CPUOdroid.h
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef CPU_CPUODROID_H_
#define CPU_CPUODROID_H_

#include "CPU.h"

class CPUOdroid : CPU{
public:
	CPUOdroid();
	virtual ~CPUOdroid();

private:
	void initCPUFreqValues();
	void afterSetGovernorToUserspace(){
		//Do nothing
	};
};

#endif /* CPU_CPUODROID_H_ */
