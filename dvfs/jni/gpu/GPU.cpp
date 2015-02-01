/*
 * GPU.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPU.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <android/log.h>
#include <stack>
#include <string>
#include <IOStuff.h>

#define CLASSNAME "GPU"
#define FPS_RESULT_LINE_LENGTH 100
#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView"
#define FPS_INTERVAL_NANOS 1000000000
#define MAX_FPS 60

using std::stack;

GPU::GPU() {
	gpuFreqPosition = 0;
}

GPU::~GPU() {
}


void GPU::initGPUFreqValues(){
	//Do nothing here
}

void GPU::setGPUFreq(int position){
	//Do nothing here
}

int GPU::getFPS(){

	__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "Get FPS start");

	redi::pstream * proc = getShellInstance();
	* proc << FPS_COMMAND << std::endl;


	char line[FPS_RESULT_LINE_LENGTH];

	std::streamsize extracted = proc->readsome(line, 1);


	if(extracted == 0){
		__android_log_print(ANDROID_LOG_INFO, CLASSNAME, "FPS line length 0");
		return NO_FPS_CALCULATED;
	}


	proc->getline(line, FPS_RESULT_LINE_LENGTH);


	char *finishFrameTime;


	stack<long long> values;

	while(proc->readsome(line, 1) > 0){
		//Ignore the first two values

		proc->getline(line, FPS_RESULT_LINE_LENGTH);
		strtok(line, "\t");
		strtok(NULL, "\t");
		finishFrameTime = strtok(NULL, "\t");

		if(finishFrameTime != NULL){
			long long parsed = atoll(finishFrameTime);
			values.push(parsed);
		}

	}


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
