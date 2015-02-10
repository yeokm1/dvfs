/*
 * GPU.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPU.h>
#include <android/log.h>
#include <IOStuff.h>

#define CLASSNAME "GPU"
#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView"

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

	int linesEncountered = 0;
	while(fgets(line, sizeof (line), pp) != NULL){
    	linesEncountered++;

    	if(linesEncountered < LINES_IN_DUMPSYS_TO_IGNORE){
    		continue;
    	}

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
	long long lastFrameFinishedTime = values.top();

	while(!values.empty()){
		long long currentValue = values.top();
		values.pop();

		if ((lastFrameFinishedTime - currentValue) <= FPS_INTERVAL_NANOS)	{
			frameCount++;
		}
	}

	if(frameCount == 1){
		return NO_FPS_CALCULATED;
	}


	//Cap to Max FPS
	return (frameCount <= MAX_FPS) ? frameCount : MAX_FPS;
}

float GPU::getUtilisation(){
	return 0;
}
