/*
 * PowerManagement.h
 *
 *  Created on: 15 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

#include "CommonUtilities.h"
#include "DroidPwMgmt.h"

//#define DEBUG_APP                   1
#define LINE_BUFFER_LENGTH          512
#define ONE_SECOND                  (1000 * 1000)
#define GAME_CONST_DELAY            (1 * ONE_SECOND)
#define SAMPLE_COUNT                INT_MAX
#define MIN_POSSIBLE_FPS            25
#define MAX_POSSIBLE_FPS            62

#ifdef DEBUG_APP
#define LEAVE_TRACE printf("%s\n", __FUNCTION__);
#else
#define LEAVE_TRACE
#endif

#define SET_FREQS(newFc, newFg) \
{ \
    Fc = newFc; \
    Fg = newFg; \
    forceFrequencies(Fc, Fg); \
    Fi = FREQ_INDEX_COMB(Fc, Fg); \
}

typedef enum{
    MEASURE_IDX_UC = 0,
    MEASURE_IDX_UG,
    MEASURE_IDX_FPS,
    MEASURE_IDX_LAST
} MeasureIdx;

typedef enum{
    MEASURE_IDX_IC = 3,
    MEASURE_IDX_IG,
    MEASURE_IDX_PC,
    MEASURE_IDX_PG,
    MEASURE_IDX_LAST_ALL
} MeasureAuxIdx;

typedef enum{
    VAR_IDX_CONST = 0,
    VAR_IDX_FC,
    VAR_IDX_FG,
    VAR_IDX_LAST
} VariablesConsidered;

typedef struct{
//     int                 Fc;
//     int                 Fg;
//     int                 combIdx;
    int                 count;
    Element             *values;
} CounterInfo;

typedef struct{
    MeasureIdx          what;
    Element             min;
    Element             max;
} Requirements;

extern int              Fc;
extern int              Fg;
extern int              Fi;
extern Requirements     req;
extern int              sampleCount;

/* PowerManagementCore.c */

void            savePower(void);

/* PowerManagementAux.c */

void            updateCounterInfo(CounterInfo *counterInfo);
void            printCounterInfo(CounterInfo c);

#endif /* POWER_MANAGEMENT_H_ */
