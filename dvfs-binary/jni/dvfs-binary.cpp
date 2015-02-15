#include <stdio.h>
#include "dvfs/DVFS.h"
#include "dvfs/DVFSNew.h"
#include "dvfs/DVFSOriginal.h"



int main( int argc, char* argv[]){


	bool useNew = true;

	if(argc < 3){
		printf("Not enough arguments\n");
		return 1;
	} else if(argc >= 4 && (strcmp(argv[3], "-org") == 0)){
		useNew = false;
	}

	char * lowBoundStr = argv[1];
	char * highBoundStr = argv[2];

	int lowBound = atoi(lowBoundStr);
	int highBound = atoi(highBoundStr);

	if(lowBound > highBound){
		printf("Low Bound cannot be higher that High bound\n");
		return 1;
	}

	DVFS * dvfs;

	if(useNew){
		dvfs = new DVFSNew(lowBound, highBound);
	} else {
		dvfs = new DVFSOriginal(lowBound, highBound);
	}


    dvfs->startDVFS();
    return 0;
}






