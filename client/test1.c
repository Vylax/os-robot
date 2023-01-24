#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "init.h"
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

int main(void)
{
    /* Components */
    uint8_t sn_sonar;           //components[0]
    uint8_t sn_gyro;            //components[1]
    uint8_t sn_color;           //components[2]
    uint8_t sn_touch;           //components[3]
    uint8_t sn_compass;         //components[4]
    uint8_t sn_left;            //components[5]
    uint8_t sn_right;           //components[6]
    uint8_t sn_arm;             //components[7]
    uint8_t sn_hand;            //components[8]
    uint8_t components[9];
    /* Values */
    int count_per_rot;
    int arm_cpr, hand_cpr;
    int ball_found = 0;
    int max_speed;
    int arm_vmax, hand_vmax;
    int val;
    int completed;
    float value;
    float distance;
    float current_speed;
    float t;
    float speed_ratio = 0.1;


    /* Initialize robot*/
    robot_init(components);
    /* Extract needed values */
    sn_sonar = components[0];
    sn_left = components[5];
    sn_right = components[6];
    sn_arm = components[7];
    sn_hand = components[8];

    /* Step 1: Use sonar to approach the ball & get in position */
    //  TODO: create a movement function to do this

    /* Step 2: Grab the ball */
    grab_ball(sn_hand);

    /* Step 3: Raise the ball (shooting position)*/
    reload(sn_hand);
    
    /* Uninitialize robot*/
    robot_uninit();
    /*      END     */
    return 0;
}
