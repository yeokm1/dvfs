#include <stdio.h>
#include "dvfs/DVFS.h"
#include "dvfs/DVFSNew.h"
#include "dvfs/DVFSOriginal.h"
#include "dvfs/DVFSAlex.h"


int main( int argc, char* argv[]){

	DVFS * dvfs;


	int lowBound;
	int highBound;

	if(argc < 3){
		lowBound = DYNAMIC_FPS_TARGET;
		highBound = DYNAMIC_FPS_TARGET;

		printf("Dynamic FPS range selected\n");

	} else {
		char * lowBoundStr = argv[1];
		char * highBoundStr = argv[2];

		lowBound = atoi(lowBoundStr);
		highBound = atoi(highBoundStr);

		if(lowBound > highBound){
			printf("Low Bound cannot be higher that High bound\n");
			return 1;
		} else {
			printf("FPS range selected %d to %d\n", lowBound, highBound);
		}
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






