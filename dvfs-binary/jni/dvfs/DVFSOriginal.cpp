/*
 * DVFSOriginal.cpp
 *
 *  Created on: 15 Feb, 2015
 *      Author: yeokm1
 */

#include <dvfs/DVFSOriginal.h>

DVFSOriginal::DVFSOriginal(int fpsLowBound, int fpsHighBound) : DVFS(fpsLowBound,  fpsHighBound){


}

DVFSOriginal::~DVFSOriginal() {

}



void DVFSOriginal::regularRunner(){

	int currentFPS = gpu->getFPS();
	D(printf("FPS %d\n", currentFPS));


	if(currentFPS == NO_FPS_CALCULATED){
		//Recalculate GPU immediately on next try if cannot get FPS
		currentSlidingWindowPosition = SLIDING_WINDOW_LENGTH;
	} else {

		int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);

		if(newValueFPS == DO_NOT_PURSUE_FPS_VALUE){
			processInputs(currentFPS, currentFPS, true, cpu, gpu);
		} else {
			processInputs(currentFPS, newValueFPS, false, cpu , gpu);
		}
	}


}

void DVFSOriginal::makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu){
	D(printf("CPU meet this FPS: %d\n", targetFPS));

	int currentCPUFreqPosition = cpu->getCpuFreqPosition();

	vector<long> cpuFreqs = cpu->getCPUFreqs();

	float cpuUtil = cpu->getAverageCPUUtilisation();
	D(printf("CPU Avg Util %f\n", cpuUtil));


	long currentCPUFrequency = cpuFreqs[currentCPUFreqPosition];


	double currentCost = (cpuUtil * currentCPUFrequency)
					* (((double)targetFPS)
							/ currentFPS);

	int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, cpuFreqs, cpuUtil);


	if(currentCPUFreqPosition != newCPUFreqPosition){
		cpu->setCPUFreq(newCPUFreqPosition);
	}

	D(printf("New CPU Freq Pos: %d\n", newCPUFreqPosition));
}

void DVFSOriginal::makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu){
	D(printf("GPU meet this FPS: %d\n", targetFPS));

	int currentGPUFreqPosition = gpu->getGpuFreqPosition();

	vector<long> gpuFreqs = gpu->getGPUFreqs();

	long currentGPUFrequency = gpuFreqs[currentGPUFreqPosition];

	float gpuUtil = gpu->getUtilisation();

	D(printf("GPU Util %f\n", gpuUtil));

	double currentCost = gpuUtil * currentGPUFrequency
					* (((double)targetFPS)
							/ currentFPS);


	int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, gpuFreqs, gpuUtil);


	if(currentGPUFreqPosition != newGPUFreqPosition){
		gpu->setGPUFreq(newGPUFreqPosition);
	}

	D(printf("New GPU Freq Pos: %d\n", newGPUFreqPosition));

}

void DVFSOriginal::processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu){

	D(printf("Current FPS: %d, target FPS: %d\n", currentFPS, newFPSValue));

	if(!fpsInRange){
		makeCPUMeetThisFPS(newFPSValue, currentFPS, cpu);

		currentSlidingWindowPosition++;

		if(currentSlidingWindowPosition > SLIDING_WINDOW_LENGTH){
			currentSlidingWindowPosition = 0;
			makeGPUMeetThisFPS(newFPSValue, currentFPS, gpu);
		}
	}



}



