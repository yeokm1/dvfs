#include <stdio.h>
#include "dvfs/DVFS.h"
#include "dvfs/DVFSNew.h"
#include "dvfs/DVFSOriginal.h"
#include "dvfs/DVFSAlex.h"


int main( int argc, char* argv[]){

	DVFS * dvfs;


	if(argc < 3){
		printf("Not enough arguments\n");
		return 1;
	}

	char * lowBoundStr = argv[1];
	char * highBoundStr = argv[2];

	int lowBound = atoi(lowBoundStr);
	int highBound = atoi(highBoundStr);

	if(lowBound > highBound){
		printf("Low Bound cannot be higher that High bound\n");
		return 1;
	}



	if(argc >= 4){
		if(strcmp(argv[3], "-org") == 0){

			printf("Selected DVFS Original\n");
			dvfs = new DVFSOriginal(lowBound, highBound);
		} else if(strcmp(argv[3], "-alex") == 0){

			printf("Selected DVFS Alex\n");
			dvfs = new DVFSAlex(lowBound, highBound);
		} else {

			printf("Selected DVFS New\n");
			dvfs = new DVFSNew(lowBound, highBound);
		}
	} else {
		printf("Selected DVFS New\n");
		dvfs = new DVFSNew(lowBound, highBound);
	}






    dvfs->startDVFS();

    free(dvfs);
    return 0;
}






