#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "../include/sensors.h"

#define SENSOR_NUMBER 5

enum {SONAR, GYRO, COLOR, TOUCH, COMPASS, LEFT_MOTOR, RIGHT_MOTOR, ARM, HAND};


/* Update if including more sensors */

const char sensor_names[SENSOR_NUMBER][16] = {"LEGO_EV3_US","LEGO_EV3_GYRO","LEGO_EV3_COLOR","LEGO_EV3_TOUCH","HT_NXT_COMPASS"};

/* GET VALUES */

int get_value_sonar(uint8_t component) {
    /* Sonar: distance in cm, (0, 2550) */
    /* NOTE: the guide says it's expressed in cm, but those are likely mm. Just run a quick test to check */
    float buf;
    get_sensor_value0(component, &buf);
    return (int)buf;
}

int get_value_gyro_ang(uint8_t component) {
    /* Gyro angle: expressed in degrees, (-32768, 32767) */
    float buf;
    get_sensor_value0(component, &buf);
    return (int)buf;
}

int get_value_gyro_rate(uint8_t component) {
    /* Gyro rotational speed: expressed in deg/s, (-440, 440) */
    float buf;
    get_sensor_value1(component, &buf);
    return (int)buf;
}

int get_value_color(uint8_t component) {
    /* Color: integer between (0, 7) */
    float buf;
    get_sensor_value1(component, &buf);
    return (int)buf;
}

int get_value_touch(uint8_t component) {
    /* Touch: 1 if pressed, 0 otherwise */
    float buf;
    get_sensor_value0(component, &buf);
    return (int)buf;
}

int get_value_compass(uint8_t component) {
    /* Compass angle: expressed in degrees, (0, 359) */
    float buf;
    get_sensor_value0(component, &buf);
    return (int)buf;
}

/* RESET FUNCTIONS */

void reset_sonar(uint8_t component) {
    set_sensor_mode(component, "US-DIST-CM");
    printf("Sonar reset: actual sensor mode: %s\n", ev3_sensor_mode(component));
    return;
}

void reset_gyro(uint8_t component) {
    set_sensor_mode(component, "GYRO-CAL");
    set_sensor_mode(component, "GYRO-G&A");
    printf("Gyro reset: actual sensor mode: %s\n", ev3_sensor_mode(component) );
    return;
}

void reset_color(uint8_t component) {
    set_sensor_mode(component, "COL-COLOR");
    printf("Color reset: actual sensor mode: %s\n", ev3_sensor_mode(component) );
    return;
}
