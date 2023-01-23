#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_servo.h"
#include "ev3_sensor.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)
//////////////////////////////////////////////////
#endif

#define PI 3.142857
#define WHEEL_DIAM 55 // Wheels' diameter is 5.5 cm

const char const *color[] = {"?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN"};
#define COLOR_COUNT ((int)(sizeof(color) / sizeof(color[0])))

static bool _check_pressed(uint8_t sn)
{
    int val;

    if (sn == SENSOR__NONE_)
    {
        return (ev3_read_keys((uint8_t *)&val) && (val & EV3_KEY_UP));
    }
    return (get_sensor_value(0, sn, &val) && (val != 0));
}

void grab_routine(uint8_t sn_arm, uint8_t sn_hand, int arm_v, int hand_v)
{
    /*	NOTE: Adapted to new implementation (rem. param. later)	*/
    int arm_t = 2300;
    /*  Raise the ball	*/
    printf("Raising the ball...\n");
    set_tacho_speed_sp(sn_arm, +arm_v);
    set_tacho_time_sp(sn_arm, arm_t);
    set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
    Sleep(arm_t);
    return;
}

void release_routine(uint8_t sn_hand, int hand_v)
{
    /*	NOTE: This should be now the throw function	*/
    int hand_t = 400;
    /* Start spinning */
    printf("Throwing the ball...\n");
    set_tacho_speed_sp(sn_hand, +hand_v);
    set_tacho_time_sp(sn_hand, hand_t);
    set_tacho_command_inx(sn_hand, TACHO_RUN_TIMED);
    Sleep(hand_t);
    return;
}

int main(void)
{
    // int i;
    uint8_t sn_left;
    uint8_t sn_right;
    uint8_t sn_arm;
    uint8_t sn_hand;
    // FLAGS_T state;
    uint8_t sn_touch;
    uint8_t sn_color;
    uint8_t sn_gyro;
    uint8_t sn_sonar;
    // uint8_t sn_mag;
    // char s[ 256 ];
    // int val;
    int count_per_rot;
    int arm_cpr, hand_cpr;
    int ball_found = 0;
    int max_speed;
    int arm_vmax, hand_vmax;
    int left_wheel_port = 65;
    int right_wheel_port = 66;
    int dc_port = 67;
    int servo_port = 68;
    int val;
    int completed;
    float value;
    float distance;
    float current_speed;
    float t;
    float speed_ratio = 0.1;
    // uint32_t n, ii;
#ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "10.192.59.99";

#endif
    if (ev3_init() == -1)
        return (1);

#ifndef __ARM_ARCH_4T__
    printf("The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr);

#else
    printf("Waiting tacho is plugged...\n");

#endif

    while (ev3_tacho_init() < 1)
        Sleep(1000); // Waiting for tachos being plugged

    printf("*** ( EV3 ) Hello! ***\n");

    /*        Left Wheel      */
    if (ev3_search_tacho_plugged_in(left_wheel_port, 0, &sn_left, 0))
    {

        printf("LEGO_EV3_M_MOTOR (LEFT) is found...\n");
        get_tacho_max_speed(sn_left, &max_speed);
        printf("  max speed = %d\n", max_speed);
        get_tacho_count_per_rot(sn_left, &count_per_rot);
        printf("  count_per_rot = %d\n", count_per_rot);
        set_tacho_stop_action_inx(sn_left, TACHO_COAST);
        // current_speed = max_speed * speed_ratio;
    }
    else
    {
        printf("LEGO_EV3_M_MOTOR (LEFT) is NOT found\n");
    }

    /*        Right wheel     */
    if (ev3_search_tacho_plugged_in(right_wheel_port, 0, &sn_right, 0))
    {

        printf("LEGO_EV3_M_MOTOR (RIGHT) is found...\n");
        get_tacho_max_speed(sn_right, &max_speed);
        printf("  max speed = %d\n", max_speed);
        get_tacho_count_per_rot(sn_left, &count_per_rot);
        printf("  count_per_rot = %d\n", count_per_rot);
        set_tacho_stop_action_inx(sn_right, TACHO_COAST);
    }
    else
    {
        printf("LEGO_EV3_M_MOTOR (RIGHT) is NOT found\n");
    }

    /*  Arm engine  */
    if (ev3_search_tacho_plugged_in(dc_port, 0, &sn_arm, 0))
    {
        /* Getting informations */
        printf("LEGO_EV3_M_MOTOR (ARM) is found...\n");
        get_tacho_max_speed(sn_arm, &arm_vmax);
        printf("  max speed = %d\n", arm_vmax);
        get_tacho_count_per_rot(sn_arm, &arm_cpr);
        printf("  count_per_rot = %d\n", arm_cpr);
        set_tacho_stop_action_inx(sn_arm, TACHO_COAST);
    }
    else
    {
        printf("LEGO_EV3_M_MOTOR (ARM) is NOT found\n");
    }

    /*  Hand engine  */
    if (ev3_search_tacho_plugged_in(servo_port, 0, &sn_hand, 0))
    {
        /*  Getting informations    */
        printf("LEGO_EV3_M_MOTOR (HAND) is found...\n");
        get_tacho_max_speed(sn_hand, &hand_vmax);
        printf("  max speed = %d\n", hand_vmax);
        get_tacho_count_per_rot(sn_hand, &hand_cpr);
        printf("  count_per_rot = %d\n", hand_cpr);
        set_tacho_stop_action_inx(sn_hand, TACHO_COAST);
    }
    else
    {
        printf("LEGO_EV3_M_MOTOR (HAND) is NOT found\n");
    }

    /*      --- Sensors ---     */
    /*  Initialize all the sensors  */
    ev3_sensor_init();

    /* Just throw */
    int pos, servo_rate;
    set_tacho_speed_sp(sn_arm, -arm_vmax);
    set_tacho_time_sp(sn_arm, 240);
    set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
    Sleep(240);
    set_tacho_speed_sp(sn_arm, +arm_vmax/5);
    set_tacho_time_sp(sn_arm, 400);
    set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);

    ev3_uninit();;
    return 0;
}
