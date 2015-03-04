/*
 * DVFSAlex.cpp
 *
 *  Created on: 4 Mar, 2015
 *      Author: yeokm1
 */

#include <dvfs/DVFSAlex.h>

#define FREQ_INDEX_CPU(cpuFreq) (cpuFreq / 100 - 8)
#define FREQ_INDEX_GPU(gpuFreq) (gpuFreq / 100 - 1)

#define HIGHER_FC_IDX(cpuFreqIdx) (((cpuFreqIdx) < (10 - 1)) ? \
    (cpuFreqIdx + 1) : (cpuFreqIdx))
#define HIGHER_FG_IDX(gpuFreqIdx) (((gpuFreqIdx) < (6 - 1)) ? \
    (gpuFreqIdx + 1) : (gpuFreqIdx))


#define HIGHER_FC_IDX(cpuFreqIdx) (((cpuFreqIdx) < (10 - 1)) ? \
    (cpuFreqIdx + 1) : (cpuFreqIdx))
#define HIGHER_FG_IDX(gpuFreqIdx) (((gpuFreqIdx) < (6 - 1)) ? \
    (gpuFreqIdx + 1) : (gpuFreqIdx))

#define LOWER_FC_IDX(cpuFreqIdx) (((cpuFreqIdx) > 0) ? (cpuFreqIdx - 1) : (cpuFreqIdx))
#define LOWER_FG_IDX(gpuFreqIdx) (((gpuFreqIdx) > 0) ? (gpuFreqIdx - 1) : (gpuFreqIdx))

    int sample  = 0;
    int newFc   = 0;
    int newFg   = 0;
    float estimFPS = 0.0;

    float Qmin;
    float Qmax;
    float UC;
    float UG;
    float PC;
    float PG;
    int c, ci;
    int g, gi;
    float Q, Qreal;
    float OC, OCi;
    float OG, OGi;


DVFSAlex::DVFSAlex(int fpsLowBound, int fpsHighBound) : DVFS(fpsLowBound,  fpsHighBound){
Qmin = fpsLowBound;
Qmax = fpsHighBound;

}

DVFSAlex::~DVFSAlex() {

}



void DVFSAlex::regularRunner(){

	int currentFPS = gpu->getFPS();
	D(printf("FPS %d\n", currentFPS));





	if(currentFPS == NO_FPS_CALCULATED){
		//Recalculate GPU immediately on next try if cannot get FPS

		noFpsDetected();

		if(inGameMode == false){
			return;
		}



	} else {

		fpsDetected();

		if(inGameMode == false){
			return;
		}



        UC = cpu->getAverageCPUUtilisation();
        UG = gpu->getUtilisation();
        Qreal = gpu->getFPS();
        c = cpu->getCPUFreqs()[cpu->getCpuFreqPosition()];
        g = gpu->getGPUFreqs()[gpu->getGpuFreqPosition()];

        PC = (UC * c) / Qreal; // price paid by CPU for 1 FPS
        PG = (UG * g) / Qreal;

        int newCPUPosition = cpu->getCpuFreqPosition();
        int newGPUPosition = gpu->getGpuFreqPosition();


        if(Qreal < Qmin)
        {
            Q = Qmax;
            OC = PC * Q; // expected CPU cost sufficient to achieve Q
            OG = PG * Q;


            for(int idxFc = 0; idxFc < cpu->getCPUFreqs().size(); idxFc++)
            {
                ci = cpu->getCPUFreqs()[idxFc];
                OCi = ci * UC;

                if(OCi >= OC)
                {
                    c = ci;
                    newCPUPosition = idxFc;
                    break;
                }
            }

            for(int idxFg = 0; idxFg < gpu->getGPUFreqs().size(); idxFg++)
            {
                gi = gpu->getGPUFreqs()[idxFg];
                OGi = gi * UG;

                if(OGi >= OG)
                {
                    g = gi;
                    newGPUPosition = idxFg;
                    break;
                }
            }
        }

        if(Qreal > Qmax)
        {
            Q = Qmin;
            OC = PC * Q; // expected CPU cost sufficient to achieve Q
            OG = PG * Q;



            for(int idxFc = 0; idxFc >= cpu->getCPUFreqs().size(); idxFc--)
            {
                ci = cpu->getCPUFreqs()[idxFc];
                OCi = ci * UC;

                if(OCi >= OC)
                {
                    c = ci;
                    newCPUPosition = idxFc;
                    //break;
                }
            }

            for(int idxFg = 0; idxFg >= gpu->getGPUFreqs().size(); idxFg--)
            {
                gi = gpu->getGPUFreqs()[idxFg];
                OGi = gi * UG;

                if(OGi >= OG)
                {
                    g = gi;
                    newGPUPosition = idxFg;
                    //break;
                }
            }
        }

        //printf("cLowerLimit: %d, cUpperLimit: %d, gLowerLimit: %d, gUpperLimit: %d\n", cLowerLimit, cUpperLimit, gLowerLimit, gUpperLimit);

        //printf("currFc: %d, currFg: %d, UC: %f, UG: %f, Qreal: %f, OC: %f, OG: %f, PC: %f, PG: %f\n",
               //c, g, UC, UG, Qreal, OC, OG, PC, PG);


        if(cpu->getCpuFreqPosition() != newCPUPosition){
            cpu->setCPUFreq(newCPUPosition);
        }


        if(gpu->getGpuFreqPosition() != newGPUPosition){
            gpu->setGPUFreq(newGPUPosition);
        }















	}


}






