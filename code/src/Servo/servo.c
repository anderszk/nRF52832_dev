#include "servo.h"
#define LOG_MODULE_NAME SERVO

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

//Set servo period in Hz
#define SERVO_PERIOD 50

//Set timer settings
#define CLOCK_SPEED 16000000
#define TIMER_RELOAD 320000
// #define TIMER_RELOAD (CLOCK_SPEED/SERVO_PERIOD)

#define PWMN_GPIOTE_CH      {0, 1, 2, 3}
#define PWMN_PPI_CH_A       {0, 2, 3, 4}
#define PWMN_PPI_CH_B       {1, 1, 5, 5}
#define PWMN_TIMER_CC_NUM   {0, 1, 2, 3}

static uint32_t pwmN_gpiote_ch[]    = PWMN_GPIOTE_CH;
static uint32_t pwmN_ppi_ch_a[]     = PWMN_PPI_CH_A;
static uint32_t pwmN_ppi_ch_b[]     = PWMN_PPI_CH_B;
static uint32_t pwmN_timer_cc_num[] = PWMN_TIMER_CC_NUM;


// Timer CC register use to reset the timer.
#define TIMER_RELOAD_CC_NUM 5

int timer_init()
{
    NRF_TIMER3->BITMODE                 = TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER3->PRESCALER               = 0;
    NRF_TIMER3->SHORTS                  = TIMER_SHORTS_COMPARE0_CLEAR_Msk << TIMER_RELOAD_CC_NUM;
    NRF_TIMER3->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER3->CC[TIMER_RELOAD_CC_NUM] = TIMER_RELOAD;

    LOG_INF("Timer initialized.");
    return 0 ;
}

int timer_start()
{
    NRF_TIMER3->TASKS_START = 1;

    LOG_INF("Timer started.");
    return 0;
}

uint32_t azimuth_servo_angle;
uint32_t horizontal_servo_angle;

int servo_init(uint32_t N, int servo_pin)
{
    if(N>3) {
        // Invalid N.
        return 1;
    }
    
    NRF_GPIOTE->CONFIG[pwmN_gpiote_ch[N]] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
                                            servo_pin << GPIOTE_CONFIG_PSEL_Pos |
                                            GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;
    
    NRF_PPI->CH[pwmN_ppi_ch_a[N]].EEP = (uint32_t)&NRF_TIMER3->EVENTS_COMPARE[pwmN_timer_cc_num[N]];
    NRF_PPI->CH[pwmN_ppi_ch_a[N]].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[pwmN_gpiote_ch[N]];
    if((N%2) == 0) {
        NRF_PPI->CH[pwmN_ppi_ch_b[N]].EEP = (uint32_t)&NRF_TIMER3->EVENTS_COMPARE[TIMER_RELOAD_CC_NUM];
        NRF_PPI->CH[pwmN_ppi_ch_b[N]].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[pwmN_gpiote_ch[N]];
    } else {
        NRF_PPI->FORK[pwmN_ppi_ch_b[N-1]].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[pwmN_gpiote_ch[N]];
    }
    NRF_PPI->CHENSET                      = (1 << pwmN_ppi_ch_a[N]) | (1 << pwmN_ppi_ch_b[N]);
    
    LOG_INF("Initializing servo %u on pin %i success!\n", N, servo_pin);
    return 0;
}

uint32_t convert_to_raw(uint32_t value)
{
    uint32_t angle = 0;
    angle = (value * 32000)/270 + 8000;

    return angle;
}


void raw_move_servo(int N, uint32_t position)
{
    if (N > 3)
    {
        LOG_INF("Invalid N, %u > 3", N);
        return;
    }
    if (position <= 0)
    {
        position = 1;
    } else if (position >= TIMER_RELOAD)
    {
        position = TIMER_RELOAD - 1;
    }
    NRF_TIMER3->CC[pwmN_timer_cc_num[N]] = position;
}

void angle_move_servo(int N, uint32_t angle)
{   
    if(N == 0){
    angle += 45;
    azimuth_servo_angle = angle;
    }
    else if(N == 1){
        horizontal_servo_angle = angle;
    }

    if (angle >= 270){angle = 270;} 
    else if(angle <= 0){angle = 0;}

    if(N == 0){azimuth_servo_angle = angle;}
    else if(N == 1){horizontal_servo_angle = angle;}

    angle = convert_to_raw(angle);
    raw_move_servo(N, angle);
}

void increment_servo(int N){
    if(N == 0){
        azimuth_servo_angle += 1;
        uint32_t raw_angle = convert_to_raw(azimuth_servo_angle);
        raw_move_servo(N,raw_angle);
    }
    else if(N == 1){
        horizontal_servo_angle +=1;
        uint32_t raw_angle = convert_to_raw(horizontal_servo_angle);
        raw_move_servo(N, raw_angle);
    }
}

void decrement_servo(int N){
    if(N == 0){
        azimuth_servo_angle -= 1;
        uint32_t raw_angle = convert_to_raw(azimuth_servo_angle);
        raw_move_servo(N,raw_angle);
    }
    else if(N == 1){
        horizontal_servo_angle -= 1;
        uint32_t raw_angle = convert_to_raw(horizontal_servo_angle);
        raw_move_servo(N, raw_angle);
    }
}

void get_servo_angle(int N){
    if(N == 0){printk("Azimuth servomotor angle: %d\n", azimuth_servo_angle);}
    else if(N == 1){printk("Horizontal servomotor angle: %d\n", horizontal_servo_angle);}
}

