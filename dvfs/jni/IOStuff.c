#include <stdio.h>
#include <stdlib.h>


#define TEXT_BUFFER_SIZE 20


void writeStringToFile(char * filePath, char * value){
    FILE * filePtr = fopen(filePath, "w");
    fputs(value, filePtr);
    fclose (filePtr);
}

void writeValueToFile(char * filePath, float value){
    FILE * filePtr = fopen(filePath, "w");
    fprintf(filePtr, "%f", value);
    fclose (filePtr);
}

float getValueFromFile(char * filename){
    float  result;
    char newLine[TEXT_BUFFER_SIZE];
    FILE   *filePtr = fopen(filename, "r");
    fgets(newLine, TEXT_BUFFER_SIZE, filePtr);
    fclose(filePtr);

    result = atof(newLine);

    return result;
}

char * getStringFromFile(char * filename){
	FILE *fp =  fopen (filename , "r");
	fseek( fp , 0L , SEEK_END);
	long fileSize = ftell( fp );
	long buffSize = fileSize + 1;
	rewind(fp);

	//+1 for null character
	char *buffer = (char*) calloc(buffSize, sizeof(char));
	fgets(buffer, buffSize, fp);
	fclose(fp);

	return buffer;
}

