#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <android/log.h>
#include <errno.h>


#define TEXT_BUFFER_SIZE 20
#define COMMAND_LENGTH 200
#define CLASSNAME "IOStuff"


void writeStringToFile(const char * filePath, const char * value){
	char buff[COMMAND_LENGTH];
	sprintf(buff,"su -c \"echo %s > %s\"", value, filePath );
	system(buff);
}

void writeValueToFile(const char * filePath, float value){
	char buff[COMMAND_LENGTH];
	sprintf(buff,"su -c \"echo %f > %s\"", value, filePath );
	system(buff);
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
	fgets(buffer, buffSize, fp);
	fclose(fp);
}

