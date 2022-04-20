#include <zephyr.h>
#include <sys/printk.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <nrfx_qdec.h>
#include <nrfx_gpiote.h>

const uint32_t pin_a = 3;
const uint32_t pin_b = 4;
//const uint32_t pin_a = 32+1; //Denne ga same feilen som å bruke pin 3 og 4
//const uint32_t pin_b = 32+2;
const uint32_t pin_led = 0xFFFFFFFF;


// nrfx_qdec_config_t qdec_config = NRFX_QDEC_DEFAULT_CONFIG(pin_a, pin_b, pin_led);


volatile int16_t acc_data = 0;
volatile int16_t accdbl_data = 0;



static void qdec_nrfx_event_handler(nrfx_qdec_event_t event)
{
    //printk("Enetered qdec_nrfx_event_handler \n");
	switch (event.type) {
    case NRF_QDEC_EVENT_SAMPLERDY:
        printk("SAMPLERDY interrupt \n");
		//nrfx_qdec_accumulators_read(acc_data, accdbl_data);
		//printk(event.data.report.acc);
		//printk(event.data.report.accdbl);
        break;

	case NRF_QDEC_EVENT_REPORTRDY:
        printk("REPORTRDY event handled");
		acc_data = event.data.report.acc;
		//printk(acc_data);

		//acc_data = event.data.report.acc;
		//printk(acc_data);
		//printk(event.data.report.accdbl);
        break;

    default:
        //printk("unhandled event (0x%x) \n", event.type);
        break;
    }
}

void main(void)

{

	nrfx_err_t err;

    nrfx_qdec_config_t qdec_config = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_200,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_1024us,            
        .psela              = pin_a,                                
        .pselb              = pin_b,                                
        .pselled            = pin_led,                              
        .ledpre             = 500,                                   
        .ledpol             = NRF_QDEC_LEPOL_ACTIVE_HIGH,            
        .dbfen              = NRF_QDEC_DBFEN_DISABLE,                
        .sample_inten       = false,                                 
        .interrupt_priority = NRFX_QDEC_DEFAULT_CONFIG_IRQ_PRIORITY  
};

	
	IRQ_CONNECT(QDEC_IRQn, 0, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);
	
    
    err = nrfx_qdec_init(&qdec_config, qdec_nrfx_event_handler);
	if (err == NRFX_SUCCESS) {
		printk("OK");
	}
	else {printk("mordi");}

    nrfx_qdec_enable();

    printk("Running...\n");

    while(1)
	{
		k_msleep(1);
	}
}

