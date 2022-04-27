#include <logging/log.h>
#include "servo.h"
#include <nrfx_qdec.h>



void test_me_3();
int init_encoder_servos();
int init_encoder_azimuth();
int init_encoder_elevation();
void update_encoder(int N);
void angle_slow_move(int N, uint32_t angle);
