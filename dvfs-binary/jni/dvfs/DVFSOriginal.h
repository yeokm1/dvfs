/*
 * DVFSOriginal.h
 *
 *  Created on: 15 Feb, 2015
 *      Author: yeokm1
 */

#ifndef DVFS_DVFSORIGINAL_H_
#define DVFS_DVFSORIGINAL_H_

#include <dvfs/DVFS.h>

class DVFSOriginal: public DVFS {
public:
	DVFSOriginal(int fpsLowBound, int fpsHighBound);
	virtual ~DVFSOriginal();

protected:
	void regularRunner();

private:
	void makeCPUMeetThisFPS(int targetFPS, int currentFPS, CPU * cpu);
	void makeGPUMeetThisFPS(int targetFPS, int currentFPS, GPU * gpu);
	void processInputs(int currentFPS, int newFPSValue, bool fpsInRange, CPU * cpu, GPU * gpu);
};

#endif /* DVFS_DVFSORIGINAL_H_ */
