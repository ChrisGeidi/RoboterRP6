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
#include "RP6RobotBaseLib.h"

int destination = 1000;

enum state_model
{
    IDLE,
    BUMPER_LEFT,
    BUMPER_RIGHT,
    MOVE_TO_DESTINATION,
} state;


void bumperActive(void)
{
    destination = destination-(mleft_dist+mright_dist)/2;

    if (bumper_left)
    {
        //stop();
        state = BUMPER_LEFT;
    }
    if (bumper_right)
    {
        //stop();
        state = BUMPER_RIGHT;
    }
}

void stateModel(void)
{
	switch(state)
	{
		case IDLE:
		     stop();
		break;

		case MOVE_TO_DESTINATION:

             move(1000, FWD, DIST_MM(destination),true);
             if(isMovementComplete())
            {
                state=IDLE;
            }

        break;

		case BUMPER_LEFT:
            move(150, BWD, DIST_MM(200), true);
            stop();
            rotate(150, RIGHT, 90, true); // um 180° nach links drehen
            stop();
            move(150, FWD, DIST_MM(200), true);
            stop();
            rotate(150, LEFT, 90, true); // um 180° nach links drehen
            stop();
            if(isMovementComplete())
            {
                state=MOVE_TO_DESTINATION;
            }
		break;

        case BUMPER_RIGHT:
            move(150, BWD, DIST_MM(200), true);
            stop();
            rotate(150, LEFT, 90, true); // um 180° nach links drehen
            stop();
            move(150, FWD, DIST_MM(200), true);
            stop();
            rotate(150, RIGHT, 90, true); // um 180° nach links drehen
            stop();
            if(isMovementComplete())
            {
                state=MOVE_TO_DESTINATION;
            }
		break;

	}
}

int main(void)
{
	initRobotBase();
	mSleep(1000);
	powerON();
	//initial value
    state = MOVE_TO_DESTINATION;



	while(true)
	{
	    BUMPERS_setStateChangedHandler(bumperActive);
		stateModel();
		task_RP6System();

	}
	return 0;
}

/******************** End of file <main.c> ***********************************/
