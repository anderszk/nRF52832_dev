#include "encoder.h"

#define LOG_MODULE_NAME ENCODER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define servo_azimuth_N 0
#define servo_azimuth_pin 13
#define servo_horizontal_N 1
#define servo_horizontal_pin 15
#define servo_antenna_N 2
#define servo_antenna_pin 16

#define pin_a_azimuth 26
#define pin_b_azimuth 27
#define pin_a_elevation 3
#define pin_b_elevation 4



int16_t azimuth_encoder_value = 0;
int16_t elevation_encoder_value = 0;
int16_t azimuth_encoder_degrees;
int16_t elevation_encoder_degrees;
extern struct k_sem my_sem;

static void qdec_nrfx_event_handler_azimuth(nrfx_qdec_event_t event){}
static void qdec_nrfx_event_handler_elevation(nrfx_qdec_event_t event){}



void update_encoder(int N){
    int16_t acc;
    int16_t accdbl;
    nrfx_qdec_accumulators_read(&acc, &accdbl);
    if (N < 1){
        azimuth_encoder_value -= acc;
        azimuth_encoder_degrees = azimuth_encoder_value/23;
        printk("Azimuth enc: %d\n",  (azimuth_encoder_degrees));
    }
    else{
        elevation_encoder_value += acc;
        elevation_encoder_degrees = elevation_encoder_value/23;
        // printk("Elevation enc: %d\n", elevation_encoder_degrees);
        }
}

int16_t get_encoder(int N){//fake encoder verdier som gjenspeiler servo vinkelen så bra den kan
    if(N < 1){
        return azimuth_encoder_degrees;
    }
    else{
        return elevation_encoder_degrees;
    }
}

int init_encoder_azimuth(){

    int err;
    nrfx_qdec_disable();
    nrfx_qdec_uninit();
    

    nrfx_qdec_config_t qdec_config_azimuth = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_DISABLED,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_128us,            
        .psela              = pin_a_azimuth,                                
        .pselb              = pin_b_azimuth,                                
        .pselled            = NRF_QDEC_LED_NOT_CONNECTED,                                          
        .dbfen              = NRF_QDEC_DBFEN_DISABLE,                
        .sample_inten       = false,                                 
        .interrupt_priority = NRFX_QDEC_DEFAULT_CONFIG_IRQ_PRIORITY
    };
    err = nrfx_qdec_init(&qdec_config_azimuth, qdec_nrfx_event_handler_azimuth);
	if (err == NRFX_SUCCESS) {
		printk("Azimuth QDEC initialization was successful.\n");
	}
    else if (err == NRFX_ERROR_INVALID_STATE) {
        printk("Azimuth QDEC was already initialized.");
    }
	else {printk("Azimuth QDEC initialization failed.\n");}
    nrfx_qdec_enable();

    return err;
}

void set_encoder(int value, int N){
    if(N < 1){
        azimuth_encoder_value = value;}
    else{elevation_encoder_value = value;}
}


int init_encoder_elevation(){

    int err;
    nrfx_qdec_disable();
    nrfx_qdec_uninit();
    

    nrfx_qdec_config_t qdec_config_elevation = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_DISABLED,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_128us,            
        .psela              = pin_a_elevation,                                
        .pselb              = pin_b_elevation,                                
        .pselled            = NRF_QDEC_LED_NOT_CONNECTED,                                          
        .dbfen              = NRF_QDEC_DBFEN_DISABLE,                
        .sample_inten       = false,                                 
        .interrupt_priority = NRFX_QDEC_DEFAULT_CONFIG_IRQ_PRIORITY
    };


    err = nrfx_qdec_init(&qdec_config_elevation, qdec_nrfx_event_handler_elevation);
	if (err == NRFX_SUCCESS) {
		printk("Elevation QDEC initialization was successful.\n");
	}
    else if (err == NRFX_ERROR_INVALID_STATE) {
        printk("Elevation QDEC was already initialized.");
    }
	else {printk("Elevation QDEC initialization failed.\n");}
    nrfx_qdec_enable();

    return err;
}


int init_encoder_servos(){
	int err = servo_init(servo_azimuth_N, servo_azimuth_pin);
    err = servo_init(servo_horizontal_N,servo_horizontal_pin);
    err = servo_init(servo_antenna_N, servo_antenna_pin);
    angle_move_servo(servo_azimuth_N, 0);
    angle_move_servo(servo_horizontal_N, 40);
    angle_move_servo(servo_antenna_N, 0);
    k_msleep(1000);
    azimuth_encoder_value = 0;
    elevation_encoder_value = 0;

    IRQ_CONNECT(QDEC_IRQn, 4, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);

    return err;
}


void angle_slow_move(int N, uint32_t angle){
    if(N == 0){
        angle += 45;
        if (angle >= 270){angle = 270;} 
        else if(angle <= 0){angle = 0;}
        // printk("Old angle: %d, new angle: %d\n", get_servo_angle(N), angle);
        int size = angle-get_servo_angle(N);
        // printk("size: %d\n", size);
        if(size > 0){
            printk("going up\n");
            for(int i = 0; i < size; i++){
                increment_servo(N);
                k_msleep(90);
                update_encoder(N);
                }
            }
        else if(size < 0){
            printk("going down:\n");
            for(int i = 0; i > size; i--){
                decrement_servo(N);
                k_msleep(90);
                update_encoder(N);
                }
            }
        // int value = get_servo_angle(0) -45;
        // printk("servo azimuth: %d\n", value);
    }
    if(N == 1){
        angle += 110;
        if (angle >= 200){angle = 200;} 
        else if(angle <= 110){angle = 110;}
        // printk("Old angle: %d, new angle: %d\n", get_servo_angle(N), angle);
        int size = angle - get_servo_angle(N);
        // printk("size: %d\n", size);
        if(size > 0){
            printk("going up\n");
            for(int i = 0; i < size; i++){
                increment_servo(N);
                k_msleep(60);
                update_encoder(N);
                }
            }
        else if(size < 0){
            printk("going down:\n");
            for(int i = 0; i > size; i--){
                decrement_servo(N);
                k_msleep(60);
                update_encoder(N);
                }
        }
        // int value = get_servo_angle(1) -45;
        // printk("servo elevation: %d\n", value);
    }
}