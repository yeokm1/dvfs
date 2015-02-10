/*
 * GPUOdroid.h
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#ifndef GPU_GPUODROID_H_
#define GPU_GPUODROID_H_

#include <gpu/GPU.h>

class GPUOdroid: public GPU {
public:
	GPUOdroid();
	virtual ~GPUOdroid();
	void setGPUFreq(int position);
	int getFPS();
	float getUtilisation();

protected:
	void initGPUFreqValues();
};

#endif /* GPU_GPUODROID_H_ */
