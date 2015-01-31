/*
 * IOStuff.h

 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef IOSTUFF_H_

#define IOSTUFF_H_

void writeStringToFile(const char * filename, const char * value);
void writeValueToFile(const char * filename, float value);
float getValueFromFile(const char * filename);
char * getStringFromFile(const char * filename);




#endif /* IOSTUFF_H_ */


