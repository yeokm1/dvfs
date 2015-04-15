#include <IOStuff.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define TEXT_BUFFER_SIZE 20
#define COMMAND_LENGTH 200
#define CLASSNAME "IOStuff"

#define ECHO_VALUE_COMMAND "echo %ld > %s"
#define ECHO_STRING_COMMAND "echo %s > %s"

void writeStringToFile(const char * filePath, const char * value){
    FILE  *filePtr = fopen(filePath, "w");
    fputs(value, filePtr);
    fclose(filePtr);
}

void writeValueToFile(const char * filePath, long value){
    FILE  *filePtr = fopen(filePath, "w");
    fprintf(filePtr, "%ld", value);
    fclose(filePtr);
}

void writeStringToFileWithEcho(const char * filePath, const char * value){
	char buff[COMMAND_LENGTH];
	sprintf(buff, ECHO_STRING_COMMAND, value, filePath);
	runCommand(buff);
}

void writeValueToFileWithEcho(const char * filePath, long value){
	char buff[COMMAND_LENGTH];
	sprintf(buff, ECHO_VALUE_COMMAND, value, filePath);
	runCommand(buff);
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

void runCommand(const char * command){
	FILE * run = popen(command, "r");
	pclose(run);

}

