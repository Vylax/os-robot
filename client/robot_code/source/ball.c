/* 2023-01-23 The robot has now 2 motors related to the ball:
    1) One, called 'hand', it's used to grab the ball from the ground and put it in the shooting position
    2) The second one, called 'arm', it's used to shoot the ball */
#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_servo" //Actually all motors are seen as tachos
#include "ev3_tacho"
#include "ball.h"

/* Tells if the hand is currently grabbing a ball */
int ball_slot1 = 0;
/* Tells if there is a ball ready to be shot */
int ball_slot2 = 0;

/* TODO:    All values related to the 'hand' are
            purely indicative, they should be fixed     */

/*      Hand        */
int grabbing_speed = 500;
int grabbing_time = 200;
int risefall_speed = 500;
int risefall_time = 500;
/*      Arm         */
int shooting_speed = 1050;
int shooting_time = 240;
int shooting_cooldown = 400;

int initialize_arm(int servo_port, uint8_t *sn_arm)
{
    /* Initialization is made somewhere else...*/
    return 0;
}

int initialize_hand(int dc_port, uint8_t *sn_hand)
{
    /* Initialization is made somewhere else...*/
    return 0;
}

int shoot_ball(uint8_t sn_arm)
{
    /* NOTE: maybe we need to shoot based on sensor input */
    /*       i.e. don't shoot always at maximum range */
    if (ball_slot2 == 1)
    {
        set_tacho_speed_sp(sn_arm, -shooting_speed);
        set_tacho_time_sp(sn_arm, shooting_time);
        set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
        Sleep(shooting_time);
        set_tacho_speed_sp(sn_arm, +shooting_speed / 5);
        set_tacho_time_sp(sn_arm, shooting_cooldown);
        set_tacho_command_inx(sn_arm, TACHO_RUN_TIMED);
        // set flags
        ball_slot2 = 0;
    }
    else
    {
        if (ball_slot1 == 0)
            printf("Find a ball first!\n");
        if (ball_slot1 == 1)
            printf("Load the ball first!\n");
    }
    return 0;
}

int grab_ball(uint8_t sn_hand)
{
    /* Used to grab the ball from the ground */
    /* NOTE: this function does not imply rising the ball
             because maybe we are already carring another ball*/

    if (ball_slot1 == 0)
    {
        // step 1: go down
        set_tacho_speed_sp(sn_hand, risefall_speed);
        set_tacho_time_sp(sn_hand, risefall_time);
        set_tacho_command_inx(sn_hand, TACHO_RUN_TIMED);
        Sleep(risefall_time);
        // step 2: grab
        set_tacho_speed_sp(sn_hand, -grabbing_speed);
        set_tacho_time_sp(sn_hand, grabbing_time);
        set_tacho_command_inx(sn_hand, TACHO_RUN_TIMED);
        Sleep(grabbing_time);
        // update flags
        ball_slot1 = 1;
    }
    else
    {
        printf("Ball already grabbed!\n");
    }
    return 0;
}

int reload(uint8_t sn_hand)
{
    /* NOTE: use this function after take_ball
             so we can assume that the ball is already
             been grabbed */
    if (ball_slot1 == 1 && ball_slot2 == 0)
    {
        // step 1: go up
        set_tacho_speed_sp(sn_hand, -risefall_speed);
        set_tacho_time_sp(sn_hand, risefall_time);
        set_tacho_command_inx(sn_hand, TACHO_RUN_TIMED);
        Sleep(risefall_time);
        // step 2: release
        set_tacho_speed_sp(sn_hand, +grabbing_speed);
        set_tacho_time_sp(sn_hand, grabbing_time);
        set_tacho_command_inx(sn_hand, TACHO_RUN_TIMED);
        Sleep(grabbing_time);
        // update flags
        ball_slot1 = 0;
        ball_slot2 = 1;
    }
    else
    {
        if (ball_slot1 == 0 && ball_slot2 == 0)
            printf("Find a ball first!\n");
        if (ball_slot1 == 0 && ball_slot2 == 1)
            printf("Ball already loaded!\n");
        if (ball_slot1 == 1 && ball_slot2 == 1)
            printf("Shot the other ball first!\n");
    }
    return 0;
}