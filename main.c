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
enum Zustand
{
    IDLE,
    TURN_LEFT,
    TURN_RIGHT,
    WAITING
};
struct RP6_State
{
    enum Zustand state;
}rp6_state;
struct RP6_settings
{
    uint8_t rotate_speed;
    uint16_t angle;
    uint8_t dir;
    uint8_t block;
    uint8_t complete;
    uint8_t warning;
}
rp6_set;

uint8_t move_state = rp6_state.IDLE;

void move_stateMachine(void)
{
	switch( )
	{
		case STATE_START:
			setLEDs(0b010000);
			move_state = STATE_WAIT_FOR_BUMPER_HIT_LEFT;
		break;

		case STATE_WAIT_FOR_BUMPER_HIT_LEFT:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED5 = !statusLEDs.LED5;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			if(bumper_left)
			{
				setLEDs(0b011001);
				move(50, BWD, DIST_MM(150), NON_BLOCKING);
				move_state = STATE_MOVE_BACKWARDS;
			}
		break;
		case STATE_MOVE_BACKWARDS:
			if(isMovementComplete())
			{
				setLEDs(0b000010);
				move_state = STATE_WAIT_FOR_BUMPER_HIT_RIGHT;
			}
		break;

		case STATE_WAIT_FOR_BUMPER_HIT_RIGHT:
			if(getStopwatch1() > 500)
			{
				statusLEDs.LED2 = !statusLEDs.LED2;
				updateStatusLEDs();
				setStopwatch1(0);
			}
			if(bumper_right)
			{
				setLEDs(0b100110);
				move(50, FWD, DIST_MM(150), NON_BLOCKING);
				move_state = STATE_MOVE_FORWARDS;
			}
		break;
		case STATE_MOVE_FORWARDS:
			if(isMovementComplete())
				move_state = STATE_START;
		break;
	}
}




int main(void)
{
	initRobotBase();
	setLEDs(0b111111);
	mSleep(1000);

	powerON();

	startStopwatch1();

	while(true)
	{
		move_stateMachine();
		task_RP6System();
	}
	return 0;
}


/******************** End of file <main.c> ***********************************/
