#include "sensors.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"

#define SENSOR_NUMBER 4;

/* Update if including more sensors */

const char sensor_names[SENSOR_NUMBER][16] = {'LEGO_EV3_US','LEGO_EV3_GYRO','LEGO_EV3_COLOR','LEGO_EV3_TOUCH'};

/* INIT/UNINIT FUNCTIONS */

void init_sensors(uint8_t** ports) {
    *ports = (int *) malloc(SENSOR_NUMBER * sizeof(uint_8));
    ev3_init();
    for(int i = 0; i < SENSOR_NUMBER; i++) {
        ev3_search_sensor(sensor_names[i], &((*ports)[i]) , 0);
    }
    return;
}

void uninit_sensors(uint8_t** ports) {
    free(*ports);
    ev3_uninit();
    return;    
}

/* GET VALUES */

int get_value_sonar(uint8_t port) {
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

int get_value_gyro_ang(uint8_t port) {
    /* Angle: expressed in degrees, (-32768, 32767) */
    float buf;
    get_sensor_value0(port, &buf);
    return (int)buf;
}

int get_value_gyro_rate(uint8_t port) {
    /* Rotational speed: expressed in deg/s, (-440, 440) */
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
