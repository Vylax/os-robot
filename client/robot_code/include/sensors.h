#include "ev3.h"

/* get value from sensor */
int get_value_sonar(uint8_t component);
int get_value_gyro_ang(uint8_t component);
int get_value_gyro_rate(uint8_t component);
int get_value_color(uint8_t component);
int get_value_touch(uint8_t component);
int get_value_compass(uint8_t component);
/* reset sensor (where needed) */
void reset_sonar(uint8_t component);
void reset_gyro(uint8_t component);
void reset_color(uint8_t component);
