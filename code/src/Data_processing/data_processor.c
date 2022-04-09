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

void get_data(matrix_3x3 *buffer_data){

    buffer_data->encoder =  get_encoder();
    buffer_data->delta =  get_average(data_delta);
    buffer_data->zigma =  get_average(data_zigma);
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


void value_validater(matrix_3x3 *raw_data, int n){

    int16_t max_value = -10;
    int16_t min_value = -90;

    for(int i = 0; i < n; i++){
        if(raw_data[i].delta > max_value || raw_data[i].delta  < min_value){
            raw_data[i].encoder = 0;
            raw_data[i].delta = 0;
            raw_data[i].zigma = 0;
        }
        else if(raw_data[i].zigma > max_value || raw_data[i].zigma  < min_value){
            raw_data[i].encoder = 0;
            raw_data[i].delta = 0;
            raw_data[i].zigma = 0;
        }
    }
}

void update_matrix(matrix_3x3 *data, int16_t *n){
    printk("size:%d", *n);
    for(int i = 0; i < *n; i++){
        if(data[i].delta == 0 || data[i].zigma == 0){
            for(int pos = i; pos < *n-1; pos++){
                data[pos].encoder = data[pos+1].encoder;
                data[pos].delta = data[pos+1].delta;
                data[pos].zigma = data[pos+1].zigma;
            }
            *n -= 1;
        }   
    }
}

bool zero_point_validater(int16_t value_zigma){
    int16_t treshold_zigma = -30;
    if(value_zigma > treshold_zigma){
        return true;
    }
    else return false;
}

int find_zero_point(matrix_3x3 validated_values[], int n){
    int zero_point_index;

    if(n == 1){
        return 0;
    }
    if(validated_values[0].delta <= validated_values[1].delta && zero_point_validater(validated_values[0].zigma)){
        zero_point_index = 0;
    }
    if(validated_values[n-1].delta <= validated_values[n-2].zigma && zero_point_validater(validated_values[n-1].zigma)){
        zero_point_index = n-1;
    }

    for(int i = 1; i < n-1; i++){
        if(validated_values[i].delta <= validated_values[i-1].delta && validated_values[i].delta <= validated_values[i+1].delta){
            if(validated_values[i].delta < validated_values[zero_point_index].delta && zero_point_validater(validated_values[i].zigma)){
                zero_point_index = i;
            }
        }
    }
    return zero_point_index;

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

