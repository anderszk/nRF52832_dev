#include "search.h"

#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;


matrix_3x3 readings[270];

void sweep_search(bool state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    printk("Starting to search in Azimuth\n");    
    int16_t rows = (max_encoder_search-min_encoder_search)/increment;
    printk("rows: %d\n", rows);
    int16_t index = 0;
    matrix_3x3 buffer_data;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        angle_move_servo(0,i);
        printk("started\n");
        set_observer(true);
        printk("waiting\n");
        k_sem_take(&my_sem, K_FOREVER);
        printk("next\n");
        get_data(&buffer_data);
        readings[index].encoder = buffer_data.encoder;
        readings[index].delta = buffer_data.delta;
        readings[index].zigma = buffer_data.zigma;
        printk("Encoder: %d, Delta: %d, Zigma %d, i: %d: \n",readings[index].encoder, readings[index].delta, readings[index].zigma, index);
        index+=1;
      
    }
    k_sem_give(&my_sem);
    printk("Azimuth search done\n");
}


int getReadings(matrix_3x3 *main_readings, int n){
    for(int i = 0; i < n; i++){
            main_readings[i].encoder = readings[i].encoder;
            main_readings[i].delta = readings[i].delta;
            main_readings[i].zigma = readings[i].zigma;
    }
    return 0;
}

