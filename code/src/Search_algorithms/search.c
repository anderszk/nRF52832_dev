#include "search.h"


#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;
matrix_3x3 readings[270];

#define MY_STACK_SIZE 500
#define MY_PRIORITY 7


uint32_t *this_value;
int *N = 0;

extern void myThread(uint32_t *this_value, int *N){
    while(1){
        angle_move_servo(*N,*this_value);       
    }
    printk("Threading done");
}

K_THREAD_DEFINE(my_tid, MY_STACK_SIZE,
                myThread, &this_value, &N, NULL,
                MY_PRIORITY, 0, K_TICKS_FOREVER);

void sweep_search(int state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    N = state;
    printk("state: %d", N);
    k_thread_start(my_tid);
    printk("Starting to search in Azimuth\n");
    int16_t rows = (max_encoder_search-min_encoder_search)/increment;
    printk("rows: %d\n", rows);
    int16_t index = 0;
    matrix_3x3 buffer_data;
    k_thread_resume(my_tid);
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        this_value = i;
        set_observer(true);
        k_sem_take(&my_sem, K_FOREVER);
        get_data(&buffer_data, state);
        readings[index].encoder = buffer_data.encoder;
        readings[index].delta = buffer_data.delta;
        readings[index].zigma = buffer_data.zigma;
        printk("Encoder: %d, Delta: %d, Zigma %d, i: %d: \n",readings[index].encoder, readings[index].delta, readings[index].zigma, index);
        index+=1;
      
    }
    k_thread_suspend(my_tid);
    k_sem_give(&my_sem);
    printk("Azimuth search done\n");
}

void fine_sweeper(int state, int threshold_degrees, int threshold_search, int sweep_sector, matrix_3x3 zero_point){
    matrix_3x3 temp_data[sweep_sector + 1];
    int increment = 1;
    int16_t zero_point_index;
    int16_t min_encoder_value = zero_point.encoder - (sweep_sector/2);
    int16_t max_encoder_value = min_encoder_value + sweep_sector;
    printk("min enc: %d, max end: %d, zero: %d\n", min_encoder_value, max_encoder_value, zero_point.encoder);
    int16_t size = max_encoder_value - min_encoder_value;
    bool lower_cap = true;
    bool higher_cap = true;
    set_average_counter(10);
    set_encoder(min_encoder_value, state);//fake encoder verdi til min encoder value
    sweep_search(state, min_encoder_value, max_encoder_value, increment);
    get_readings(&temp_data, &size);
    zero_point_index = find_zero_point(temp_data, size);
    // this_value =  temp_data[zero_point_index].encoder;
    printk("Index: %d, Zero at: %d, delta: %d\n", zero_point_index, temp_data[zero_point_index].encoder, temp_data[zero_point_index].delta); 

    for (int i  = 0; i < sweep_sector/2; i++){
        if(lower_cap && temp_data[zero_point_index-i].delta <= temp_data[zero_point_index].delta+threshold_degrees && (zero_point_index-i) > 0){
            min_encoder_value = temp_data[zero_point_index-i].encoder;
            printk("Lower cap down one\n");
        }
        else{lower_cap = false;}
        if(higher_cap && temp_data[zero_point_index+i].delta <= temp_data[zero_point_index].delta+threshold_degrees && (zero_point_index + i) < size){
            max_encoder_value = temp_data[zero_point_index+i].encoder;
            printk("Higher cap up one\n");
        }
        else{higher_cap = false;}
    }
    sweep_sector = max_encoder_value - min_encoder_value;
    printk("lower enc: %d, higher enc: %d\n", min_encoder_value, max_encoder_value);
    printk("New sweep sector: %d degrees\n", sweep_sector);
    if(sweep_sector > threshold_search){
        fine_sweeper(state, threshold_degrees/2, threshold_search, sweep_sector, temp_data[zero_point_index]);
    }
    else{
        printk("Im fucking done\n");
        angle_move_servo(state,temp_data[zero_point_index].encoder);
        k_thread_suspend(my_tid);
        return;
        }



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

