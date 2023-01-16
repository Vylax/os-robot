#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "utils.c"

uint8_t sn_gyro;
uint8_t sn_sonar;
uint8_t sn_color;
uint8_t sn_touch;

ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0);
ev3_search_sensor(LEGO_EV3_US, &sn_sonar, 0);
ev3_search_sensor(LEGO_EV3_COLOR, &sn_color, 0);
ev3_search_sensor(LEGO_EV3_TOUCH, &sn_touch, 0);

/// Value in 
void get_gyro_value(int *value_buf)
{
    get_sensor_value(0, sn_gyro, value_buf);
}

/// Value in (0-2550) cm
void get_sonar_value(int *value_buf)
{
    get_sensor_value(0, sn_sonar, value_buf);
}

/// Value in (0-100)
void get_color_value(int *value_buf)
{
    get_sensor_value(0, sn_color, value_buf);
}

bool _check_pressed()
{
    int val;
    return (get_sensor_value(0, sn_touch, &val) && (val != 0));
}
