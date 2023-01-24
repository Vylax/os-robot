/* TODO fix the includes if nec. */
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"


/* init/uninit functions */
void init_sensors(uint8_t** ports);
void uninit_sensors(uint8_t** ports);
/* get value from sensor */
int get_value_sonar(uint8_t port);
int get_value_gyro_ang(uint8_t port);
int get_value_gyro_rate(uint8_t port);
int get_value_color(uint8_t port);
int get_value_touch(uint8_t port);
/* reset sensor (where needed) */
void reset_sonar(uint8_t port);
void reset_gyro(uint8_t port);
void reset_color(uint8_t port);
