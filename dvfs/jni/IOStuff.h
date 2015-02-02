/*
 * IOStuff.h

 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef IOSTUFF_H_

#define IOSTUFF_H_
#include "pstream.h"
using std::string;

void writeStringToFile(const char * filename, const char * value);
void writeValueToFile(const char * filename, long value);
float getValueFromFile(const char * filename);
void getStringFromFile(const char * filename, char * buffer, int buffSize);
void getStringFromFileByCat(const char * filename, char * buffer, int buffSize);

void startShell();
void stopShell();
redi::pstream * getShellInstance();

#endif /* IOSTUFF_H_ */


