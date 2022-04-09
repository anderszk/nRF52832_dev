#include "search.h"

#define LOG_MODULE_NAME SEARCHER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem;

// int16_t **get(int N, int M){

//     int16_t i, **array;
//     array = k_malloc(N*sizeof(int16_t *));
//     for(i = 0; i < N; i++){
//         array[i] = k_malloc(M*sizeof(int16_t));
//     }
//     return array;
// }

// void freeArray(int16_t **p, int N){//denne trenges kanskje ikke
//     for (int i = 0; i < N; i++){
//         k_free(p[i]);
//     }
//     k_free(p);
// }

// int16_t *get_1_3(){
//     int16_t i, *array;
//     array = k_malloc(3*sizeof(int16_t));
//     return array;
// }
matrix_3x3 readings[270];

void sweep_search(bool state, int16_t min_encoder_search, int16_t max_encoder_search, int increment){
    printk("Starting to search in Azimuth\n");    
    // int16_t **readings;
    int16_t rows = (max_encoder_search-min_encoder_search)/increment;
    printk("rows: %d\n", rows);
    // readings = get(rows,3);
    // if(readings != NULL){ freeArray(readings, sizeof(readings)/sizeof(readings[0]));}//trenger kanskje ikke han her
    int16_t index = 0;
    matrix_3x3 buffer_data;
    for (int i = min_encoder_search; i < max_encoder_search; i+= increment){
        //move_servo(state, i) bevege servo enten i asimuth til enkoder verdi i (ikke implementert enda)
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
    // for (int i = 0; i<rows; i++){
	// 	printk("Encoder: %d, delta: %d, zigma: %d\n", readings[i].encoder, readings[i].delta, readings[i].zigma);
    // }

    k_sem_give(&my_sem);
    printk("Azimuth search done\n");
}


int getReadings(matrix_3x3 *main_readings, int n){
    for(int i = 0; i < n; i++){
        // for (int j = 0; j < 3; j++){
            main_readings[i].encoder = readings[i].encoder;
            main_readings[i].delta = readings[i].delta;
            main_readings[i].zigma = readings[i].zigma;
        // }
    }
    // for(int i = 0; i < n; i++){
	// 	printk("Encoder: %d, delta: %d, zigma: %d \n", main_readings[i][0], main_readings[i][1], main_readings[i][2]);
	// }
    // for(int i = 0; i < n; i++){
	// 	printk("Encoder: %d, delta: %d, zigma: %d \n", readings[i][0], readings[i][1], readings[i][2]);
	// }
    return 0;
}

