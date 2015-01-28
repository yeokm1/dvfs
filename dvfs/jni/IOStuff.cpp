/*
 * IOStuff.cpp
 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#include <IOStuff.h>
#include <stdio.h>
#include <cstdlib>

#define TEXT_BUFFER_SIZE 20

IOStuff::IOStuff() {
	// TODO Auto-generated constructor stub

}

IOStuff::~IOStuff() {
	// TODO Auto-generated destructor stub
}


void IOStuff::writeValueToFile(const char * filePath, float value){
    FILE * filePtr = fopen(filePath, "w");
    fprintf(filePtr, "%f", value);
    fclose (filePtr);
}

float IOStuff::getValueFromFile(const char * filename){
    float  result;
    char newLine[TEXT_BUFFER_SIZE];
    FILE   *filePtr = fopen(filename, "r");
    fgets(newLine, TEXT_BUFFER_SIZE, filePtr);
    fclose(filePtr);

    result = atof(newLine);

    return result;
}

