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
} rp6_set;

void bumpersStateChanged(void)
{
    if(bumper_left)
    rotate(50, LEFT, 180, true);

    if(bumper_right)
    rotate(50, RIGHT, 180, true);
}



int main(void)
{
    initRobotBase();

    mSleep(1500);

    powerON(); // Encoder und Motorstromsensoren anschalten!

    while(1)
    {
        BUMPERS_setStateChangedHandler(bumpersStateChanged);
    }

    return 0;
}


/******************** End of file <main.c> ***********************************/
