#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "sensors.h"

#define SENSOR_NUMBER 5

enum {SONAR, GYRO, COLOR, TOUCH, COMPASS, LEFT_MOTOR, RIGHT_MOTOR, ARM, HAND};


/* Update if including more sensors */
const char sensor_names[SENSOR_NUMBER][16] = {"LEGO_EV3_US","LEGO_EV3_GYRO","LEGO_EV3_COLOR","LEGO_EV3_TOUCH","HT_NXT_COMPASS"};

/* GET VALUES */

int get_value_sonar() {
    /* Sonar: distance in cm, (0, 2550) */
    /* NOTE: the guide says it's expressed in cm, but those are likely mm. Just run a quick test to check */
    float buf;
    get_sensor_value0(components[SONAR], &buf);
    return (int)buf;
}

int get_value_gyro_ang() {
    /* Gyro angle: expressed in degrees, (-32768, 32767) */
    float buf;
    get_sensor_value0(components[GYRO], &buf);
    return (int)buf;
}

int get_value_gyro_rate() {
    /* Gyro rotational speed: expressed in deg/s, (-440, 440) */
    float buf;
    get_sensor_value1(components[GYRO], &buf);
    return (int)buf;
}

int get_value_color() {
    /* Color: integer between (0, 7) */
    float buf;
    get_sensor_value1(components[COLOR], &buf);
    return (int)buf;
}

int get_value_touch() {
    /* Touch: 1 if pressed, 0 otherwise */
    float buf;
    get_sensor_value0(components[TOUCH], &buf);
    return (int)buf;
}

int get_value_compass() {
    /* Compass angle: expressed in degrees, (0, 359) */
    float buf;
    get_sensor_value0(components[COMPASS], &buf);
    return (int)buf;
}

/* RESET FUNCTIONS */

void reset_sonar() {
    char outputMode[32];
    set_sensor_mode(components[SONAR], "US-DIST-CM");
    printf("Sonar reset: actual sensor mode: %s\n", get_sensor_mode(components[SONAR],outputMode,32));
    return;
}

void reset_gyro() {
    char outputMode[32];
    set_sensor_mode(components[GYRO], "GYRO-CAL");
    set_sensor_mode(components[GYRO], "GYRO-G&A");
    printf("Gyro reset: actual sensor mode: %s\n", get_sensor_mode(components[GYRO],outputMode,32));
    return;
}

void reset_color() {
    char outputMode[32];
    set_sensor_mode(components[COLOR], "COL-COLOR");
    printf("Color reset: actual sensor mode: %s\n", get_sensor_mode(components[COLOR],outputMode,32));
    return;
}
