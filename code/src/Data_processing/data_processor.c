#include "data_processor.h"

#define LOG_MODULE_NAME DATA_PROCESSOR
#define average_counter 1
LOG_MODULE_REGISTER(LOG_MODULE_NAME);


int16_t data_delta[average_counter];
int16_t data_zigma[average_counter];

void send_data_delta(int16_t rssi, int index){
    data_delta[index] = rssi;
}
void send_data_zigma(int16_t rssi, int index){
    data_zigma[index] = rssi;
}

void get_data(int16_t *buffer_data){

    buffer_data[0] =  get_encoder();
    buffer_data[1] =  get_average(data_delta);
    buffer_data[2] =  get_average(data_zigma);
    // for(int i = 0; i<3; i++){
    //     printk("value processor %d : %d\n", i, buffer_data[i]);
    // }

}

int16_t get_average(int16_t *list){
    int16_t average = 0;
    int16_t size = average_counter;
    for(int i = 0; i < size; i++){
        average += list[i];
    }
    average = average/size;
    return (int16_t) average;
}







// int find_local_minima(int data_array[][3]){

//     int16_t nulls_encoder_values[270];
//     int nulls_minimum_indexes[270];
//     int nulls_maximum_indexes[270];
//     int append_index_delta = 0;
//     int append_index_zigma = 0;

//     if(data_array[0][1] < data_array[1][1]){
//         nulls_maximum_indexes[append_index_delta] = 0;
//         append_index_delta += 1;
//     }
//     else if(data_array[0][2] > data_array[1][2]){
//         nulls_maximum_indexes[append_index_zigma] = 0;
//         append_index_zigma += 1;
//     }
//     for(int i = 1; i < sizeof(data_array); i++){
//         if(data_array[i-1][1] > data_array[i][1] && data_array[i][1] < data_array[i+1][1]){
//             nulls_minimum_indexes[append_index_delta] = i;
//             append_index_delta += 1;
//         }
//         else if(data_array[i-1][2] < data_array[i][2] && data_array[i][2] > data_array[i+1][2]){
//             nulls_maximum_indexes[append_index_zigma] = i;
//             append_index_zigma += 1;

//         }
//     }



//     return 0; //returnere minimum punktene som må videre undersøkes
// }

