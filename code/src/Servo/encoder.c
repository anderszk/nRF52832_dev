#include "encoder.h"



#define servo_azimuth_N 0
#define servo_azimuth_pin 2
#define servo_elevationl_N 1
#define servo_elevation_pin 3
#define servo_antenna_N 2
#define servo_antenna_pin 4

#define pin_a_azimuth 26
#define pin_b_azimuth 27
#define pin_a_elevation 30
#define pin_b_elevation 31

#define starting_angle_azimuth 0
#define starting_angle_elevation 0

int16_t azimuth_encoder_value = 0;
int16_t elevation_encoder_value = 0;
int16_t azimuth_encoder_degrees;
int16_t elevation_encoder_degrees;
extern struct k_sem my_sem;

static void qdec_nrfx_event_handler_azimuth(nrfx_qdec_event_t event){}
static void qdec_nrfx_event_handler_elevation(nrfx_qdec_event_t event){}


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
        nrfx_qdec_enable();
        return err;

	}
    else if (err == NRFX_ERROR_INVALID_STATE) {
        printk("Azimuth QDEC was already initialized.\n");
        return err;
    }
	else {printk("Azimuth QDEC initialization failed.\n");}

    return err;
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
        nrfx_qdec_enable();
        return err;

	}
    else if (err == NRFX_ERROR_INVALID_STATE) {
        printk("Elevation QDEC was already initialized.\n");
        return err;
    }
	else {printk("Elevation QDEC initialization failed.\n");}

    return err;
}

int init_encoder_servos(){
	int err;
    err = servo_init(servo_azimuth_N, servo_azimuth_pin);
    if (err){
		printk("Could not Azimuth servomotor (err:%d).\n", err);
		return err;
	}
    err = servo_init(servo_elevationl_N,servo_elevation_pin);
    if (err){
		printk("Could not Elevation servomotor (err:%d).\n", err);
		return err;
	}
    err = servo_init(servo_antenna_N, servo_antenna_pin);
    if (err){
		printk("Could not configure Antenna servomotor (err:%d).\n", err);
		return err;
	}
    angle_move_servo(servo_azimuth_N, starting_angle_azimuth);
    angle_move_servo(servo_elevationl_N,starting_angle_elevation);
    angle_move_servo(servo_antenna_N, 90);
    
    
    k_msleep(1000);
    azimuth_encoder_value = starting_angle_azimuth * 23;
    elevation_encoder_value = starting_angle_elevation * 23;

    IRQ_CONNECT(QDEC_IRQn, 4, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);

}


void update_encoder(int N){
    int16_t acc;
    int16_t accdbl;
    nrfx_qdec_accumulators_read(&acc, &accdbl);
    if (N == 0){
        azimuth_encoder_value -= acc;
        azimuth_encoder_degrees = azimuth_encoder_value/23;
        // printk("Azimuth enc: %d.\n",  (azimuth_encoder_degrees));
    }
    else if (N ==1){
        elevation_encoder_value += acc;
        elevation_encoder_degrees = elevation_encoder_value/23;
        // printk("Elevation enc: %d.\n", elevation_encoder_degrees);
    }
    else{
        printk("Error, wrong encoder number.\n");
        return;
    }
}


void angle_slow_move(int N, uint32_t angle){
    int size = 0;
    
    if(N == 0){
        angle += 45;
        if (angle >= 225){angle = 225;} 
        else if(angle <= 45){angle = 45;}
        size = angle-get_servo_angle(N);

    }
    else if(N == 1){
        angle += 130;
        if (angle >= 200){angle = 200;} 
        else if(angle <= 130){angle = 130;}
        size = angle - get_servo_angle(N);

    }
    if(size > 0){
        // printk("going up, size: %d\n",size);
        for(int i = 0; i < size; i++){
            increment_servo(N);
            k_msleep(60);
            update_encoder(N);
            }
        }
    else if(size < 0){
        // printk("going down, size: %d\n", size);
        for(int i = 0; i > size; i--){
            decrement_servo(N);
            k_msleep(60);
            update_encoder(N);
            }
    }
}
int16_t get_encoder(int N){
    if(N < 1){
        return azimuth_encoder_degrees;
    }
    else{
        return elevation_encoder_degrees;
    }
}