/*
 * CPU.h
 *
 *  Created on: 28 Jan, 2015
 *      Author: yeokm1
 */

#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#define FILE_CPU_UTIL = "/proc/stat"
#define FILE_CPU_SCALING_FREQ  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define FILE_CPU_SCALING_GOVERNER  "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

class CPU {
public:
	CPU();
	virtual ~CPU();
};

#endif /* CPU_CPU_H_ */
