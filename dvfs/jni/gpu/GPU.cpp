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
#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView && echo e"
#define FPS_COMMAND_ENDING "e"
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

	string intermediate;
	char *finishFrameTime;

	stack<long long> values;

	//Grab the first line. It is usually in the form 16666... and useless
	std::getline(*proc, intermediate);

	if(intermediate.compare(FPS_COMMAND_ENDING) == 0){
		return NO_FPS_CALCULATED;
	}

    while (true) {

    	std::getline(*proc, intermediate);
    	if(intermediate.compare(FPS_COMMAND_ENDING) == 0){
    		break;
    	}

		const char * lineConst = intermediate.c_str();
		char line[intermediate.length()];
		std::strcpy(line, lineConst);

		strtok(line, "\t");
		strtok(NULL, "\t");
		finishFrameTime = strtok(NULL, "\t");

		if(finishFrameTime != NULL){
			long long parsed = atoll(finishFrameTime);
			values.push(parsed);
		}

    }

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
