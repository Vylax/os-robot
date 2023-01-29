#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "init.h"
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

/* The easiest way to implement a unified init for sensors and motors,
   with fixed positions. */
/*
    ports[0] = Sonar
    ports[1] = Gyro
    ports[2] = Color
    ports[3] = Touch
    ports[4] = Compass
    ports[5] = Left motor
    ports[6] = Right motor
    ports[7] = Arm
    ports[8] = Hand
*/

int left_wheel_port = 65;
int right_wheel_port = 66;
int hand_port = 67;
int arm_port = 68;

static const int sensor_names[5] = {LEGO_EV3_US, LEGO_EV3_GYRO, LEGO_EV3_COLOR, LEGO_EV3_TOUCH, HT_NXT_COMPASS};

void robot_init()
{

// #ifndef __ARM_ARCH_4T__
//     /* Disable auto-detection of the brick (you have to set the correct address below) */
//     ev3_brick_addr = "10.192.59.99";
// #endif
    // initialize the robot
if (ev3_init() == -1)
{
    printf("ev3 initialization failed!\n");
    exit(EXIT_FAILURE);
}
#ifndef __ARM_ARCH_4T__
    printf("The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr);
#else
    printf("Waiting tacho is plugged...\n");
#endif

    // init motors & sensors
    ev3_tacho_init();
    ev3_sensor_init();
    /* Find ports related to sensors */
    for (int i = 0; i < 5; i++)
    {
        ev3_search_sensor(sensor_names[i], &components[i] , 0);
    }
    /* Left Wheel */
    if (!ev3_search_tacho_plugged_in(left_wheel_port, 0, &components[5], 0))
    {
        printf("Left wheel motor not found!\n");
        exit(EXIT_FAILURE);
    }
    /* right Wheel */
    if (!ev3_search_tacho_plugged_in(right_wheel_port, 0, &components[6], 0))
    {
        printf("Right wheel motor not found!\n");
        exit(EXIT_FAILURE);
    }
    /* Arm */
    if (!ev3_search_tacho_plugged_in(arm_port, 0, &components[7], 0))
    {
        printf("Arm motor not found!\n");
        exit(EXIT_FAILURE);
    }
    /* Hand */
    if (!ev3_search_tacho_plugged_in(hand_port, 0, &components[8], 0))
    {
        printf("Hand motor not found!\n");
        exit(EXIT_FAILURE);
    }
    return;
}

void robot_uninit()
{
    ev3_uninit();
    return;
}
