#include <IOStuff.h>
#include <stdio.h>
#include <cstdlib>

#define TEXT_BUFFER_SIZE 20


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

