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
using std::string;

DVFS::DVFS(int fpsLowBound, int fpsHighBound) {
	this->fpsLowBound = fpsLowBound;
	this->fpsHighBound = fpsHighBound;
	this->currentSlidingWindowPosition = 0;
	this->dvfsInProgress = false;

	if(isPhoneNexus5()){
		D(printf("Model Nexus 5\n"));
		cpu = new CPUNexus5();
		gpu = new GPUNexus5();
	} else {
		D(printf("Model not Nexus 5\n"));
		cpu = new CPUOdroid();
		gpu = new GPUOdroid();
	}
}

DVFS::~DVFS() {
	free(cpu);
	free(gpu);
}

void DVFS::startDVFS(){
	dvfsInProgress = true;

	struct timeval tvBegin, tvEnd, tvDiff;

	while(dvfsInProgress){

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
	dvfsInProgress = false;
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

bool DVFS::isPhoneNexus5(){
	string model = getModel();
	if(model.compare("Nexus 5") == 0){
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

