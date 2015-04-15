/*
 * GPU.cpp
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#include <gpu/GPU.h>
#include <IOStuff.h>

#define CLASSNAME "GPU"
#define FPS_COMMAND "dumpsys SurfaceFlinger --latency SurfaceView"

GPU::GPU() {
	gpuFreqPosition = 0;
}

GPU::~GPU() {
}


void GPU::initGPUFreqValues(const char * freqFilename){
	char freqLongString[FILE_BUFFER_SIZE];

	getStringFromFile(freqFilename, freqLongString, FILE_BUFFER_SIZE);

	char * freqString;

	long currentFreq;

    freqString = strtok (freqLongString," ");
    currentFreq = atol(freqString);


    //We use a stack as the frequencies in the file are in descending order
    std::stack<long> gpuFreqsStack;
    long prevFreq = currentFreq;

    gpuFreqsStack.push(currentFreq);

	while ((freqString = strtok (NULL, " ")) != NULL){
	   currentFreq = atol(freqString);

	   if(currentFreq != 0 && currentFreq != prevFreq){
		   prevFreq = currentFreq;
		   gpuFreqsStack.push(currentFreq);
	   }
	 }


	gpuFreqs.clear();

	while(!gpuFreqsStack.empty()){
		long freq = gpuFreqsStack.top();
		gpuFreqsStack.pop();
		gpuFreqs.push_back(freq);
	}

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

void GPU::initGPUForModification(){
	//Do nothing
}

void GPU::revertGPUToOriginal(){
	//Do nothing
}
