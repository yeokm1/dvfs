/*
 * DroidPwMgmt.h
 *
 *  Created on: 01 Sep, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#ifndef DROID_PW_MGMT_ESSENTIALS_H_
#define DROID_PW_MGMT_ESSENTIALS_H_

#define CPU_FILE_GOVERNOR       "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU_FILE_SET_FREQ       "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define CPU_FILE_FREQ_INFO      "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"
#define CPU_FILE_MIN_FREQ       "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU_FILE_MAX_FREQ       "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

#define CPU_FILE_VOLTAGE        "/sys/bus/i2c/drivers/INA231/4-0040/sensor_V"
#define CPU_FILE_CURRENT        "/sys/bus/i2c/drivers/INA231/4-0040/sensor_A"
#define CPU_FILE_POWER          "/sys/bus/i2c/drivers/INA231/4-0040/sensor_W"

#define GPU_FILE_CURRENT        "/sys/bus/i2c/drivers/INA231/4-0044/sensor_A"
#define GPU_FILE_POWER          "/sys/bus/i2c/drivers/INA231/4-0044/sensor_W"
#define GPU_FILE_SET_MIN_FREQ   "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_min_lock"
#define GPU_FILE_SET_MAX_FREQ   "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_max_lock"
#define GPU_FILE_FREQ_INFO      "/sys/module/pvrsrvkm/parameters/sgx_gpu_clk"
#define GPU_FILE_UTILIZATION    "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization"

#endif /* DROID_PW_MGMT_ESSENTIALS_H_ */
