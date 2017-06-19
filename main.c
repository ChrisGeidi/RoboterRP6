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

#define speedfwd 399
#define angle 50
#define dist 20


int destination = DIST_MM(100);

enum state_model
{
    IDLE,
    BUMPER_LEFT,
    BUMPER_RIGHT,
    MOVE_TO_DESTINATION,
} state;


void bumperActive(void)
{
    if (bumper_left)
    {
        state = BUMPER_LEFT;
    }
    if (bumper_right)
    {
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
            move(speedfwd, FWD, destination,true);

            if(isMovementComplete())
			{
                state = IDLE;
			}
        break;

		case BUMPER_LEFT:
		    destination = DIST_MM(getLeftDistance());
            move(speedfwd, BWD, DIST_CM(dist), true);
            rotate(speedfwd, RIGHT, angle, true); // um 180° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, LEFT, angle, true); // um 180° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, LEFT, angle, true); // um 180° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, RIGHT, angle, true); // um 180° nach links drehen

            state = MOVE_TO_DESTINATION;
		break;

        case BUMPER_RIGHT:
            destination = DIST_MM(getLeftDistance());
            move(speedfwd, BWD, DIST_CM(dist), true);
            rotate(speedfwd, LEFT, angle, true); // um 90° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, RIGHT, angle, true); // um 180° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, RIGHT, angle, true); // um 180° nach links drehen
            move(speedfwd, FWD, DIST_CM(dist), true);
            rotate(speedfwd, LEFT, angle, true); // um 180° nach links drehen

            state = MOVE_TO_DESTINATION;

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
    BUMPERS_setStateChangedHandler(bumperActive);

	while(true)
	{
	    bumperActive();
		stateModel();
		task_RP6System();
	}
	return 0;
}

/******************** End of file <main.c> ***********************************/
