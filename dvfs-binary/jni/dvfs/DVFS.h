/*
 * DVFS.h
 *
 *  Created on: 15 Feb, 2015
 *      Author: yeokm1
 */

#ifndef DVFS_DVFS_H_
#define DVFS_DVFS_H_

#include "cpu/CPUOdroid.h"
#include "cpu/CPUNexus5.h"
#include "gpu/GPUOdroid.h"
#include "gpu/GPUNexus5.h"


#include <vector>
#include <string>

//#define DEBUG

#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif

#define CLASSNAME "DVFS-ndk"
#define POLL_RATE_IN_MICROSECOND 1000000  //1 second
#define DO_NOT_PURSUE_FPS_VALUE -1
#define SLIDING_WINDOW_LENGTH 5

using std::vector;
using std::string;

class DVFS {
public:
	DVFS(int fpsLowBound, int fpsHighBound);
	void startDVFS();
	void stopDVFS();
	virtual ~DVFS();

private:
	int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
	bool isPhoneOdroid();
	string getModel();
	bool loopInProgress;

protected:

	void fpsDetected();
	void noFpsDetected();
	bool inGameMode;
	CPU * cpu;
	GPU * gpu;
	int fpsLowBound;
	int fpsHighBound;
	int currentSlidingWindowPosition;
	virtual void regularRunner();
	int findLowestFreqPositionThatMeetsThisCost(double costToMeet, vector<long> availableFrequencies, float factor);
	int shouldPursueFPSRecalculationToThisFPS(int fps);
	void readySystemForDVFS();
	void setSystembackToDefault();
};

#endif /* DVFS_DVFS_H_ */
