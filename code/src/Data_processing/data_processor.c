#include "data_processor.h"

#define LOG_MODULE_NAME DATA_PROCESSOR
#define ZIGMA_ZERO_VALUE -60

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

int16_t average_counter = 1;

int16_t data_delta[10];
int16_t data_zigma[10];

void send_data_delta(int16_t rssi, int index){
    data_delta[index] = rssi;
}
void send_data_zigma(int16_t rssi, int index){
    data_zigma[index] = rssi;
}

void get_data(matrix_3x3 *buffer_data, int N){

    buffer_data->encoder =  get_encoder(N);
    buffer_data->delta =  get_average(data_delta);
    buffer_data->zigma =  get_average(data_zigma);

}

int16_t get_average(int16_t *list){
    int16_t average = 0;
    int16_t size = average_counter;
    for(int i = 0; i < size; i++){
        average += list[i];
    }
    average = average/size;
    return  average;
}


void value_validater(matrix_3x3 *raw_data, int16_t *n){

    int16_t max_value = -10;
    int16_t min_value = -90;
    printk("n: %d \n", *n);

    for(int i = 0; i < *n; i++){
        if(raw_data[i].delta > max_value || raw_data[i].delta  < min_value || raw_data[i].zigma > max_value || raw_data[i].zigma  < min_value){
            raw_data[i].encoder = 0;
            raw_data[i].delta = 0;
            raw_data[i].zigma = 0;
            printk("Changed values at: %d", raw_data[i].encoder);
        }
    }
}

void update_matrix(matrix_3x3 *data, int16_t *n){
    printk("n: %d\n", *n);
    for(int i = 0; i < *n; i++){
        if(data[i].delta == 0 || data[i].zigma == 0){
            printk("Changed values at: %d", data[i].encoder);
            for(int pos = i; pos < *n-1; pos++){
                data[pos].encoder = data[pos+1].encoder;
                data[pos].delta = data[pos+1].delta;
                data[pos].zigma = data[pos+1].zigma;
            }
            *n -= 1;
        }   
    }
}

bool zero_point_validater(int16_t value_zigma, int16_t value_delta){
    int16_t treshold_zigma = ZIGMA_ZERO_VALUE;
    if(value_zigma >= treshold_zigma && value_delta < value_zigma){
        return true;
    }
    else{ return false;}
}

int find_zero_point(matrix_3x3 validated_values[], int n){
    printk("size: %d\n", n);
    int zero_point_index = 1;

    // if(n == 1){
    //     return 0;
    // }
    // if(validated_values[0].delta <= validated_values[1].delta && zero_point_validater(validated_values[0].zigma, validated_values[0].delta)){
    //     zero_point_index = 0;
    // }
    // if(validated_values[n-1].delta <= validated_values[n-2].zigma && zero_point_validater(validated_values[n-1].zigma, validated_values[n-1].delta)){
    //     zero_point_index = n-1;
    // }

    for(int i = 1; i < n-1; i++){
        // if(validated_values[i].delta <= validated_values[i-1].delta && validated_values[i].delta <= validated_values[i+1].delta){
            if(validated_values[i].delta <= validated_values[zero_point_index].delta && zero_point_validater(validated_values[i].zigma, validated_values[i].delta)){
                printk("Old index: %d, value %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta);
                zero_point_index = i;
                printk("New index: %d, value %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta);
            // }
        }
    }
    printk("encoder: %d, delta %d, zigma: %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta, validated_values[zero_point_index].zigma);
    return zero_point_index;

}



void set_fake_values(matrix_3x3 *matrix){
	matrix[22].delta = -77;
	matrix[9].zigma = 33;
    matrix[0].delta = -99;
    matrix[11].delta = -89;
    matrix[11].zigma = -60;
    matrix[2].zigma = 21331;
    matrix[17].delta = -70;

	// azimuth_reading[269].delta = 0;
	    
}

void set_average_counter(int16_t value){
    average_counter = value;
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

