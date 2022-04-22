#include "search.h"


#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
#define MY_STACK_SIZE 500
#define MY_PRIORITY 7



extern struct k_sem my_sem;
matrix_3x3 readings[270];
matrix_3x3 azimuth_readings[180];
matrix_3x3 horizontal_readings[180];


uint32_t *this_value;
uint32_t *that_value;
int *N = 0;

extern void myThread_0(uint32_t *this_value){
    while(1){
        angle_move_servo(0,*this_value);       
    }
}

K_THREAD_DEFINE(my_tid_0, MY_STACK_SIZE,
                myThread_0, &this_value, NULL, NULL,
                MY_PRIORITY, 0, K_TICKS_FOREVER);
                
extern void myThread_1(uint32_t *that_value){
    while(1){
        angle_move_servo(1,*that_value);       
    }
}

K_THREAD_DEFINE(my_tid_1, MY_STACK_SIZE,
                myThread_1, &that_value, NULL, NULL,
                MY_PRIORITY, 0, K_TICKS_FOREVER);


zeros fine_search(zeros enc_values){

    zeros fine_zeros;
    angle_move_servo(2,0);
    k_msleep(1000);
	fine_zeros.azimuth_index = fine_sweeper(0,10,10,20,enc_values.azimuth_index);
    k_msleep(1000);
	angle_move_servo(2,90);
	k_msleep(1000);
	fine_zeros.horizontal_index = fine_sweeper(1, 10, 10, 20, enc_values.horizontal_index);
	k_msleep(1000);
	angle_move_servo(2,0);

    return fine_zeros;


}



zeros coarse_search(){
    zeros coarse_zeros;
    int zero_point_index_azimuth, zero_point_index_horizontal;
    int16_t min_encoder_search_azimuth = 0;
    int16_t max_encoder_search_azimuth = 180;
    int16_t min_encoder_search_horizontal = 0;
    int16_t max_encoder_search_horizontal = 90;
    int increment = 1;

	int16_t size = (max_encoder_search_azimuth-min_encoder_search_azimuth)/increment;
	sweep_search(0, min_encoder_search_azimuth, max_encoder_search_azimuth,increment);
	get_readings(&azimuth_readings, &size);
    for(int i = 0; i < size; i++){
		printk("Encoder: %d,  delta: %d, zigma: %d \n", azimuth_readings[i].encoder, azimuth_readings[i].delta, azimuth_readings[i].zigma);
	}
	zero_point_index_azimuth = find_zero_point(azimuth_readings, size);

    printk("index:%d\n",zero_point_index_azimuth);
	angle_slow_move(0, azimuth_readings[zero_point_index_azimuth].encoder);
	printk("zero value azimuth: %d\n",azimuth_readings[zero_point_index_azimuth].encoder);
    coarse_zeros.azimuth_index = azimuth_readings[zero_point_index_azimuth].encoder;
    
    k_msleep(1000);
    angle_move_servo(2, 90);
    k_msleep(1000);

	set_average_counter(3);
	size = (max_encoder_search_horizontal-min_encoder_search_horizontal)/increment;
	sweep_search(1,min_encoder_search_horizontal, max_encoder_search_horizontal, increment);
	get_readings(&horizontal_readings, &size);
    for(int i = 0; i < size; i++){
		printk("Encoder: %d,  delta: %d, zigma: %d \n", horizontal_readings[i].encoder, horizontal_readings[i].delta, horizontal_readings[i].zigma);
	}

    zero_point_index_horizontal = find_zero_point(horizontal_readings, size);
    printk("index:%d\n",zero_point_index_horizontal);
	angle_slow_move(1, horizontal_readings[zero_point_index_horizontal].encoder);
	printk("zero value azimuth: %d\n",horizontal_readings[zero_point_index_horizontal].encoder);
    coarse_zeros.horizontal_index = horizontal_readings[zero_point_index_horizontal].encoder;

    k_msleep(1000);
    angle_move_servo(2, 0);
    k_msleep(1000);

    return coarse_zeros;
}



void sweep_search(int state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    k_msleep(1000);
    printk("state: %d", state);
    if(state == 0){
        k_thread_start(my_tid_0);
        k_thread_resume(my_tid_0);
        }
    else{
        k_thread_start(my_tid_1);
        k_thread_resume(my_tid_1);
    }

    printk("Starting to search in Azimuth\n");
    int16_t rows = (max_encoder_search-min_encoder_search)/increment;
    printk("rows: %d\n", rows);
    int16_t index = 0;
    matrix_3x3 buffer_data;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        if(state){that_value = i;}
        else{this_value = i;}
        set_observer(true);
        k_sem_take(&my_sem, K_FOREVER);
        get_data(&buffer_data, state);
        readings[index].encoder = buffer_data.encoder;
        readings[index].delta = buffer_data.delta;
        readings[index].zigma = buffer_data.zigma;
        printk("Encoder: %d, Delta: %d, Zigma %d, i: %d: \n",readings[index].encoder, readings[index].delta, readings[index].zigma, index);
        index+=1;
      
    }
    k_thread_suspend(my_tid_0);
    k_thread_suspend(my_tid_1);
    k_sem_give(&my_sem);
    printk("Azimuth search done\n");
}

int16_t fine_sweeper(int state, int threshold_degrees, int threshold_search, int sweep_sector, int zero_point){
    matrix_3x3 temp_data[sweep_sector + 1];
    int increment = 1;
    int16_t zero_point_index;
    int16_t min_encoder_value = zero_point - (sweep_sector/2);
    int16_t max_encoder_value = min_encoder_value + sweep_sector;
    printk("min enc: %d, max end: %d, zero: %d\n", min_encoder_value, max_encoder_value, zero_point);
    int16_t size = max_encoder_value - min_encoder_value;
    bool lower_cap = true;
    bool higher_cap = true;
    set_average_counter(10);
    set_encoder(min_encoder_value, state);//fake encoder verdi til min encoder value
    sweep_search(state, min_encoder_value, max_encoder_value, increment);
    get_readings(&temp_data, &size);
    zero_point_index = find_zero_point(temp_data, size);
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
        fine_sweeper(state, threshold_degrees/2, threshold_search, sweep_sector, temp_data[zero_point_index].encoder);
    }
    else{
        printk("Im fucking done\n");
        angle_slow_move(state,temp_data[zero_point_index].encoder);
        k_thread_suspend(my_tid_0);
        k_thread_suspend(my_tid_1);
        return temp_data[zero_point_index].encoder;
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

