#include <stdio.h>
#include "dvfs/DVFSNew.h"



int main( int argc, char* argv[]){
	if(argc != 3){
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


    DVFSNew dvfs(lowBound, highBound);
    dvfs.startDVFS();
    return 0;
}






