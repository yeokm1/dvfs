/*
 * GPU.h
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#ifndef GPU_GPU_H_
#define GPU_GPU_H_
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stack>
using std::stack;

#define NO_FPS_CALCULATED -1
#define FILE_BUFFER_SIZE 300

#define FPS_RESULT_LINE_LENGTH 100
#define FPS_COMMAND_ENDING "e"
#define FPS_INTERVAL_NANOS 1000000000
#define MAX_FPS 60

#define LINES_IN_DUMPSYS_TO_IGNORE 60

using std::vector;

class GPU {
public:
	GPU();
	virtual ~GPU();
	vector<long> getGPUFreqs(){ return gpuFreqs; };
	int getGpuFreqPosition(){ return gpuFreqPosition; };
	int getFPS();
	virtual void setGPUFreq(int position);
	virtual float getUtilisation();

protected:
	vector<long> gpuFreqs;
	int gpuFreqPosition;;
	virtual void initGPUFreqValues();
};

#endif /* GPU_GPU_H_ */
