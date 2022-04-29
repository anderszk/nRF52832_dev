#include "data_processor.h"

#define MIN_VALID_RSSI -90
#define MAX_VALID_RSSI -10

int16_t average_counter = 1;
int16_t data_delta[10];
int16_t data_zigma[10];

void send_data(int16_t rssi, int index, int state){
    if(state == 1){data_zigma[index] = rssi;}
    else{data_delta[index] = rssi;}
}

int16_t get_average(int16_t *list){
    int16_t average = 0;
    for(int i = 0; i < average_counter; i++){
        average += list[i];
    }
    return  average/average_counter;
}

void set_average_counter(int16_t value){
    if(value > 10){value = 10;}
    else if(value < 1){value = 1;}
    average_counter = value;
}


void get_data(matrix_x3 *buffer_data, int N){

    buffer_data->encoder =  get_encoder(N);
    buffer_data->delta =  get_average(data_delta);
    buffer_data->zigma =  get_average(data_zigma);

}

void value_validater(matrix_x3 *raw_data, int16_t *n){

    for(int i = 0; i < *n; i++){
        if(raw_data[i].delta > MAX_VALID_RSSI || raw_data[i].delta  < MIN_VALID_RSSI 
        || raw_data[i].zigma > MAX_VALID_RSSI || raw_data[i].zigma  < MIN_VALID_RSSI){
            raw_data[i].encoder = 0;
            raw_data[i].delta = 0;
            raw_data[i].zigma = 0;
            printk("Changed values at: %d", raw_data[i].encoder);
        }
    }
}

void update_matrix(matrix_x3 *data, int16_t *n){

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

bool zero_point_validater(int16_t value_zigma, int16_t value_delta, int16_t ZIGMA_ZERO_VALUE){

    return value_zigma >= ZIGMA_ZERO_VALUE && value_delta < value_zigma;

}

int find_zero_point(matrix_x3 validated_values[], int n){
    int zero_point_index = 0;
    bool first_zero = false;
    int16_t ZIGMA_ZERO_VALUE;

    if(n == 1){return 0;}

    ZIGMA_ZERO_VALUE = find_zigma_zero_value(validated_values, n);

    for(int i = 0; i < n-1; i++){
        if(!first_zero && zero_point_validater(validated_values[i].zigma,validated_values[i].delta, ZIGMA_ZERO_VALUE)){
            zero_point_index = i;
            first_zero = true;
            printk("First zero at index %d\n", i);
        }
    }

    for(int i = (zero_point_index + 1); i < n-1; i++){
        // if(validated_values[i].delta <= validated_values[i-1].delta && validated_values[i].delta <= validated_values[i+1].delta){
            if(validated_values[i].delta <= validated_values[zero_point_index].delta && zero_point_validater(validated_values[i].zigma, validated_values[i].delta, ZIGMA_ZERO_VALUE)){
                // printk("Old index: %d, value %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta);
                zero_point_index = i;
                // printk("New index: %d, value %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta);
            // }
        }
    }
    printk("encoder: %d, delta %d, zigma: %d\n", validated_values[zero_point_index].encoder, validated_values[zero_point_index].delta, validated_values[zero_point_index].zigma);
    return zero_point_index;

}

int16_t find_zigma_zero_value(matrix_x3 values[], int n){
    int16_t ZIGMA_ZERO_VALUE = -90;
    for (int i = 0; i < n-1; i++){
        if(values[i].zigma > ZIGMA_ZERO_VALUE){ZIGMA_ZERO_VALUE = values[i].zigma;}
    }
    printk("New zigma zero value validator: %d.\n",ZIGMA_ZERO_VALUE);
    return ZIGMA_ZERO_VALUE - 3;
}







