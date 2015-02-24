/*
 * CPUOdroid.h
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef CPU_CPUODROID_H_
#define CPU_CPUODROID_H_

#include "CPU.h"

class CPUOdroid : public CPU{
public:
	CPUOdroid();
	virtual ~CPUOdroid();
	void setGovernorToUserspaceAndInit();
	void setGovernorToOndemandAndRevert();

private:
	void initCPUFreqValues();
};

#endif /* CPU_CPUODROID_H_ */
