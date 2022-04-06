#include "search.h"

#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;

int16_t **sweep_search(bool state, int min_encoder_search, int16_t max_encoder_search, int increment){
    printk("Starting to search in Azimuth\n");
    static int16_t readings[][3] = {0};
    int16_t index = 0;
    for (int i = 0; i < 10; i+= 1){
        //move_servo(state, i) bevege servo enten i asimuth til enkoder verdi i (ikke implementert enda)
        int16_t *buffer_data;
        printk("started\n");
        set_observer(true);
        printk("waiting\n");
        k_sem_take(&my_sem, K_FOREVER);
        printk("next\n");
        buffer_data = get_data();
        for (int i = 0; i < 3; i++){
            readings[index][i] = buffer_data[i];
        }
        int16_t encoder = readings[index][0];
        int16_t delta = readings[index][1];
        int16_t zigma = readings[index][2];
        printk("Encoder: %d, Delta: %d, Zigma %d, i: %d: \n",encoder, delta, zigma, index);
        index+=1;
    }

    k_sem_give(&my_sem);
    printk("Azimuth search done\n");
    return readings;
}