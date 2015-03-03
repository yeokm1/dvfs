/*
 * DroidPwMgmtCPU.c
 *
 *  Created on: 19 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "DroidPwMgmt.h"

//#include "PVRScopeComms.h"
//#include "PVRScopeStats.h"

const int gpuFreqs[] = {177, 266, 350, 480, 532, 640};
const float gpuVoltages[] = {0.9, 0.9, 0.9, 1.0, 1.0, 1.1};
const int gpuFreqsCount = sizeof(gpuFreqs) / sizeof(int);
const int gpuVoltagesCount = sizeof(gpuVoltages) / sizeof(float);

int isGpuFreqValid(int *gpuFreq)
{
    int isValid = false;

    for(int gpuFreqIdx = 0; gpuFreqIdx < gpuFreqsCount; gpuFreqIdx++)
    {
        if(*gpuFreq == gpuFreqs[gpuFreqIdx])
        {
            isValid = true;
            break;
        }
        else if(*gpuFreq == 1000 * gpuFreqs[gpuFreqIdx])
        {
            *gpuFreq *= 1000;
            isValid = true;
            break;
        }
    }

    return isValid;
}

void setGpuFrequency(int gpuFrequency)
{
    FILE *minFrequencyFile;
    FILE *maxFrequencyFile;

    if(true == isGpuFreqValid(&gpuFrequency))
    {
        minFrequencyFile = fopen(GPU_FILE_SET_MIN_FREQ, "w");
        fprintf(minFrequencyFile, "%d", gpuFrequency);
        fclose(minFrequencyFile);

        maxFrequencyFile = fopen(GPU_FILE_SET_MAX_FREQ, "w");
        fprintf(maxFrequencyFile, "%d", gpuFrequency);
        fclose(maxFrequencyFile);
    }
    else
    {
        printf("Invalid GPU Frequency: %d.\n", gpuFrequency);
    }
}

int getGpuFreq(void)
{
    int result;

    result = (int)(getValueFromFile(GPU_FILE_FREQ_INFO));

    return result;
}

GPUPower getGpuPower(void)
{
    GPUPower result;

    memset(&result, 0, sizeof(GPUPower));
    result.value = getValueFromFile(GPU_FILE_POWER);

    return result;
}

GPUCurrent getGpuCurrent(void)
{
    GPUCurrent gpuCurrent;

    memset(&gpuCurrent, 0, sizeof(GPUCurrent));
    gpuCurrent.value = getValueFromFile(GPU_FILE_CURRENT);

    return gpuCurrent;
}

GPUUtilization getGpuUtilization(void)
{
    GPUUtilization              gpuUtilization;

    memset(&gpuUtilization, 0, sizeof(GPUUtilization));
    gpuUtilization.total = getValueFromFile(GPU_FILE_UTILIZATION) / 2.56;

    return gpuUtilization;
}

FPS getFPS(float timeIntervalMicroseconds)
{
    FPS                         fps;
    FILE                        *pp;
    char                        line[128];
    char                        finishTime[128][32];
    float                       timeIntervalNanoseconds;
    float                       frameCount = 0;
    char                        *finishFrameTime;

    memset(&fps, 0, sizeof(FPS));

    timeIntervalNanoseconds = timeIntervalMicroseconds * 1000;

    pp = popen("dumpsys SurfaceFlinger --latency SurfaceView", "r");

    fgets(line, sizeof(line), pp);

    for (int i = 0; i < 128; i++)
    {
        fgets(line, sizeof line, pp);

        strtok(line, "\t");
        strtok(NULL, "\t");
        finishFrameTime = strtok(NULL, "\t");
        strcpy(finishTime[i], finishFrameTime);
    }

    pclose(pp);

    atof(finishTime[127]);

    for (int i = 0; i < 128; i++)
    {
        if ((atof(finishTime[127]) - atof(finishTime[i]))
                <= timeIntervalNanoseconds)
        {
            frameCount = frameCount + 1.0;
        }
    }

    fps.value = (frameCount / timeIntervalNanoseconds) * 1000000000;

    fps.value = (fps.value <= 60) ? fps.value : 60;

    return fps;
}

FPS getFps(void)
{
    return getFPS(1000000);
}


unsigned int uActiveGroup;
unsigned int uActiveGroupSelect;
struct SPVRScopeImplData *psData;
struct SPVRScopeCounterDef *psCounters;
unsigned int uCounterNum;
int bActiveGroupChanged;
//struct SPVRScopeCounterReading sReading;
unsigned int sampleRate = 100;

// bool PSInit(struct SPVRScopeImplData **ppsPVRScopeData,
//             struct SPVRScopeCounterDef **ppsCounters,
//             struct SPVRScopeCounterReading* const psReading,
//             unsigned int* const pnCount
// )
// {
//     enum EPVRScopeInitCode eInitCode = PVRScopeInitialise(ppsPVRScopeData);

//     if(ePVRScopeInitCodeOk != eInitCode)
//     {
//         printf("Error: failed to initialise services connection.\n");
//         *ppsPVRScopeData = NULL;
//         exit(1);
//     }

//     if (PVRScopeGetCounters(*ppsPVRScopeData, pnCount, ppsCounters, psReading))
//     {
//         //printf("Total counters enabled: %d.", *pnCount);
//     }

//     return true;
// }

void initGPUCounters(void)
{
//    if (!PSInit(&psData, &psCounters, &sReading, &uCounterNum))
//    {
//        printf("Error initializing PVRScope.\n");
//        exit(1);
//    }
//
////     printf("Find below the list of counters (%d):\n", uCounterNum);
////
////     for(int i = 0; i < uCounterNum; ++i)
////     {
////         printf("Group %d %s\n", psCounters[i].nGroup, psCounters[i].pszName);
////     }
//
//    // Step 3. Set the active group to 0
//    bActiveGroupChanged = true;
//    uActiveGroupSelect = 0;
}

void refreshGPUCounters(void)
{

    if(bActiveGroupChanged)
    {
        uActiveGroup = uActiveGroupSelect;
    }
    else
    {
        uActiveGroup = 0xffffffff;
    }
//     unsigned int sampleRate = 100;
//
//     for(int k = 0; k < sampleRate; k++)
//     {
        PVRScopeReadCountersThenSetGroup(psData, NULL, 0, uActiveGroup);
//         usleep(10 * 1000);
//     }
}

// GPUUtilization getDetailedGPUUtilization(void)
// {
//     GPUUtilization          gpuUtilization;
//     unsigned int            index1 = 0;
//     unsigned int            i = 0;
//     struct timeval          tv;
//     unsigned long           time_in_mill = 0;
//
//     memset(&gpuUtilization, 0, sizeof(GPUUtilization));
//
//     gettimeofday(&tv, NULL);
//     time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
//
//     if(PVRScopeReadCountersThenSetGroup(psData, &sReading, time_in_mill *
//         1000, uActiveGroup))
//     {
//         for(int j = 0; j < uCounterNum; ++j)
//         {
//             if(j < sReading.nValueCnt)
//             {
//                 if (strcmp(psCounters[j].pszName, "GPU task load: 3D core") == 0)
//                 {
//                     gpuUtilization.util3D = sReading.pfValueBuf[j];
//                 }
//                 else if (strcmp(psCounters[j].pszName, "GPU task load: TA core") == 0)
//                 {
//                     gpuUtilization.utilTA = sReading.pfValueBuf[j];
//                 }
//                 else if (strcmp(psCounters[j].pszName, "Vertices per second") == 0)
//                 {
//                     gpuUtilization.verticesPerSecond = sReading.pfValueBuf[j];
//                 }
//             }
//         }
//     }
//
//     return gpuUtilization;
// }

// GPUUtilization getDetailedGPUUtilization(void)
// {
//     GPUUtilization          gpuUtilization;
//     unsigned int            index1 = 0;
//     unsigned int            i = 0;
//     struct timeval          tv;
//     unsigned long           time_in_mill = 0;

//     memset(&gpuUtilization, 0, sizeof(GPUUtilization));

//     if(bActiveGroupChanged)
//     {
//         uActiveGroup = uActiveGroupSelect;
//     }
//     else
//     {
//         uActiveGroup = 0xffffffff;
//     }

//     unsigned int sampleRate = 100;

//     for(int k = 0; k < sampleRate; k++)
//     {
//         PVRScopeReadCountersThenSetGroup(psData, NULL, 0, uActiveGroup);
//         usleep(10 * 1000);
//     }

//     gettimeofday(&tv, NULL);
//     time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

//     if(PVRScopeReadCountersThenSetGroup(psData, &sReading, time_in_mill *
//         1000, uActiveGroup))
//     {
//         for(int j = 0; j < uCounterNum; ++j)
//         {
//             if(j < sReading.nValueCnt)
//             {
//                 if (strcmp(psCounters[j].pszName, "GPU task load: 3D core") == 0)
//                 {
//                     gpuUtilization.util3D = sReading.pfValueBuf[j];
//                 }
//                 else if (strcmp(psCounters[j].pszName, "GPU task load: TA core") == 0)
//                 {
//                     gpuUtilization.utilTA = sReading.pfValueBuf[j];
//                 }
//                 else if (strcmp(psCounters[j].pszName, "Vertices per second") == 0)
//                 {
//                     gpuUtilization.verticesPerSecond = sReading.pfValueBuf[j];
//                 }
//             }
//         }
//     }

//     return gpuUtilization;
// }

void deinitGPUCounters(void)
{
    // Step 5. Shutdown PVRScopeStats
   // PVRScopeDeInitialise(&psData, &psCounters, &sReading);
}

