/*
 * IOStuff.h

 *
 *  Created on: 29 Jan, 2015
 *      Author: yeokm1
 */

#ifndef IOSTUFF_H_


#define IOSTUFF_H_

class IOStuff {
public:
	IOStuff();
	virtual ~IOStuff();
	void writeValueToFile(const char * filename, float value);
	float getValueToFile(const char * filename);
};

#endif /* IOSTUFF_H_ */
