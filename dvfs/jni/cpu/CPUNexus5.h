/*
 * CPUNexus5.h
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#ifndef CPUNEXUS5_H_
#define CPUNEXUS5_H_

#include <cpu/CPU.h>

class CPUNexus5: public CPU {
public:
	CPUNexus5();
	virtual ~CPUNexus5();

private:
	void initCPUFreqValues();
	void afterSetGovernorToUserspace();
};

#endif /* CPUNEXUS5_H_ */
