/*
 * GPU.h
 *
 *  Created on: 1 Feb, 2015
 *      Author: yeokm1
 */

#ifndef GPU_GPU_H_
#define GPU_GPU_H_
#include <vector>

#define NO_FPS_CALCULATED -1
#define FILE_BUFFER_SIZE 300

using std::vector;

class GPU {
public:
	GPU();
	virtual ~GPU();
	vector<long> getGPUFreqs(){ return gpuFreqs; };
	int getGpuFreqPosition(){ return gpuFreqPosition; };
	int getFPS();
	virtual void setGPUFreq(int position);

protected:
	vector<long> gpuFreqs;
	int gpuFreqPosition;;
	virtual void initGPUFreqValues();
};

#endif /* GPU_GPU_H_ */
