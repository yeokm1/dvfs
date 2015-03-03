/*
 * DroidPwMgmt.h
 *
 *  Created on: 19 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#ifndef DROID_PW_MGMT_H_
#define DROID_PW_MGMT_H_

#include "DroidPwMgmtEssentials.h"
#include "CommonUtilities.h"

//#define CPU_INFO_EACH_CORE
//#define CPU_INFO_PROCESS
#define PIPE_TEXT_DIM               64
#define TEXT_BUFFER_DIM             1024
#define SPARE_COMMAND_LENGTH        1024
#define CORE_COUNT                  4

#define FREQ_INDEX_CPU(cpuFreq) (cpuFreq / 100 - 8)
#define FREQ_INDEX_GPU(gpuFreq) (gpuFreq / 100 - 1)

#define FREQ_INDEX_COMB(cpuFreq, gpuFreq) \
    (FREQ_INDEX_CPU(cpuFreq) * gpuFreqsCount + FREQ_INDEX_GPU(gpuFreq))

#define FREQ_COMB_INDEX_CPU(index) \
    (index / gpuFreqsCount)

#define FREQ_COMB_INDEX_GPU(index) \
    (index - FREQ_COMB_INDEX_CPU(index) * gpuFreqsCount)

#define DECREASE_FC(cpuFreq) (cpuFreq = ((FREQ_INDEX_CPU(cpuFreq) > 0) ? \
    cpuFreqs[FREQ_INDEX_CPU(cpuFreq) - 1] : cpuFreq))
#define DECREASE_FG(gpuFreq) (gpuFreq = ((FREQ_INDEX_GPU(gpuFreq) > 0) ? \
    gpuFreqs[FREQ_INDEX_GPU(gpuFreq) - 1] : gpuFreq))

#define INCREASE_FC(cpuFreq) (cpuFreq = ((FREQ_INDEX_CPU(cpuFreq) < cpuFreqsCount - 1) ? \
    cpuFreqs[FREQ_INDEX_CPU(cpuFreq) + 1] : cpuFreq))
#define INCREASE_FG(gpuFreq) (gpuFreq = ((FREQ_INDEX_GPU(gpuFreq) < gpuFreqsCount - 1) ? \
    gpuFreqs[FREQ_INDEX_GPU(gpuFreq) + 1] : gpuFreq))

#define LOWER_FC_IDX(cpuFreqIdx) (((cpuFreqIdx) > 0) ? (cpuFreqIdx - 1) : (cpuFreqIdx))
#define LOWER_FG_IDX(gpuFreqIdx) (((gpuFreqIdx) > 0) ? (gpuFreqIdx - 1) : (gpuFreqIdx))

#define HIGHER_FC_IDX(cpuFreqIdx) (((cpuFreqIdx) < (cpuFreqsCount - 1)) ? \
    (cpuFreqIdx + 1) : (cpuFreqIdx))
#define HIGHER_FG_IDX(gpuFreqIdx) (((gpuFreqIdx) < (gpuFreqsCount - 1)) ? \
    (gpuFreqIdx + 1) : (gpuFreqIdx))

#define LOWER_FC(cpuFreq) (cpuFreqs[LOWER_FC_IDX(cpuFreq)])
#define LOWER_FG(gpuFreq) (gpuFreqs[LOWER_FG_IDX(gpuFreq)])

#define HIGHER_FC(cpuFreq) (cpuFreqs[HIGHER_FC_IDX(cpuFreq)])
#define HIGHER_FG(gpuFreq) (gpuFreqs[HIGHER_FG_IDX(gpuFreq)])

#define VOLTAGE_CPU(cpuFreq) (cpuVoltages[FREQ_INDEX_CPU(cpuFreq)])
#define VOLTAGE_GPU(gpuFreq) (gpuVoltages[FREQ_INDEX_GPU(gpuFreq)])


typedef struct
{
    float               utime;
    float               stime;
    float               cputime;
    float               cpuIdleTime;
    float               cpuTimePerCore[CORE_COUNT];
    float               cpuIdleTimePerCore[CORE_COUNT];
} ProcessorTime;

typedef struct
{
    float               total;
    float               perCore[CORE_COUNT];
    float               utilMax;
    int                 idxMax;
    float               perProcess;
} CPUUtilization;

typedef struct
{
    int                 changed;
    float               value;
} CPUCurrent;

typedef struct
{
    float               value;
} CPUPower;

typedef struct
{
    float               total;
    float               util3D;
    float               utilTA;
    float               verticesPerSecond;
} GPUUtilization;

typedef struct
{
    float               value;
} GPUCurrent;

typedef struct
{
    float               value;
} GPUPower;

typedef struct
{
    float               value;
} FPS;

typedef struct
{
    int                 app;
    int                 total;
    int                 buffers;
} MemoryInfo;

extern const int        cpuFreqsCount;
extern const int        cpuFreqs[];
extern const int        cpuVoltagesCount;
extern const float      cpuVoltages[];

extern const int        gpuFreqsCount;
extern const int        gpuFreqs[];
extern const int        gpuVoltagesCount;
extern const float      gpuVoltages[];

void setValueToFile(const char * const fileName, float value);
void setCpuFreqLimits(int minCpuFreq, int maxCpuFreq);

void                    initDroidPwMgmt(void);
void                    deinitDroidPwMgmt(void);
float                   getValueFromFile(const char * const fileName);
void                    checkInitialized(void);
void                    testCounters(void);

int                     getCpuFreq(void);
CPUPower                getA15Power(void);
CPUCurrent              getA15Current(void);
CPUCurrent              getA15CurrentSynced(void);
CPUUtilization          getCpuUtilization();
ProcessorTime           getProcTime();

int                     getGpuFreq(void);
GPUPower                getGpuPower(void);
GPUCurrent              getGpuCurrent(void);
GPUUtilization          getGpuUtilization(void);
FPS                     getFPS(float timeIntervalMicroseconds);
FPS                     getFps(void);

MemoryInfo              getMemoryInfo(int gamePID);

int                     isCpuFreqValid(int *cpuFreq);
int                     isGpuFreqValid(int *gpuFreq);
void                    setCpuFrequency(int cpuFrequency);
void                    setGpuFrequency(int gpuFrequency);
void                    setFrequencies(int cpuFrequency, int gpuFrequency);
void                    forceFrequencies(int cpuFrequency, int gpuFrequency);

void                    printProcessorTime(ProcessorTime *processorTime);
void                    printCpuUtilization(CPUUtilization *cpuUtilization);

void                    refreshGPUCounters(void);
void                    deinitGPUCounters(void);
//GPUUtilization          getDetailedGPUUtilization(void);

#endif /* DROID_PW_MGMT_H_ */
