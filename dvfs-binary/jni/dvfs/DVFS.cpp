/*
 * DVFS.cpp
 *
 *  Created on: 15 Feb, 2015
 *      Author: yeokm1
 */

#include <dvfs/DVFS.h>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/system_properties.h>
#include "IOStuff.h"

#define BATTERY_LEVEL_BUFF 5
#define FILE_BATTERY_LEVEL "/sys/class/power_supply/battery/capacity"

#define MIN_OF_MIN_FPS_ALLOWED 30
#define MAX_OF_MIN_FPS_ALLOWED 55
#define FPS_GAP 5

#define MIN_BATTERY_CUTOFF 40
#define MAX_BATTERY_CUTOFF 100

#define MIN_FPS_CUTOFF 30
#define MAX_FPS_CUTOFF 60

using std::string;


DVFS::DVFS(int fpsLowBound, int fpsHighBound) {
	this->fpsLowBound = fpsLowBound;
	this->fpsHighBound = fpsHighBound;
	this->currentSlidingWindowPosition = 0;
	this->loopInProgress = false;
	this->inGameMode = false;
	this->numTimesFPSNotDetected = 0;

	if(isPhoneOdroid()){
		D(printf("Model Odroid\n"));
		cpu = new CPUOdroid();
		gpu = new GPUOdroid();
	} else {
		D(printf("Model Not Odroid\n"));
		cpu = new CPUNexus5();
		gpu = new GPUNexus5();
	}


	if(fpsLowBound == DYNAMIC_FPS_TARGET || fpsHighBound == DYNAMIC_FPS_TARGET){
		dynamicTargetRange = true;
	} else {
		dynamicTargetRange = false;
	}
}

DVFS::~DVFS() {
	free(cpu);
	free(gpu);
}

void DVFS::startDVFS(){
	loopInProgress = true;

	struct timeval tvBegin, tvEnd, tvDiff;


	//To force to ondemand intially
	inGameMode = true;
	noFpsDetected();

	while(loopInProgress){

		gettimeofday(&tvBegin, NULL);

		regularRunner();

		gettimeofday(&tvEnd, NULL);

		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);

		int elapsedTime = tvDiff.tv_usec;

		//This is to calculate how much time to sleep before the next poll
		if(elapsedTime < POLL_RATE_IN_MICROSECOND){
			int timeToSleep = POLL_RATE_IN_MICROSECOND - elapsedTime;
			usleep(timeToSleep);
		}


	}

}

void DVFS::stopDVFS(){
	loopInProgress = false;
}


//From http://stackoverflow.com/questions/1468596/calculating-elapsed-time-in-a-c-program-in-milliseconds
/* Return 1 if the difference is negative, otherwise 0.  */
int DVFS::timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1){
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
}

void DVFS::regularRunner(){
	//Not implemented here
}

int DVFS::shouldPursueFPSRecalculationToThisFPS(int fps){
	if(fps > fpsHighBound){
		//We need to decrease FPS
		return fpsLowBound;
	} else if(fps < fpsLowBound){
		//We need to increase FPS
		return fpsHighBound;
	} else {
		currentSlidingWindowPosition = 0;
		return DO_NOT_PURSUE_FPS_VALUE;
	}

}


int DVFS::findLowestFreqPositionThatMeetsThisCost(double costToMeet, vector<long> availableFrequencies, float factor){

	for(unsigned int position = 0; position < availableFrequencies.size(); position++){
		long chosenFreq = availableFrequencies[position];
		double calculatedCost = chosenFreq * factor;
		if(calculatedCost >= costToMeet){
			return position;
		}
	}

	return availableFrequencies.size() - 1;
}

bool DVFS::isPhoneOdroid(){
	string model = getModel();
	if(model.compare("ODROID-XU") == 0){
		return true;
	} else {
		return false;
	}
}

string DVFS::getModel(){
	char model_string[PROP_VALUE_MAX+1];
	__system_property_get("ro.product.model", model_string);
	return model_string;
}

void DVFS::readySystemForDVFS(){
	cpu->setGovernorToUserspaceAndInit();
	gpu->initGPUForModification();

}
void DVFS::setSystembackToDefault(){
	cpu->setGovernorToOndemandAndRevert();
	gpu->revertGPUToOriginal();

}

void DVFS::fpsDetected(){
	if(!inGameMode){
		readySystemForDVFS();
		inGameMode = true;
		numTimesFPSNotDetected = 0;
	}

	if(dynamicTargetRange){
		decideDynamicFPSTarget();
	}
}

void DVFS::noFpsDetected(){
	if(inGameMode){

		numTimesFPSNotDetected++;

		if(numTimesFPSNotDetected > GO_BACK_TO_ONDEMAND_IF_FPS_NOT_DETECTED){
			setSystembackToDefault();
			inGameMode = false;
		}

	}
}

int DVFS::getBatteryLevel(){
	char buff[BATTERY_LEVEL_BUFF];

	getStringFromFile(FILE_BATTERY_LEVEL, buff, BATTERY_LEVEL_BUFF);

	int level = atoi(buff);

	return level;
}

void DVFS::decideDynamicFPSTarget(){
	int battery = getBatteryLevel();

	int targeted = (int) mapRange(MIN_BATTERY_CUTOFF, MAX_BATTERY_CUTOFF, MIN_FPS_CUTOFF, MAX_FPS_CUTOFF, battery);

	if(targeted < MIN_OF_MIN_FPS_ALLOWED){
		targeted = MIN_OF_MIN_FPS_ALLOWED;
	} else if(targeted > MAX_OF_MIN_FPS_ALLOWED){
		targeted = MAX_OF_MIN_FPS_ALLOWED;
	}


	fpsLowBound = targeted;
	fpsHighBound = fpsLowBound + 5;


	D(printf("Battery %d, target range is now %d to %d\n", battery, fpsLowBound, fpsHighBound));



}


double DVFS::mapRange(double a1,double a2,double b1,double b2,double s){
	return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

