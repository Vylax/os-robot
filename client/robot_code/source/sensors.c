#include <stdio.h>
#include "../include/sensors.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"

#define SENSOR_NUMBER 5

/* Update if including more sensors */

const char sensor_names[SENSOR_NUMBER][16] = {"LEGO_EV3_US","LEGO_EV3_GYRO","LEGO_EV3_COLOR","LEGO_EV3_TOUCH","HT_NXT_COMPASS"};

/* GET VALUES */

int get_value_sonar(uint8_t port) {
    /* Sonar: distance in cm, (0, 2550) */
    /* NOTE: the guide says it's expressed in cm, but those are likely mm. Just run a quick test to check */
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

int get_value_gyro_ang(uint8_t port) {
    /* Gyro angle: expressed in degrees, (-32768, 32767) */
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

int get_value_gyro_rate(uint8_t port) {
    /* Gyro rotational speed: expressed in deg/s, (-440, 440) */
    float buf;
    get_sensor_value1(port, &buf);
    return (int)buf;
}

int get_value_color(uint8_t port) {
    /* Color: integer between (0, 7) */
    float buf;
    get_sensor_value1(port, &buf);
    return (int)buf;
}

int get_value_touch(uint8_t port) {
    /* Touch: 1 if pressed, 0 otherwise */
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

int get_value_compass(uint8_t port) {
    /* Compass angle: expressed in degrees, (0, 359) */
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

/* RESET FUNCTIONS */

void reset_sonar(uint8_t port) {
    set_sensor_mode(port, "US-DIST-CM");
    printf("Sonar reset: actual sensor mode: %s\n", ev3_sensor_mode(port));
    return;
}

void reset_gyro(uint8_t port) {
    set_sensor_mode(port, "GYRO-CAL");
    set_sensor_mode(port, "GYRO-G&A");
    printf("Gyro reset: actual sensor mode: %s\n", ev3_sensor_mode(port) );
    return;
}

void reset_color(uint8_t port) {
    set_sensor_mode(port, "COL-COLOR");
    printf("Color reset: actual sensor mode: %s\n", ev3_sensor_mode(port) );
    return;
}
