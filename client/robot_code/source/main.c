#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "../include/ball.h"
#include "../include/sensors.h"
#include "../include/movement.h"
#include "../include/init.h"
#include "../include/utils.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)
//////////////////////////////////////////////////
#endif

enum {SONAR, GYRO, COLOR, TOUCH, COMPASS, LEFT_MOTOR, RIGHT_MOTOR, ARM, HAND};

uint8_t components[9];

void stop_handler()
{
    stop(TACHO_COAST);
    exit(0);
}

/// @brief Move forward and get the ball
void test1()
{
    const int SPEED = 800;
    int distance, t;

    get_sonar_value(&distance);
    t = cal_run_time(components[LEFT_MOTOR], distance, SPEED);
    move_timed(SPEED, t, components);
    Sleep(t);
    // Todo
    grab_routine();
    
}

/// @brief Shoot a ball already in hand to the basket
void test2()
{
    //aim_routine();
    //throw_routine();
}

/// @brief Scan the environment to find the ball and go get it
void test3()
{
    //turn_robot();
    //test1();
}

/// @brief Scan the environment to find the ball, go get it and throw it
void test4()
{
    //test3();
    //test2();
}

/// @brief Behave as an attacker
void attacker()
{
    // Todo: implement the strategy
}

/// @brief  Behave as an defender
void defender()
{
    // Todo: implement the startegy
}

int main()
{
    /* Initialize sensors & motors */
    robot_init(components);

    // movement test from movement.c
    movement_test(components);

    // test1()

    // test2()

    // test3()

    // attacker()

    // defender()

    /* Uninitialize the robot */
    robot_uninit();
    return 0;
}