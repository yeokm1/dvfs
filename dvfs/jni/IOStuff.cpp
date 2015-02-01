#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <android/log.h>
#include <unistd.h>
#include "pstream.h"


#define TEXT_BUFFER_SIZE 20
#define COMMAND_LENGTH 200
#define CLASSNAME "IOStuff"

redi::pstream * proc;

void startShell(){
	proc = new redi::pstream("su", redi::pstreams::pstdout | redi::pstreams::pstdin);
}

void stopShell(){
	free(proc);
}

void writeStringToFile(const char * filePath, const char * value){
	char buff[COMMAND_LENGTH];
	sprintf(buff,"echo %s > %s", value, filePath );
	*proc << buff << std::endl;
}

void writeValueToFile(const char * filePath, long value){
	char buff[COMMAND_LENGTH];
	sprintf(buff,"echo %ld > %s", value, filePath );
	*proc << buff << std::endl;
}

float getValueFromFile(const char * filename){
    float  result;
    char newLine[TEXT_BUFFER_SIZE];
    FILE   *filePtr = fopen(filename, "r");
    fgets(newLine, TEXT_BUFFER_SIZE, filePtr);
    fclose(filePtr);

    result = atof(newLine);

    return result;
}

void getStringFromFile(const char * filename, char * buffer, int buffSize){
	FILE *fp =  fopen (filename , "r");
	fread(buffer, sizeof(char), buffSize, fp);
	fclose(fp);
}

void getStringFromFileByCat(const char * filename, char * buffer, int buffSize){
	char command[COMMAND_LENGTH];
	sprintf(command,"cat %s", filename);

	FILE *pp = popen(command, "r");
	fgets(buffer, buffSize, pp);
	pclose(pp);
}

