/******************************************************************************
 *
 * Project: Hindernisumfahrung mit dem RP6
 * File:    main.c
 *
 * Ingenieurorientierte Programmierung
 * Dr. Alexander Kling
 *
 * Carsten Bevermann
 * Christoph Geitner
 * Jens Reimer
 * Marc Töpker
 *
 *****************************************************************************/

//#include <avr/io.h>
#include <RP6RobotBaseLib.h>

int main(void)
{
    initRobotBase();

    mSleep(1500);

    powerON(); // Encoder und Motorstromsensoren anschalten!

    while(1)
    {
        //
    }

    return 0;
}


/******************** End of file <main.c> ***********************************/
