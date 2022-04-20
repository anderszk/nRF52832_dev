#include "search.h"


#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;
matrix_3x3 readings[270];

#define MY_STACK_SIZE 500
#define MY_PRIORITY 1

uint64_t this_time;
uint64_t last_time;

int16_t min_thread = 45;
int16_t max_thread = 225;

// extern void myThread(){
//     int i = min_thread;
//     while(1){
//         this_time = k_uptime_get();
//         if((this_time - last_time) > 756){
//             angle_move_servo(0,i);
//             i += 1;
//             last_time = this_time;            
//         }
//         if (i >= max_thread){
//             return;
//         }        
//     }
// }

// K_THREAD_DEFINE(my_tid, MY_STACK_SIZE,
//                 myThread, NULL, NULL, NULL,
//                 MY_PRIORITY, 0, K_TICKS_FOREVER);

void sweep_search(bool state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    min_thread = min_encoder_search;
    max_thread = max_encoder_search;
    last_time = k_uptime_get();
    printk("%lld\n", last_time);
    // k_thread_start(my_tid);
    printk("Starting to search in Azimuth\n");
    int16_t rows = (max_encoder_search-min_encoder_search)/increment;
    printk("rows: %d\n", rows);
    int16_t index = 0;
    matrix_3x3 buffer_data;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        angle_move_servo(0,i);
        set_observer(true);
        k_sem_take(&my_sem, K_FOREVER);
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

void fine_sweeper(bool state, int threshold_degrees, int threshold_search, int sweep_sector, matrix_3x3 zero_point){
    matrix_3x3 temp_data[sweep_sector + 1];
    int increment = 1;
    int16_t zero_point_index;
    int16_t min_encoder_value = zero_point.encoder - (sweep_sector/2);
    int16_t max_encoder_value = zero_point.encoder + (sweep_sector/2);
    int16_t size = sweep_sector + 1;
    sweep_search(state, min_encoder_value, max_encoder_value, increment);
    get_readings(&temp_data, &size);
    zero_point_index = find_zero_point(temp_data, size); 
    // for (int i  = 0; i < )



}


int get_readings(matrix_3x3 *main_readings, int16_t *n){
    printk("%d\n", *n);
    // set_fake_values(&readings);
    value_validater(&readings, n);
    update_matrix(&readings, n);
    for(int i = 0; i < *n; i++){
            main_readings[i].encoder = readings[i].encoder;
            main_readings[i].delta = readings[i].delta;
            main_readings[i].zigma = readings[i].zigma;
    }
    printk("%d\n", *n);

    reset_readings();
    return 0;
}

void reset_readings(){
    for(int i = 0; i < 270; i++){
        readings[i].delta = 0;
        readings[i].encoder = 0;
        readings[i].zigma = 0;
    }
}

