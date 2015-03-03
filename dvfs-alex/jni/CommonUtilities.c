/*
 *  Created on: 18 Aug, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 */

#include "CommonUtilities.h"

void readFromFile(const char *filePath, char *textBuffer, int *textSize)
{
    FILE *fp = NULL;

    assert(textBuffer);
    assert(filePath);
    fp = fopen(filePath, "r");
    assert(fp);
    *textSize = fread(textBuffer, 1, *textSize, fp);
    fclose(fp);
}

Element *allocVector(int dim)
{
    return (Element *)malloc(dim * sizeof(Element));
}

Element *allocVectorClean(int dim)
{
    return (Element *)calloc(dim, sizeof(Element));
}

Element **allocMatrixPointer(int dim)
{
    return (Element **)malloc(dim * sizeof(Element *));
}

Element **allocMatrix(int dim1, int dim2)
{
    Element **matrix;

    matrix = allocMatrixPointer(dim1);

    for(int i = 0; i < dim1; i++)
    {
        matrix[i] = allocVector(dim2);
    }

    return matrix;
}

Element **allocMatrixClean(int dim1, int dim2)
{
    Element **matrix;

    matrix = (Element **)malloc(dim1 * sizeof(Element *));

    for(int i = 0; i < dim1; i++)
    {
        matrix[i] = allocVectorClean(dim2);
    }

    return matrix;
}

Element getSimpleRandom(void)
{
    Element result;

    //srand(time(NULL));
    result = (Element)rand() / (Element)RAND_MAX;

    return result;
}

Element getRandom(Element low, Element high)
{
    Element result;

    result = getSimpleRandom();
    result = result * (high - low) + low;

    return result;
}
