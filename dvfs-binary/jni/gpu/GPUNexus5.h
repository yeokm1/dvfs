/*
 * GPUNexus5.h
 *
 *  Created on: 3 Feb, 2015
 *      Author: yeokm1
 */

#ifndef GPUNEXUS5_H_
#define GPUNEXUS5_H_

#include <gpu/GPU.h>

class GPUNexus5: public GPU {
public:
	GPUNexus5();
	virtual ~GPUNexus5();
	void setGPUFreq(int position);
	float getUtilisation();
	void initGPUForModification();
	void revertGPUToOriginal();

protected:
	void initGPUFreqValues();
	void setToNoGPUPolicy();
	void setToDefaultGPUPolicy();
};

#endif /* GPUNEXUS5_H_ */
