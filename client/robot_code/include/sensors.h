#ifndef HEADER_SENSORS
#define HEADER_SENSORS

extern s[];

/* get value from sensor */
int get_value_sonar();
int get_value_gyro_ang();
int get_value_gyro_rate();
int get_value_color();
int get_value_touch();
int get_value_compass();
/* reset sensor (where needed) */
void reset_sonar();
void reset_gyro();
void reset_color();

#endif