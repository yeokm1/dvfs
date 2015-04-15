/*
 * DVFSOriginal.cpp
 *
 *  Created on: 15 Feb, 2015
 *      Author: yeokm1
 */

#include <dvfs/DVFSNew.h>

#define TARGET_CPU_UTILISATION 80

DVFSNew::DVFSNew(int fpsLowBound, int fpsHighBound, bool maxTargetIfCharging) : DVFS(fpsLowBound,  fpsHighBound, maxTargetIfCharging){
}

DVFSNew::~DVFSNew() {
}

void DVFSNew::regularRunner(){

	int currentFPS = gpu->getFPS();
	D(printf("FPS %d\n", currentFPS));

	if(currentFPS == NO_FPS_CALCULATED){

		noFpsDetected();

		if(inGameMode == false){
			return;
		}

		//Recalculate GPU immediately on next try if cannot get FPS
		currentSlidingWindowPosition = SLIDING_WINDOW_LENGTH;
	} else {

		fpsDetected();

		if(inGameMode == false){
			return;
		}

		int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);


		if(newValueFPS == DO_NOT_PURSUE_FPS_VALUE){
			processInputs(currentFPS, currentFPS, true, cpu, gpu);
		} else {
			processInputs(currentFPS, newValueFPS, false, cpu , gpu);
		}
	}

}

void DVFSNew::makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu){
	D(printf("CPU meet this FPS: %d\n", targetFPS));

	int currentCPUFreqPosition = cpu->getCpuFreqPosition();

	vector<long> cpuFreqs = cpu->getCPUFreqs();

	float cpuUtil = cpu->getUtilisationOfHighestCore();
	D(printf("CPU Highest Util %f\n", cpuUtil));


	long currentCPUFrequency = cpuFreqs[currentCPUFreqPosition];


	double currentCost = (cpuUtil * currentCPUFrequency)
					* (((double)targetFPS)
							/ currentFPS);

	float targetCPUUtil;

	if(cpuUtil > TARGET_CPU_UTILISATION){
		targetCPUUtil = cpuUtil;
	} else {
		targetCPUUtil = TARGET_CPU_UTILISATION;
	}


	int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, cpuFreqs, targetCPUUtil);


	if(currentCPUFreqPosition != newCPUFreqPosition){
		cpu->setCPUFreq(newCPUFreqPosition);
	}

	D(printf("New CPU Freq Pos: %d\n", newCPUFreqPosition));
}

void DVFSNew::makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu){
	D(printf("GPU meet this FPS: %d\n", targetFPS));

	int currentGPUFreqPosition = gpu->getGpuFreqPosition();

	vector<long> gpuFreqs = gpu->getGPUFreqs();

	long currentGPUFrequency = gpuFreqs[currentGPUFreqPosition];

	double currentCost = currentGPUFrequency
					* (((double)targetFPS)
							/ currentFPS);


	int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(currentCost, gpuFreqs, 1);


	if(currentGPUFreqPosition != newGPUFreqPosition){
		gpu->setGPUFreq(newGPUFreqPosition);
	}

	D(printf("New GPU Freq Pos: %d\n", newGPUFreqPosition));
}

void DVFSNew::processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu){


	D(printf("Current FPS: %d, target FPS: %d\n", currentFPS, newFPSValue));
	makeCPUMeetThisFPS(newFPSValue, currentFPS, cpu);

	if(fpsInRange){
		return;
	}

	currentSlidingWindowPosition++;

	if(currentSlidingWindowPosition > SLIDING_WINDOW_LENGTH){
		currentSlidingWindowPosition = 0;
		makeGPUMeetThisFPS(newFPSValue, currentFPS, gpu);
	}


}

