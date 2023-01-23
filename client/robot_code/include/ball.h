#include "ev3_tacho.h"

int initialize_arm(int servo_port, uint8_t *sn_arm);

int initialize_hand(int dc_port, uint8_t *sn_hand);

int shoot_ball(uint8_t sn_arm);

int grab_ball(uint8_t sn_hand);

int reload(uint8_t sn_hand);