#include <zephyr.h>
#include <sys/printk.h>
#include <logging/log.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <nrfx_qdec.h>


const uint32_t pin_a = 26;
const uint32_t pin_b = 27;
uint16_t inc = 0;

static volatile int16_t acc_data = 0;
static volatile uint16_t accdbl_data = 0;


static void qdec_nrfx_event_handler(nrfx_qdec_event_t event)
{
   if (event.type == NRF_QDEC_EVENT_REPORTRDY) {
       acc_data = event.data.report.acc;
       accdbl_data = event.data.report.accdbl;
    //    printk("Data1: %u\n", acc_data);
        if(acc_data <0){
            inc++;
        }
        else{
            inc--;
        }
        printk("Data1: %u\n", inc);
        //printk("Data2: %u\n", accdbl_data);
   }
   
}

void main(void)
{
    IRQ_CONNECT(QDEC_IRQn, 0, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);

    nrfx_qdec_config_t qdec_config = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_10,                 
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


    printk("Running...\n");


    while(1)
	{
		__WFI();
	}

}
