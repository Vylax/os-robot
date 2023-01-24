#include "init.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
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
const char sensor_names[5][16] = {'LEGO_EV3_US', 'LEGO_EV3_GYRO', 'LEGO_EV3_COLOR', 'LEGO_EV3_TOUCH', 'HT_NXT_COMPASS'};

void robot_init(uint8_t *components)
{
    // init motors & sensors
    ev3_tacho_init();
    ev3_sensor_init();
    /* Find ports related to sensors */
    for (int i = 0; i < 5; i++)
    {
        ev3_search_sensor(sensor_names[i], &components[i]) , 0);
    }
    /* Left Wheel */
    if (!ev3_search_tacho_plugged_in(left_wheel_port, 0, &components[5], 0))
    {
        printf("Left wheel motor not found!\n");
        return -1;
    }
    /* right Wheel */
    if (!ev3_search_tacho_plugged_in(right_wheel_port, 0, &components[6], 0))
    {
        printf("Right wheel motor not found!\n");
        return -1;
    }
    /* Arm */
    if (!ev3_search_tacho_plugged_in(arm_port, 0, &components[7], 0))
    {
        printf("Arm motor not found!\n");
        return -1;
    }
    /* Hand */
    if (!ev3_search_tacho_plugged_in(hand_port, 0, &components[8], 0))
    {
        printf("Hand motor not found!\n");
        return -1;
    }
    return;
}

void robot_uninit()
{
    ev3_uninit();
    return;
}