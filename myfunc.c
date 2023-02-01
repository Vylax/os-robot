#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <stdio.h>
#include <math.h>



#ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "10.192.59.99";
#endif

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__
#include <windows.h>
// UNIX //////////////////////////////////////////
#else
#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)
//////////////////////////////////////////////////
#endif

//Constants declaration
#define WHEEL_RADIUS 27.5
#define PI 3.142857
#define POLLING_RATE 10
#define BALL_RADIUS 2.5 // TODO: get the actual value

#define INITIAL_CAPACITY 10 //Initial capacity for the Dynamic storage structure (List)

#define left_wheel_port 66
#define right_wheel_port 67
#define SONAR_PORT 0
#define COMPASS_PORT 4

// Variables used for objects detection
#define SENSOR_OFFSET_ANGLE 14.85 // TODO: Mesure the actual value (in degrees)
#define SENSOR_OFFSET_DIST 6.5 // TODO: Mesure the actual value (in cm)


uint8_t sn_left;
uint8_t sn_right;


void wait_motor_stop(uint8_t motor) {
 	FLAGS_T state;
 	do {
 	    get_tacho_state_flags( motor, &state );
 	} while ( state );
}



struct position {
  float x;
  float y;
  float dir;
} my_pos;


int main( void )
{
   
   printf("yes");
}

/*Function making the robot go ahead until it reaches the defined "cm" distance*/
void go_forwards_cm_foratime(uint8_t *motors, float cm, int speed) {
	
	my_pos.x = 0; /*Initialising position*/
	my_pos.y = 0;
	my_pos.dir = 0;

	float time = ((360.0*cm)/(2*M_PI*WHEEL_RADIUS)/speed)*10;
	
	set_tacho_speed_sp(sn_left, speed);
	set_tacho_speed_sp(sn_right, speed);
	set_tacho_time_sp( sn_right, time );
	set_tacho_time_sp( sn_left, time );
	set_tacho_command_inx( sn_right, TACHO_RUN_TIMED);
	set_tacho_command_inx( sn_left, TACHO_RUN_TIMED );
	wait_motor_stop(sn_left);
	wait_motor_stop(sn_right);

	my_pos.x = my_pos.x + cm * sin((my_pos.dir * M_PI) / 180.0);
	my_pos.y = my_pos.y + cm * cos((my_pos.dir * M_PI) / 180.0);
}

void go_backwards_cm_foratime(uint8_t *motors, float cm, int speed) {
	
	my_pos.x = 0; /*Initialising position*/
	my_pos.y = 0;
	my_pos.dir = 0;

	float time = ((360.0*cm)/(2*M_PI*WHEEL_RADIUS)/speed)*10;
	speed = -speed;
	printf("time %f \n", time);


	
	
	set_tacho_speed_sp(sn_left, speed);
	set_tacho_speed_sp(sn_right, speed);
	set_tacho_time_sp( sn_right, time );
	set_tacho_time_sp( sn_left, time );
	set_tacho_command_inx( sn_right, TACHO_RUN_TIMED);
	set_tacho_command_inx( sn_left, TACHO_RUN_TIMED );
	wait_motor_stop(sn_left);
	wait_motor_stop(sn_right);

	my_pos.x = my_pos.x + cm * sin((my_pos.dir * M_PI) / 180.0);
	my_pos.y = my_pos.y + cm * cos((my_pos.dir * M_PI) / 180.0);
}

void defense(uint8_t *motors, float cm, int speed) 
{
go_forwards_cm_foratime(motors, cm, speed);
go_backwards_cm_foratime(motors, cm, speed);
go_forwards_cm_foratime(motors, cm, speed);
}




#include <time.h>

void run_defense_for_4_minutes(uint8_t *motors, float cm, int speed) {
clock_t start = clock();
clock_t end = start + 240 * CLOCKS_PER_SEC;
while (clock() < end) {
defense(motors, cm, speed);
}
}