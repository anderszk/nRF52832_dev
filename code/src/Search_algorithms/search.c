#include "search.h"




#define MY_STACK_SIZE 500
#define MY_PRIORITY 7
#define MAX_READINGS 270
#define AZIMUTH_DEGREES 180
#define ELEVATION_DEGREES 90



extern struct k_sem my_sem;
matrix_x3 readings[MAX_READINGS];
matrix_x3 azimuth_readings[AZIMUTH_DEGREES];
matrix_x3 elevation_readings[ELEVATION_DEGREES];


uint32_t *azimuth_thread_servo_angle;
uint32_t *elevation_thread_servo_angle;



extern void azimuth_servo_thread(uint32_t *azimuth_thread_servo_angle){
    while(1){
        angle_slow_move(0,*azimuth_thread_servo_angle);
    }
}               

extern void elevation_servo_thread(uint32_t *elevation_thread_servo_angle){
    while(1){
        angle_slow_move(1,*elevation_thread_servo_angle);       
    }
}

K_THREAD_DEFINE(my_tid_1, MY_STACK_SIZE,
                elevation_servo_thread, &elevation_thread_servo_angle, NULL, NULL,
                MY_PRIORITY, 0, K_TICKS_FOREVER);

K_THREAD_DEFINE(my_tid_0, MY_STACK_SIZE,
                azimuth_servo_thread, &azimuth_thread_servo_angle, NULL, NULL,
                MY_PRIORITY, 0, K_TICKS_FOREVER);


void validate_servo_zero_moved(int N, uint32_t zero_point_servo_angle){
    uint32_t servo_angle;
    if(N == 0){servo_angle = get_servo_angle(N) - 45;}
    else if(N == 1){servo_angle = get_servo_angle(N) - 130;}

    if(zero_point_servo_angle  == servo_angle){
        printk("Robot moved to zero-point\n.");
        return;}
    else{
        k_msleep(1000);
        validate_servo_zero_moved(N, zero_point_servo_angle);}
}

zeros fine_search(zeros enc_values){
    zeros fine_zeros;
    if(FINE_ACTIVATE){
        if(SEARCH_AZIMUTH){
            angle_move_servo(2,90);
            k_msleep(2000);
            init_encoder_azimuth();
            printk("Starting fine search in Azimuth.\n");
            k_thread_resume(my_tid_0);
            fine_zeros.azimuth = fine_sweeper(0,10,10,20,enc_values.azimuth);
            printk("Moving robot to zero-point, encoder value: %d.\n", fine_zeros.azimuth);
            azimuth_thread_servo_angle = fine_zeros.azimuth;
            validate_servo_zero_moved(0, azimuth_thread_servo_angle);
            k_thread_suspend(my_tid_0);
        }
        if(SEARCH_ELEVATION){
            k_msleep(1000);
            angle_move_servo(2,0);
            k_msleep(2000);

            printk("Starting fine search in Elevation.\n");
            init_encoder_elevation();
            k_thread_resume(my_tid_1);
            fine_zeros.elevation = fine_sweeper(1, 10, 10, 20, enc_values.elevation);
            printk("Moving robot to zero-point, encoder value: %d.\n", fine_zeros.elevation);
            elevation_thread_servo_angle = fine_zeros.elevation;
            validate_servo_zero_moved(1, elevation_thread_servo_angle);
            k_thread_suspend(my_tid_1);

            k_msleep(1000);
            angle_move_servo(2,90);
            k_msleep(2000);
        }
        printk("Fine search finished.\n");
    return fine_zeros;
    }
    return enc_values;
}


zeros coarse_search(){

    zeros coarse_zeros;
    int zero_point_index_azimuth, zero_point_index_elevation;
    int16_t min_encoder_search_azimuth = 0;
    int16_t max_encoder_search_azimuth = 180;
    int16_t min_encoder_search_elevation = 0;
    int16_t max_encoder_search_elevation = 50;
    int increment = 1;
	int16_t size = (max_encoder_search_azimuth-min_encoder_search_azimuth)/increment;

    printk("Starting coarse sweep in Azimuth.\n");

    if(SEARCH_AZIMUTH){
        k_thread_start(my_tid_0);
        sweep_search(0, min_encoder_search_azimuth, max_encoder_search_azimuth,increment);
        get_readings(&azimuth_readings, &size);
        zero_point_index_azimuth = find_zero_point(azimuth_readings, size);
        coarse_zeros.azimuth = azimuth_readings[zero_point_index_azimuth].encoder;
        printk("Moving robot to zero point, encoder value: %d.\n", coarse_zeros.azimuth);
        azimuth_thread_servo_angle = coarse_zeros.azimuth;
        validate_servo_zero_moved(0, azimuth_thread_servo_angle);
        k_thread_suspend(my_tid_0);
        printk("Coarse sweep in Azimuth finished\n");
    }


    // for(int i = 0; i < size; i++){
	// 	printk("Encoder: %d,  delta: %d, zigma: %d \n", azimuth_readings[i].encoder, azimuth_readings[i].delta, azimuth_readings[i].zigma);
	// }

    if (SEARCH_ELEVATION){
        k_msleep(1000);
        angle_move_servo(2, 0);
        k_msleep(2000);

        init_encoder_elevation();
        set_average_counter(3);
        size = (max_encoder_search_elevation-min_encoder_search_elevation)/increment;

        printk("Starting coarse sweep in Elevation\n");
        k_thread_start(my_tid_1);
        sweep_search(1,min_encoder_search_elevation, max_encoder_search_elevation, increment);
        get_readings(&elevation_readings, &size);
        zero_point_index_elevation = find_zero_point(elevation_readings, size);
        coarse_zeros.elevation = elevation_readings[zero_point_index_elevation].encoder;
        printk("Moving robot to zero point, encoder value: %d.\n", coarse_zeros.elevation);
        elevation_thread_servo_angle = coarse_zeros.elevation;
        validate_servo_zero_moved(1, elevation_thread_servo_angle);
        k_thread_suspend(my_tid_1);

        printk("Coarse sweep in Elevation done\n");


        for(int i = 0; i < size; i++){
            printk("%d,%d,%d \n", elevation_readings[i].encoder, elevation_readings[i].zigma, elevation_readings[i].delta);
        }

        k_msleep(1000);
        angle_move_servo(2, 90);
        k_msleep(2000);
    }
    printk("Coarse search finished.\n");

    return coarse_zeros;
}



void sweep_search(int state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    k_msleep(1000);

    int16_t index = 0;
    matrix_x3 buffer_data;    

    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        if(state){elevation_thread_servo_angle = (uint32_t) i;}
        else{azimuth_thread_servo_angle = (uint32_t) i;}
        set_observer(true);
        k_sem_take(&my_sem, K_FOREVER);
        
        get_data(&buffer_data, state);
        readings[index].encoder = buffer_data.encoder;
        readings[index].delta = buffer_data.delta;
        readings[index].zigma = buffer_data.zigma;

        printk("%d,%d,%d\n",readings[index].encoder, readings[index].zigma, readings[index].delta);
        index+=1;
    }
    k_sem_give(&my_sem);
}

int16_t fine_sweeper(int state, int threshold_degrees, int threshold_search, int sweep_sector, int16_t zero_point){


    matrix_x3 temp_data[sweep_sector + 1];
    int increment = 1;
    int16_t zero_point_index;
    int16_t min_encoder_value = zero_point - (sweep_sector/2);
    int16_t max_encoder_value = min_encoder_value + sweep_sector;
    int16_t size = max_encoder_value - min_encoder_value;
    bool lower_cap = true;
    bool higher_cap = true;
    
    k_msleep(1000);
    set_average_counter(10);
    angle_slow_move(state, min_encoder_value);
    sweep_search(state, min_encoder_value, max_encoder_value, increment);
    get_readings(&temp_data, &size);
    zero_point_index = find_zero_point(temp_data, size);

    for (int i  = 0; i < sweep_sector/2; i++){

        if(lower_cap && temp_data[zero_point_index-i].delta <= temp_data[zero_point_index].delta+threshold_degrees && (zero_point_index-i) > 0){
            min_encoder_value = temp_data[zero_point_index-i].encoder;
        }
        else{lower_cap = false;}

        if(higher_cap && temp_data[zero_point_index+i].delta <= temp_data[zero_point_index].delta+threshold_degrees && (zero_point_index + i) < size){
            max_encoder_value = temp_data[zero_point_index+i].encoder;
        }
        else{higher_cap = false;}
    }

    sweep_sector = max_encoder_value - min_encoder_value;

    if(sweep_sector > threshold_search){
        fine_sweeper(state, threshold_degrees/2, threshold_search, sweep_sector, temp_data[zero_point_index].encoder);
    }
    else{
        angle_slow_move(state,temp_data[zero_point_index].encoder);
        return temp_data[zero_point_index].encoder;
        }
}


int get_readings(matrix_x3 *main_readings, int16_t *n){

    value_validater(&readings, n);
    update_matrix(&readings, n);
    for(int i = 0; i < *n; i++){
            main_readings[i].encoder = readings[i].encoder;
            main_readings[i].delta = readings[i].delta;
            main_readings[i].zigma = readings[i].zigma;
    }
    reset_readings();
    return 0;
}

void reset_readings(){
    for(int i = 0; i < 270; i++){
        memset(&readings[i], 0, sizeof readings[i]);
    }
}



