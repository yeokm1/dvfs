#include <stdio.h>
#include <stdlib.h>


#define TEXT_BUFFER_SIZE 20


void writeStringToFile(const char * filePath, const char * value){
    FILE * filePtr = fopen(filePath, "w");
    fputs(value, filePtr);
    fclose (filePtr);
}

void writeValueToFile(const char * filePath, float value){
    FILE * filePtr = fopen(filePath, "w");
    fprintf(filePtr, "%f", value);
    fclose (filePtr);
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

