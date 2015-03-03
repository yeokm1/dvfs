/*
 * DroidPwMgmtCPU.c
 *
 *  Created on: 19 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "DroidPwMgmt.h"

#define CPU_INFO_EACH_CORE

extern int      gGamePID;
ProcessorTime   procTimeOld;

const int cpuFreqs[] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};
const float cpuVoltages[] = {0.9, 0.9, 0.9, 1.0, 1.0, 1.0, 1.1, 1.1, 1.1};
const int cpuFreqsCount = sizeof(cpuFreqs) / sizeof(int);
const int cpuVoltagesCount = sizeof(cpuVoltages) / sizeof(float);

int isCpuFreqValid(int *cpuFreq)
{
    int isValid = false;

    for(int cpuFreqIdx = 0; cpuFreqIdx < cpuFreqsCount; cpuFreqIdx++)
    {
        if(*cpuFreq == cpuFreqs[cpuFreqIdx])
        {
            *cpuFreq *= 1000;
            isValid = true;
            break;
        }
        else if(*cpuFreq == 1000 * cpuFreqs[cpuFreqIdx])
        {
            isValid = true;
            break;
        }
    }

    return isValid;
}

void setCpuFrequency(int cpuFrequency)
{
    FILE                        *governorFile;
    FILE                        *frequencyFile;
    const char                  *governor = "userspace";

    if(true == isCpuFreqValid(&cpuFrequency))
    {
        //printf("Changing CPU Frequency to %d\n", cpuFrequency);

        governorFile = fopen(CPU_FILE_GOVERNOR, "w");
        fprintf(governorFile, "%s", governor);
        fclose (governorFile);

        setValueToFile(CPU_FILE_SET_FREQ, cpuFrequency);
    }
    else
    {
        printf("Invalid CPU Frequency: %d.\n", cpuFrequency);
    }
}

void setCpuFreqLimits(int minCpuFreq, int maxCpuFreq)
{
    if((true == isCpuFreqValid(&minCpuFreq)) &&
        (true == isCpuFreqValid(&maxCpuFreq)))
    {
        setValueToFile(CPU_FILE_MIN_FREQ, minCpuFreq);
        setValueToFile(CPU_FILE_MAX_FREQ, maxCpuFreq);
    }
}

int getCpuFreq(void)
{
    int result;

    result = (int)(getValueFromFile(CPU_FILE_FREQ_INFO) / 1000);

    return result;
}

CPUPower getA15Power(void)
{
    CPUPower result;

    memset(&result, 0, sizeof(CPUPower));

    //printf("CPU_FILE_POWER: %s\n", CPU_FILE_POWER);
    result.value = getValueFromFile(CPU_FILE_POWER);

    return result;
}

CPUCurrent getA15CurrentSynced(void)
{
    static char     oldLine[TEXT_BUFFER_DIM] = "default_string";
    char            newLine[TEXT_BUFFER_DIM];
    CPUCurrent      cpuCurrent;
    FILE            *powerFile;

    checkInitialized();

    memset(&cpuCurrent, 0, sizeof(CPUCurrent));

    powerFile = fopen(CPU_FILE_CURRENT, "r");
    fgets(newLine, sizeof(newLine), powerFile);
    fclose(powerFile);

    if(IDENTICAL != strcmp(oldLine, newLine))
    {
        cpuCurrent.changed = true;
        cpuCurrent.value = atof(newLine);
        strcpy(oldLine, newLine);
    }
    else
    {
        cpuCurrent.changed = false;
    }

    return cpuCurrent;
}

CPUCurrent getA15Current(void)
{
    CPUCurrent  cpuCurrent;

    checkInitialized();

    memset(&cpuCurrent, 0, sizeof(CPUCurrent));
    cpuCurrent.value = getValueFromFile(CPU_FILE_CURRENT);

    return cpuCurrent;
}

static ProcessorTime parseProcessorInfo(char *cpuData)
{
    ProcessorTime   coreTime;
    char            *token;
    float           atomicTime = 0.0;
    int             currTokenIdx = 0;

    memset(&coreTime, 0, sizeof(ProcessorTime));

    token = strtok(cpuData, " ");

    currTokenIdx = 0;
    coreTime.cputime = 0;
    coreTime.cpuIdleTime = 0;

    /// ignore first term
    token = strtok(NULL, " ");
    currTokenIdx++;

    while (NULL != token)
    {
        atomicTime = atof(token);
        coreTime.cputime += atomicTime;

        if (4 == currTokenIdx)
        {
            coreTime.cpuIdleTime = atomicTime;
        }

        currTokenIdx++;
        token = strtok(NULL, " ");
    }

    return coreTime;
}

static ProcessorTime getProcessTime(int processID)
{
    ProcessorTime   procTime;
    char            pidProcFileName[SPARE_COMMAND_LENGTH];
    FILE            *pidProcFile;
    char            data[TEXT_BUFFER_DIM];
    char            *utimeStr;
    char            *stimeStr;

    memset(&procTime, 0, sizeof(ProcessorTime));

    sprintf(pidProcFileName, "/proc/%d/stat", processID);

    pidProcFile = fopen(pidProcFileName, "r");
    fgets(data, TEXT_BUFFER_DIM, pidProcFile);
    fclose(pidProcFile);

    strtok(data, " ");

    for (int i = 0; i < 12; i++)
    {
        strtok(NULL, " ");
    }

    utimeStr = strtok(NULL, " ");
    stimeStr = strtok(NULL, " ");

    procTime.utime = atof(utimeStr);
    procTime.stime = atof(stimeStr);

    return procTime;
}

ProcessorTime getProcTime(void)
{
    ProcessorTime   procTime;
    ProcessorTime   procTimeCore;
    char            data[TEXT_BUFFER_DIM];
    FILE            *cpuProcFile;

    checkInitialized();

    memset(&procTime, 0, sizeof(ProcessorTime));

    cpuProcFile = fopen("/proc/stat", "r");

    fgets(data, TEXT_BUFFER_DIM, cpuProcFile);
    procTime = parseProcessorInfo(data);

    #ifdef CPU_INFO_EACH_CORE
    for (int coreIdx = 0; coreIdx < CORE_COUNT; coreIdx++)
    {
        fgets(data, TEXT_BUFFER_DIM, cpuProcFile);
        procTimeCore = parseProcessorInfo(data);
        procTime.cpuTimePerCore[coreIdx] = procTimeCore.cputime;
        procTime.cpuIdleTimePerCore[coreIdx] = procTimeCore.cpuIdleTime;
    }
    #endif

    fclose(cpuProcFile);

    return procTime;
}

CPUUtilization getCpuUtilization(void)
{
    CPUUtilization          cpuUtilization;
    float                   deltaCPUTime = 0.0;
    float                   deltaCPUIdleTime = 0.0;
    ProcessorTime           procTimeNew;
    ProcessorTime           processTime;

    memset(&cpuUtilization, 0, sizeof(CPUUtilization));

    procTimeNew = getProcTime();

    deltaCPUTime = procTimeNew.cputime - procTimeOld.cputime;

    if (deltaCPUTime > EPSILON)
    {
        #ifdef CPU_INFO_PROCESS
        processTime = getProcessTime(gGamePID);
        procTimeNew.utime = processTime.utime;
        procTimeNew.stime = processTime.stime;

        cpuUtilization.perProcess = 100 * (procTimeNew.utime + procTimeNew.stime
            - procTimeOld.utime - procTimeOld.stime) / deltaCPUTime;
        #endif

        deltaCPUIdleTime = procTimeNew.cpuIdleTime - procTimeOld.cpuIdleTime;

        cpuUtilization.total = 100 * (deltaCPUTime - deltaCPUIdleTime)
            / deltaCPUTime;

        #ifdef CPU_INFO_EACH_CORE
        for (int coreIdx = 0; coreIdx < CORE_COUNT; coreIdx++)
        {
            deltaCPUTime = procTimeNew.cpuTimePerCore[coreIdx]
                - procTimeOld.cpuTimePerCore[coreIdx];

            if (deltaCPUTime > EPSILON)
            {
                deltaCPUIdleTime = procTimeNew.cpuIdleTimePerCore[coreIdx]
                    - procTimeOld.cpuIdleTimePerCore[coreIdx];

                cpuUtilization.perCore[coreIdx] = 100 * (deltaCPUTime -
                    deltaCPUIdleTime) / deltaCPUTime;
            }

            if (cpuUtilization.perCore[coreIdx] > cpuUtilization.utilMax)
            {
                cpuUtilization.utilMax = cpuUtilization.perCore[coreIdx];
                cpuUtilization.idxMax = coreIdx;
            }
        }
        #endif
    }

    procTimeOld = procTimeNew;

    return cpuUtilization;
}
