/*
 * DVFSAlex.h
 *
 *  Created on: 4 Mar, 2015
 *      Author: yeokm1
 */

#ifndef DVFS_DVFSALEX_H_
#define DVFS_DVFSALEX_H_

#include <dvfs/DVFS.h>

class DVFSAlex: public DVFS {
public:
	DVFSAlex(int fpsLowBound, int fpsHighBound, bool maxTargetIfCharging);
	virtual ~DVFSAlex();

protected:
	void regularRunner();

private:
};

#endif /* DVFS_DVFSAlEX_H_ */
