/*
 * PowerManagementCore.c
 *
 *  Created on: 09 Sep, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "PowerManagement.h"

int                     Fc = 0;
int                     Fg = 0;
int                     Fi = 0;
Requirements            req;
static CounterInfo      counterInfo;
int                     sampleCount = 0;

static void initCounterInfo(void)
{
    memset(&counterInfo, 0, sizeof(CounterInfo));
    counterInfo.count = MEASURE_IDX_LAST_ALL;
    counterInfo.values = allocVectorClean(counterInfo.count);
}

static void initSavePower(void)
{
    initCounterInfo();
    SET_FREQS(800, 177);
}

static void triggerMonitor(void)
{
    char command[128];

    if(fork() == 0)
    {
        sprintf(command, "sh SampleGame.sh %d big integrated %d %d",
                sampleCount, (int)req.min, (int)req.max);
        system(command);
        exit(0);
    }
}

void savePower(void)
{
    int sample  = 0;
    int newFc   = 0;
    int newFg   = 0;
    float estimFPS = 0.0;

    float Qmin = req.min;
    float Qmax = req.max;
    float UC;
    float UG;
    float PC;
    float PG;
    int c, ci;
    int g, gi;
    float Q, Qreal;
    float OC, OCi;
    float OG, OGi;

    int cLowerLimit = 0;
    int cUpperLimit = 0;
    int gLowerLimit = 0;
    int gUpperLimit = 0;

    initSavePower();

    //printf("Fc\tFg\tRealFPS\tEstimFPS\tPc\tPg\n");
    printf("Fc\tFg\tRealFPS\tPc\tPg\tMinFPS\tMaxFPS\n");

    while(sample < SAMPLE_COUNT)
    {
        if(15 == sample)
        {
            triggerMonitor();
        }

        updateCounterInfo(&counterInfo);

        UC = counterInfo.values[MEASURE_IDX_UC];
        UG = counterInfo.values[MEASURE_IDX_UG];
        Qreal = counterInfo.values[MEASURE_IDX_FPS];
        c = Fc;
        g = Fg;

        PC = (UC * c) / Qreal; // price paid by CPU for 1 FPS
        PG = (UG * g) / Qreal;

        if(Qreal < Qmin)
        {
            Q = Qmax;
            OC = PC * Q; // expected CPU cost sufficient to achieve Q
            OG = PG * Q;

            cLowerLimit = HIGHER_FC_IDX(FREQ_INDEX_CPU(c));
            cUpperLimit = cpuFreqsCount;
            gLowerLimit = HIGHER_FG_IDX(FREQ_INDEX_GPU(g));
            gUpperLimit = gpuFreqsCount;

            for(int idxFc = gLowerLimit; idxFc < gUpperLimit; idxFc++)
            {
                ci = cpuFreqs[idxFc];
                OCi = ci * UC;

                if(OCi >= OC)
                {
                    c = ci;
                    break;
                }
            }

            for(int idxFg = gLowerLimit; idxFg < gUpperLimit; idxFg++)
            {
                gi = gpuFreqs[idxFg];
                OGi = gi * UG;

                if(OGi >= OG)
                {
                    g = gi;
                    break;
                }
            }
        }

        if(Qreal > Qmax)
        {
            Q = Qmin;
            OC = PC * Q; // expected CPU cost sufficient to achieve Q
            OG = PG * Q;

            cLowerLimit = 0;
            cUpperLimit = LOWER_FC_IDX(FREQ_INDEX_CPU(c));
            gLowerLimit = 0;
            gUpperLimit = LOWER_FG_IDX(FREQ_INDEX_GPU(g));

            for(int idxFc = cUpperLimit; idxFc >= cLowerLimit; idxFc--)
            {
                ci = cpuFreqs[idxFc];
                OCi = ci * UC;

                if(OCi >= OC)
                {
                    c = ci;
                    //break;
                }
            }

            for(int idxFg = gUpperLimit; idxFg >= gLowerLimit; idxFg--)
            {
                gi = gpuFreqs[idxFg];
                OGi = gi * UG;

                if(OGi >= OG)
                {
                    g = gi;
                    //break;
                }
            }
        }

        //printf("cLowerLimit: %d, cUpperLimit: %d, gLowerLimit: %d, gUpperLimit: %d\n", cLowerLimit, cUpperLimit, gLowerLimit, gUpperLimit);

        //printf("currFc: %d, currFg: %d, UC: %f, UG: %f, Qreal: %f, OC: %f, OG: %f, PC: %f, PG: %f\n",
               //c, g, UC, UG, Qreal, OC, OG, PC, PG);

        printf("%d\t%d\t%f\t%f\t%f\t%.2f\t%.2f\n", Fc, Fg, counterInfo.values[MEASURE_IDX_FPS],
               counterInfo.values[MEASURE_IDX_PC], counterInfo.values[MEASURE_IDX_PG], req.min, req.max);

        SET_FREQS(c, g);
        usleep(ONE_SECOND);

        //printf("%d\t%d\t%f\t%f\t%f\t%f\n", Fc, Fg);

        sample++;
    }
}
