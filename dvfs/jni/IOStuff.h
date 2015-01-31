/*
 * IOStuff.h

 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef IOSTUFF_H_

#define IOSTUFF_H_

void writeStringToFile(char * filename, char * value);
void writeValueToFile(char * filename, float value);
float getValueFromFile(char * filename);
char * getStringFromFile(char * filename);




#endif /* IOSTUFF_H_ */


