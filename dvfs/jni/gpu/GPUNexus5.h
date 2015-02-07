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
	int getFPS();

protected:
	void initGPUFreqValues();
	void setToNoGPUPolicy();
};

#endif /* GPUNEXUS5_H_ */
