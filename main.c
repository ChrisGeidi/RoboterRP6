// Uncommented Version of RP6Base_Move_04.c
// written by Dominik S. Herwald
// ------------------------------------------------------------------------------------------

#include "RP6RobotBaseLib.h"

/* define block */

#define CRUISE_SPEED_FWD    	80
#define obstacle_SPEED_BWD    	60
#define obstacle_SPEED_ROTATE 	50

enum STATE
{
	IDLE,
	MOVE,
	FRONT_START,
    FRONT_WAIT ,
	LEFT_START ,
	LEFT_WAIT	,
	RIGHT_START,
	RIGHT_WAIT ,
	WAIT_END
} state;

/* struct für drive modes */
typedef struct {
	uint8_t  speed_left;
	uint8_t  speed_right;
	unsigned dir:2;
	unsigned move:1;
	unsigned rotate:1;
	uint16_t move_value;
	uint8_t  state;
} drive_Mode_t;

/* define drive modes */

drive_Mode_t DRIVE      = {CRUISE_SPEED_FWD, CRUISE_SPEED_FWD, FWD, false, false, 0, MOVE};
drive_Mode_t OBSTACLE 	= {0, 0, FWD, false, false, 0, IDLE};
drive_Mode_t STOP 		= {0, 0, FWD, false, false, 0, IDLE};



void mode_Drive(void)
{
}

void mode_Obstacle(void)
{
	static uint8_t bump_count = 0;
	switch(OBSTACLE.state)
	{
		// nichts machen
		case IDLE:
		break;


		// beide bumper aktiviert
		case FRONT_START:
			OBSTACLE.speed_left = obstacle_SPEED_BWD;
			OBSTACLE.dir = BWD;
			OBSTACLE.move = true;

			if(bump_count > 3)
				OBSTACLE.move_value = 180;
			else
				OBSTACLE.move_value = 120;
			OBSTACLE.state = FRONT_WAIT;
			bump_count+=2;
		break;

		// Fahrparameter an obstacle übergeben
		case FRONT_WAIT:
			if(!OBSTACLE.move)
			{
				OBSTACLE.speed_left = obstacle_SPEED_ROTATE;
				if(bump_count > 3)
				{
					OBSTACLE.move_value = 90;
					OBSTACLE.dir = RIGHT;
					bump_count = 0;
				}
				else
				{
					OBSTACLE.dir = LEFT;
					OBSTACLE.move_value = 50;
				}
				OBSTACLE.rotate = true;
				OBSTACLE.state = WAIT_END;
			}
		break;


		// linker bumper aktiviert
		case LEFT_START:
			OBSTACLE.speed_left = obstacle_SPEED_BWD;
			OBSTACLE.dir 	= BWD;
			OBSTACLE.move = true;
			if(bump_count > 3)
				OBSTACLE.move_value = 160;
			else
				OBSTACLE.move_value = 100;
			OBSTACLE.state = LEFT_WAIT;
			bump_count++;
		break;


		// Fahrparameter an obstacle übergeben
		case LEFT_WAIT:
			if(!OBSTACLE.move)
			{
				OBSTACLE.speed_left = obstacle_SPEED_ROTATE;
				OBSTACLE.dir = RIGHT;
				OBSTACLE.rotate = true;
				if(bump_count > 3)
				{
					OBSTACLE.move_value = 80;
					bump_count = 0;
				}
				else
					OBSTACLE.move_value = 45;
				OBSTACLE.state = WAIT_END;
			}
		break;

		// rechter bumper aktiviert
		case RIGHT_START:
			OBSTACLE.speed_left = obstacle_SPEED_BWD;
			OBSTACLE.dir 	= BWD;
			OBSTACLE.move = true;
			if(bump_count > 3)
				OBSTACLE.move_value = 160;
			else
				OBSTACLE.move_value = 100;
			OBSTACLE.state = RIGHT_WAIT;
			bump_count++;
		break;


		// Fahrparameter an obstacle übergeben
		case RIGHT_WAIT:
			if(!OBSTACLE.move)
			{
				OBSTACLE.speed_left = obstacle_SPEED_ROTATE;
				OBSTACLE.dir = LEFT;
				OBSTACLE.rotate = true;
				if(bump_count > 3)
				{
					OBSTACLE.move_value = 80;
					bump_count = 0;
				}
				else
					OBSTACLE.move_value = 45;
				OBSTACLE.state = WAIT_END;
			}
		break;


		// Move Parameter ausgeführt, Modus IDLE aktivieren
		case WAIT_END:
			if(!(OBSTACLE.move || OBSTACLE.rotate))
				OBSTACLE.state = IDLE;
		break;
	}
}

void bumpersStateChanged(void)
{
	if(bumper_left && bumper_right)
	{
		OBSTACLE.state = FRONT_START;
	}
	else if(bumper_left)
	{
		if(OBSTACLE.state != FRONT_WAIT)
			OBSTACLE.state = LEFT_START;
	}
	else if(bumper_right)
	{
		if(OBSTACLE.state != FRONT_WAIT)
			OBSTACLE.state = RIGHT_START;
	}
}

void driveCMD(drive_Mode_t * cmd)
{
	if(cmd->move_value > 0)
	{
		if(cmd->rotate)
			rotate(cmd->speed_left, cmd->dir, cmd->move_value, false);
		else if(cmd->move)
			move(cmd->speed_left, cmd->dir, DIST_MM(cmd->move_value), false);
		cmd->move_value = 0;
	}
	else if(!(cmd->move || cmd->rotate))
	{
		changeDirection(cmd->dir);
		moveAtSpeed(cmd->speed_left,cmd->speed_right);
	}
	else if(isMovementComplete())
	{
		cmd->rotate = false;
		cmd->move = false;
	}
}


void stateModel(void)
{
	mode_Drive();
	mode_Obstacle();

	// Hindernis oder Fahrmodus
	if(OBSTACLE.state != IDLE)
		driveCMD(&OBSTACLE);
	else if(DRIVE.state != IDLE)
		driveCMD(&DRIVE);
	// Stop
	else
		driveCMD(&STOP);
}

int main(void)
{
	initRobotBase();
	mSleep(2500);

	BUMPERS_setStateChangedHandler(bumpersStateChanged);

	powerON();

	while(true)
	{
		stateModel();
		task_RP6System();
	}
	return 0;
}
