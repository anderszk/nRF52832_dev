#include "search.h"

#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;

struct data_array{
    int a[1][3];
};

void test_me(){
    LOG_INF("hehhehehe");
}

int sweep_search(bool state, int min_encoder_search, int max_encoder_search, int increment){
    LOG_INF("Starting to search in Azimuth");
    int readings[270][3] = {0};
    int index = 0;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        //move_servo(state, i) bevege servo enten i asimuth til enkoder verdi i
        LOG_INF("start");
        LOG_INF("started");
        set_observer(true);
        LOG_INF("waiting");
        k_sem_take(&my_sem, K_FOREVER);
        LOG_INF("next");
        // struct data_array *buffer_data = get_data();
        // for (int i = 0; i < 3; i++){
        //     readings[index][i] = &buffer_data->a[0][i];
        // }
        // int encoder = &buffer_data->a[0][0];
        // int delta = &buffer_data->a[0][1];
        // int zigma = &buffer_data->a[0][2];
        // LOG_INF("Encoder: %d, Delta: %d, Zigma %d",encoder, delta, zigma);
    }

    return 0;
}