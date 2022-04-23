#include <zephyr.h>
#include <sys/printk.h>
#include <drivers/gpio.h>
#include <nrfx_qdec.h>

const uint32_t pin_a = 26;
const uint32_t pin_b = 27;

// static volatile int16_t acc_data = 0;
// static volatile uint16_t accdbl_data = 0;

int16_t tisss = 0;
int16_t bassj = 0;

int32_t test = 0;
NRF_QDEC_Type tiss;


static void qdec_nrfx_event_handler(nrfx_qdec_event_t event)
{

   if (event.type == NRF_QDEC_EVENT_REPORTRDY) {
       //acc_data = event.data.report.acc;
       //accdbl_data = event.data.report.accdbl;
       //int sample_value = event.data.sample.value;

        tisss = event.data.report.acc;
        bassj = event.data.report.accdbl;
        //test = nrf_qdec_acc_get(&tiss);
        nrfx_qdec_accumulators_read(&tisss, &bassj);
        printk("ACC: %d, ACCBL %d \n", tisss, bassj);
   }
   
}

void main(void)
{
    IRQ_CONNECT(QDEC_IRQn, 0, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);

    nrfx_qdec_config_t qdec_config = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_40,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_128us,            
        .psela              = pin_a,                                
        .pselb              = pin_b,                                
        .pselled            = NRF_QDEC_LED_NOT_CONNECTED,                                          
        .dbfen              = NRF_QDEC_DBFEN_DISABLE,                
        .sample_inten       = false,                                 
        .interrupt_priority = NRFX_QDEC_DEFAULT_CONFIG_IRQ_PRIORITY
    };


    nrfx_err_t err;
	
    err = nrfx_qdec_init(&qdec_config, qdec_nrfx_event_handler);
	if (err == NRFX_SUCCESS) {
		printk("QDEC initialization was successful.\n");
	}
    else if (err == NRFX_ERROR_INVALID_STATE) {
        printk("QDEC was already initialized.");
    }
	else {printk("QDEC initialization failed.\n");}


    nrfx_qdec_enable();
    //nrf_qdec_enable(&tiss);


    printk("Running...\n");

    while(1)
	{
		k_msleep(1);
        //__WFI();
	}

}
