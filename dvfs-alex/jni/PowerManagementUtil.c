/*
 * PowerManagementUtil.c
 *
 *  Created on: 09 Sep, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "PowerManagement.h"

void printCounterInfo(CounterInfo c)
{
    printf("\n\n[Counter info (%d, %d){%d}]\n\n", Fc, Fg, Fi);

    for(int counterIdx = 0; counterIdx < c.count; counterIdx++)
    {
        printf("%d. %f\n", counterIdx, c.values[counterIdx]);
    }

    printf("\n");
}

void updateCounterInfo(CounterInfo *counterInfo)
{
    counterInfo->values[MEASURE_IDX_UC]     = getCpuUtilization().total;
    counterInfo->values[MEASURE_IDX_UG]     = getGpuUtilization().total;
    counterInfo->values[MEASURE_IDX_FPS]    = getFps().value;
    counterInfo->values[MEASURE_IDX_IC]     = getA15Current().value;
    counterInfo->values[MEASURE_IDX_IG]     = getGpuCurrent().value;
    counterInfo->values[MEASURE_IDX_PC]     = getA15Power().value;
    counterInfo->values[MEASURE_IDX_PG]     = getGpuPower().value;


    //     counterInfo->values[MEASURE_IDX_UC]     = 25;
    //     counterInfo->values[MEASURE_IDX_UG]     = 85;
    //     counterInfo->values[MEASURE_IDX_FPS]    = 51;
    //     counterInfo->values[MEASURE_IDX_IC]     = 0.5;
    //     counterInfo->values[MEASURE_IDX_IG]     = 0.6;
    //     counterInfo->values[MEASURE_IDX_PC]     = 0.7;
    //     counterInfo->values[MEASURE_IDX_PG]     = 0.7;
}
