#include "encoder.h"

#define LOG_MODULE_NAME ENCODER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

void test_me_3(){
    LOG_INF("dsadsahehhehehe");
}

int get_encoder(){
    static int value = -1;
    value+=1;
    return value;
}