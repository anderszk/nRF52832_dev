#include "search.h"

#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;

void test_me(){
    LOG_INF("hehhehehe");
}

int sweep_search(bool state, int min_encoder_search, int max_encoder_search, int increment){
    int readings[270][3] = {0};
    int index = 0;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        //move_servo(state, i) bevege servo enten i asimuth til enkoder verdi i
        k_sem_take(&my_sem, K_FOREVER);
        set_observer(true);
        k_sem_take(&my_sem, K_FOREVER);




    }
}