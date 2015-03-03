/*
 * DroidPwMgmtCommon.c
 *
 *  Created on: 19 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "DroidPwMgmt.h"

extern ProcessorTime            procTimeOld;
int                             pwMgmtInitOK = false;

void checkInitialized(void)
{
    assert(pwMgmtInitOK);
}

void initDroidPwMgmt(void)
{
    system("echo 1 > /sys/bus/i2c/drivers/INA231/4-0040/enable");
    system("echo 1 > /sys/bus/i2c/drivers/INA231/4-0041/enable");
    system("echo 1 > /sys/bus/i2c/drivers/INA231/4-0044/enable");
    system("echo 1 > /sys/bus/i2c/drivers/INA231/4-0045/enable");
    system("chmod 777 " CPU_FILE_GOVERNOR);

    pwMgmtInitOK = true;
    initGPUCounters();

    procTimeOld = getProcTime();
}

void deinitDroidPwMgmt(void)
{
    deinitGPUCounters();
}

void setValueToFile(const char * const fileName, float value)
{
    FILE    *filePtr;
    char    commandStr[SPARE_COMMAND_LENGTH] = "chmod 777 ";

    system(strcat(commandStr, fileName));
    filePtr = fopen(fileName, "w");
    fprintf(filePtr, "%f", value);
    fclose (filePtr);
}

float getValueFromFile(const char * const fileName)
{
    float   result;
    char    newLine[TEXT_BUFFER_DIM];
    FILE    *filePtr;

    filePtr = fopen(fileName, "r");
    fgets(newLine, TEXT_BUFFER_DIM, filePtr);
    fclose(filePtr);

    result = atof(newLine);

    return result;
}

void setFrequencies(int cpuFrequency, int gpuFrequency)
{
    if(cpuFrequency != getCpuFreq())
    {
        setCpuFrequency(cpuFrequency);
    }

    if(gpuFrequency != getCpuFreq())
    {
        setGpuFrequency(gpuFrequency);
    }
}

void forceFrequencies(int cpuFrequency, int gpuFrequency)
{
    setFrequencies(cpuFrequency, gpuFrequency);
    usleep(100 * 1000);
    setFrequencies(cpuFrequency, gpuFrequency);
    usleep(100 * 1000);
    setFrequencies(cpuFrequency, gpuFrequency);
}

#if 0
void printProcessorTime(ProcessorTime *processorTime)
{
    printf("# Processor time #\n");
    printf("utime: %f", processorTime->utime);
    printf("stime: %f", processorTime->stime);

    printf("cputime: %f, cpuIdleTime: %f\n", processorTime->cputime,
           processorTime->cpuIdleTime);

    for (int i = 0; i < CORE_COUNT; i++)
    {
        printf("Core %d | cputime: %f, cpuIdleTime: %f\n", i,
               processorTime->cpuTimePerCore[i],
               processorTime->cpuIdleTimePerCore[i]);
    }
}

void printCpuUtilization(CPUUtilization *cpuUtilization)
{
    printf("# CPU utilization\n");

    printf("Total: %f\n", cpuUtilization->total);

    for (int i = 0; i < CORE_COUNT; i++)
    {
        printf("Core %d: %f\n", i, cpuUtilization->perCore[i]);
    }

    printf("Maximum for core %d: %f\n", cpuUtilization->idxMax,
           cpuUtilization->perCore[cpuUtilization->idxMax]);
}
#endif
