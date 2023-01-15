#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

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
#define left_wheel_port 66
#define right_wheel_port 67

void turn_robot(int angle) {
    // Set the motors to "coast" mode
    set_tacho_stop_action_inx(left_wheel_port, TACHO_COAST);
    set_tacho_stop_action_inx(right_wheel_port, TACHO_COAST);

    // Calculate the number of degrees to rotate each wheel
    float wheel_rotations = angle / (2 * PI * WHEEL_RADIUS);
    int left_degrees = (int)(-1 * wheel_rotations * 360);
    int right_degrees = (int)(wheel_rotations * 360);

    // Set the motors to rotate to the calculated number of degrees
    set_tacho_position_sp(left_wheel_port, left_degrees);
    set_tacho_position_sp(right_wheel_port, right_degrees);

    // Start the motors
    set_tacho_command_inx(left_wheel_port, TACHO_RUN_TO_REL_POS);
    set_tacho_command_inx(right_wheel_port, TACHO_RUN_TO_REL_POS);

    // Wait for the motors to finish
    while (get_tacho_state(left_wheel_port) != TACHO_STATE_HOLD) {
        Sleep(10);
    }
    while (get_tacho_state(right_wheel_port) != TACHO_STATE_HOLD) {
        Sleep(10);
    }
}

int main( void )
{
    if ( ev3_init() == -1 ) return ( 1 );

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );  // Waiting for tachos being plugged

    printf( "*** ( EV3 ) Hello! ***\n" );

    int angle = 90; //example

    turn_robot(angle,left_wheel_port,right_wheel_port);
    return 0;
}
