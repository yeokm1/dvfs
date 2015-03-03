/*
 * CommonUtilities.h
 *
 *  Created on: 18 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#ifndef COMMON_UTILITIES_H_
#define COMMON_UTILITIES_H_

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <errno.h>

typedef float Element;

#define true                        (1)
#define false                       (0)
#define IDENTICAL                   (0)
#define EPSILON                     (0.000000001)
#define ISSPACE(c)                  (' ' == (c))
#define ABS(a)                      ((a) < 0 ? ((-1) * (a)) : (a))

#define TIC \
{ \
    struct timeval startTime; \
    struct timeval stopTime; \
    long long elapsedTime; \
    gettimeofday(&startTime, NULL);

#define TOC \
    gettimeofday(&stopTime, NULL); \
    elapsedTime = (stopTime.tv_sec - startTime.tv_sec) * 1000000LL  \
        + stopTime.tv_usec - startTime.tv_usec; \
    printf("%lld us\n", elapsedTime); \
}

void        readFromFile(const char *filePath, char *textBuffer, int *textSize);

Element     getSimpleRandom(void);
Element     getRandom(Element low, Element high);

Element     *allocVector(int dim);
Element     *allocVectorClean(int dim);
Element     **allocMatrixPointer(int dim);
Element     **allocMatrix(int dim1, int dim2);
Element     **allocMatrixClean(int dim1, int dim2);

Element     min(Element *V, int dim);
Element     max(Element *V, int dim);
Element     mean(Element *V, int dim);

char        *trimwhitespace(char *str); /// NOT OK
size_t      trimwhitespace2(char *out, size_t len, const char *str); /// NOT OK

void        matrixMultiply(int dim1, int dim2, Element **M1, Element **M2, Element **M3);
void        addToVector(int dim, Element *V, Element C);
void        printVector(Element *V, int dim);
void        printVector2(Element *V, int dim, const char *title);
void        printMatrix(Element **V, int dim1, int dim2);
void        printMatrix2(Element **V, int dim1, int dim2, const char *title);

void        populateVector(int dim, Element *V);
void        populateMatrix(int dim1, int dim2, Element **V);


// #ifdef __cplusplus
// }
// #endif

#endif /* COMMON_UTILITIES_H_ */
