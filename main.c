#include "RP6RobotBaseLib.h"

/* define block */

#define SPEED_FWD   80
#define SPEED_BWD   60
#define SPEED_ROTATE    50

enum STATE
{
	IDLE,
	DRIVE_TARGET,
	OBSTACLE_FWD,
	OBSTACLE_BWD,
	OBSTACLE_LEFT,
	OBSTACLE_LEFT_TWO,
	OBSTACLE_RIGHT,
	OBSTACLE_RIGHT_TWO
} RP6_state;

/* struct für drive modes */
typedef struct {
	uint8_t  speed_left;
	uint8_t  speed_right;
	unsigned move:1;
	uint16_t move_value;
	uint8_t  state;
	uint8_t  Obstacle_state;
} drive_Mode_t;

/* define drive modes */

drive_Mode_t DRIVE      = {SPEED_FWD, SPEED_FWD, false, 200,DRIVE_TARGET,IDLE};



void mode_Obstacle(void)
{
    switch(RP6_state)
    {
        case IDLE:
        break;

        case DRIVE_TARGET:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=1000;
            DRIVE.state=IDLE;
        }
        break;

        case OBSTACLE_BWD:
        DRIVE.speed_left=50;
        DRIVE.speed_right=50;
        DRIVE.move=true;
        DRIVE.move_value=200;

        if(DRIVE.Obstacle_state==1)
        DRIVE.state=OBSTACLE_RIGHT;
        if(DRIVE.Obstacle_state==2)
        DRIVE.state=OBSTACLE_LEFT;
        break;

        case OBSTACLE_FWD:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=400;

            if(DRIVE.Obstacle_state==1)
            DRIVE.state=OBSTACLE_RIGHT_TWO;
            if(DRIVE.Obstacle_state==2)
            DRIVE.state=OBSTACLE_LEFT_TWO;
        }
        break;

        case OBSTACLE_RIGHT:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=0;
            DRIVE.state=OBSTACLE_FWD;
        }
        break;

        case OBSTACLE_LEFT:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=0;
            DRIVE.state=OBSTACLE_FWD;
        }

        break;

        case OBSTACLE_LEFT_TWO:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=0;
            DRIVE.state=DRIVE_TARGET;
        }

        break;

        case OBSTACLE_RIGHT_TWO:
        if(DRIVE.move==false)
        {
            DRIVE.speed_left=50;
            DRIVE.speed_right=50;
            DRIVE.move=true;
            DRIVE.move_value=0;
            DRIVE.state=DRIVE_TARGET;
        }
        break;
    }
}


void bumpersStateChanged(void)
{

	if(bumper_left)
    {
        RP6_state=OBSTACLE_BWD;
        DRIVE.Obstacle_state=1;
    }
    if(bumper_right)
    {
        RP6_state=OBSTACLE_BWD;
        DRIVE.Obstacle_state=2;
    }
}

void stateModel()
{
    mode_Obstacle();

    if(DRIVE.state==IDLE)
        stop();

    if(DRIVE.state==OBSTACLE_FWD||DRIVE.state==DRIVE_TARGET)
        {move(DRIVE.speed_left,FWD,DRIVE.move_value,true);
        stop();}

    if(DRIVE.state==OBSTACLE_BWD)
        {move(DRIVE.speed_left,BWD,DRIVE.move_value,true);
        stop();}

    if(DRIVE.state==OBSTACLE_LEFT_TWO||DRIVE.state==OBSTACLE_RIGHT)
        {rotate(DRIVE.speed_left,LEFT,90,true);
        stop();}

    if(DRIVE.state==OBSTACLE_LEFT||DRIVE.state==OBSTACLE_RIGHT_TWO)
        {rotate(DRIVE.speed_left,RIGHT,90,true);
         stop();}

    if(isMovementComplete())
        DRIVE.move=false;
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
