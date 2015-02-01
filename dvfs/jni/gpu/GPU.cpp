/*
 * GPU.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPU.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <stack>

#define CLASSNAME "GPU"
#define FPS_RESULT_LINE_LENGTH 100
#define FPS_COMMAND "su -c \"dumpsys SurfaceFlinger --latency SurfaceView\""
#define FPS_INTERVAL_NANOS 1000000000
#define MAX_FPS 60

using std::stack;

GPU::GPU() {
	gpuFreqPosition = 0;
}

GPU::~GPU() {
}

vector<long> GPU::getGPUFreqs(){
	return gpuFreqs;
}

int GPU::getGpuFreqPosition(){
	return gpuFreqPosition;
}

int GPU::getFPS(){
	FILE *pp = popen(FPS_COMMAND, "r");

	char line[FPS_RESULT_LINE_LENGTH];
	//Ignore the first line
	fgets(line, sizeof(line), pp);

	if(strlen(line) == 0){
		pclose(pp);
		return NO_FPS_CALCULATED;
	}


	char *finishFrameTime;
	stack<long long> values;

	while(fgets(line, sizeof (line), pp) != NULL){
		//Ignore the first two values
		strtok(line, "\t");
		strtok(NULL, "\t");
		finishFrameTime = strtok(NULL, "\t");

		if(finishFrameTime != NULL){
			long long parsed = atoll(finishFrameTime);
			values.push(parsed);
		}

	}

	pclose(pp);

	//Final check to ensure there is data collected in the first place
	if(values.size() == 0){
		return NO_FPS_CALCULATED;
	}

	int frameCount = 0;
	int fps = 0;
	long long lastFrameFinishedTime = values.top();

	while(!values.empty()){
		long long currentValue = values.top();
		values.pop();

		if ((lastFrameFinishedTime - currentValue) <= FPS_INTERVAL_NANOS)	{
			frameCount++;
		}
	}


	//Cap to Max FPS
	return (frameCount <= MAX_FPS) ? frameCount : MAX_FPS;
}
