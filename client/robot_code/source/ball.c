#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo"
#include "ball.h"

/* State register of the arm */
static int state = 0;
const int length = 1; //must be calibrated9

int initialize_arm(int servo_port, uint8_t *sn_arm)
{
    /*  Arm engine  */
    int position;
    /* Initialize the servo */
    ev3_servo_init();

    /* Search for the servo in the given port */
    if (ev3_search_servo_plugged_in(servo_port, 0, sn_arm, 0))
    {
        printf("Servo motor is found, getting position...\n");
        /* TODO: reset at beginning */
        /* For now, always start the robot with the ARM RAISED AT THE TOP!!! */
        /* Prints current position */
        if (get_servo_position_sp(sn, &pos))
        {
            printf("position: %d\n", pos);
        }
    }
    else
    {
        printf("Servo motor is NOT found\n");
        return -1;
    }

    return 0;
}

int test_arm(uint8_t sn_arm)
{
    /*  The position values are percentages (from -100%, 100% in this case)
        at the beginning of each test, we should put the arm at the top
        and then set the values accordingly; For instance, if we start
        with the arm raised to the top, and we get position 85%, and we want to lower the arm, we just
        get the actual position, and then set the new one like this:
            set_servo_position_sp(sn, actual_position - 40);
        This will 
    */
    printf("Checking if arm is working...\n");
    set_servo_position_sp(sn, 100);
    set_servo_command_inx(sn, SERVO_RUN);
    do
    {
        get_servo_state_flags(sn, &state);
    } while (state);
    if (get_servo_position_sp(sn, &pos))
    {
        printf("position: %d\n", pos);
    }
    set_servo_position_sp(sn, -100);
    do
    {
        get_servo_state_flags(sn, &state);
    } while (state);
    if (get_servo_position_sp(sn, &pos))
    {
        printf("position: %d\n", pos);
    }
    set_servo_command_inx(sn, SERVO_FLOAT);
    return 0;
}

int lift_arm(uint8_t sn_arm, int arm_v)
{
    /*  Go up	*/
    printf("Raising the arm...\n");
    set_tacho_speed_sp(sn_arm, +arm_v);
    set_tacho_time_sp(sn_arm, length / arm_v);
    set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
    Sleep(length / arm_v);
    return 0;
}

int lower_arm(uint8_t sn_arm, int arm_v)
{
    /*  Go up	*/
    printf("Raising the arm...\n");
    set_tacho_speed_sp(sn_arm, +arm_v);
    set_tacho_time_sp(sn_arm, length / arm_v);
    set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
    Sleep(length / arm_v);
    return 0;
}
