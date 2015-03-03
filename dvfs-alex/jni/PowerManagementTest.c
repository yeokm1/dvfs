/*
 * PowerManagementTest.c
 *
 *  Created on: 09 Sep, 2014
 *      Author: Alexandru Irimiea, alexandru.irim@gmail.com
 *      Usage:  -
 */

#include "PowerManagement.h"

static void initPowerManagementTest(void)
{

}

void initPowerManagement(void)
{
    initDroidPwMgmt();
}

static void parseArguments(int argc, char* argv[])
{
    switch(argc)
    {
        case 4:
            sampleCount = atoi(argv[3]);
        case 3:
            req.min = atoi(argv[1]);
            req.max = atoi(argv[2]);
            break;
        default:
            req.min = 55.00;
            req.max = 60.00;
            break;
    }

    printf("\nRequirements: FPS(%f, %f)\n", req.min, req.max);

    if((req.min < MIN_POSSIBLE_FPS) || (req.min > req.max) ||
        (req.max > MAX_POSSIBLE_FPS))
    {
        printf("\nInvalid requirements.\n");
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    //system("reset");

    initPowerManagementTest();
    parseArguments(argc, argv);

    initPowerManagement();
    savePower();

    return 0;
}
