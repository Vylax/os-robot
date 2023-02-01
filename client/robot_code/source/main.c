#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include ".../include/ball.h"
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
    int distance;

    get_sonar_value(&distance);
    t = cal_run_time(components[LEFT_MOTOR], distance, SPEED);
    move_timed(SPEED, t);
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
    int speed_forward, speed_backward, speed_turning;
    int t1, t2, t3, t4, t5, t6;

    speed_forward = 400;
    speed_backward = 300;
    speed_turning = 200;

    // Todo: implement the strategy
    // 1)from the start position, move forward for 65cm 
    t1 = 200;
    move_timed(speed_forward, t1, components);
    
    // 2)turn right to +90 
    t2 = 200;
    turn_right(speed_turning, t2, components);
    
    // 3)move backward for 40cm 
    t3 = 200;
    move_timed(speed_forward, t3, components);
    
    // (get the 1st ball) 
    
    
    // 4) turn right to +180 
    t4 = 200;
    turn_right(speed_turning, t4, components);
    
    // 5) move backward for 50cm 
    t5 = 200;
    move_timed(speed_forward, t5, components);
    
    // (get the second ball) 
    

    // 6)  moving forward 60cm (to the shooting point) 
    t6 = 200;
    move_timed(speed_forward, t6, components);

    // 7) turn left to 0 (shoot)
    t7 = 400;
    turn_left(speed_turning, t7, components);
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