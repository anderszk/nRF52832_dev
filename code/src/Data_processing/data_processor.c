#include "data_processor.h"

#define LOG_MODULE_NAME DATA_PROCESSOR
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

int8_t data_delta[5];
int8_t data_zigma[5];

void send_data_delta(int8_t rssi, int index){
    data_delta[index] = rssi;
}
void send_data_zigma(int8_t rssi, int index){
    data_zigma[index] = rssi;
}



void test_me_2(){
    LOG_INF("aaaaaahehhehehe");
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

