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


}

void DVFSOriginal::makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu){

}

void DVFSOriginal::makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu){
	D(printf("GPU meet this FPS: %d\n", targetFPS));

}

void DVFSOriginal::processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu){


	D(printf("Current FPS: %d, target FPS: %d\n", currentFPS, newFPSValue));

}



