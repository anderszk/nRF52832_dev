#include <zephyr.h>
#include <sys/printk.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <nrfx_qdec.h>

const uint32_t pin_a = 3;
const uint32_t pin_b = 28;
const uint32_t pin_led = 0xFFFFFFFF;


// nrfx_qdec_config_t qdec_config = NRFX_QDEC_DEFAULT_CONFIG(pin_a, pin_b, pin_led);



volatile int16_t acc_data = 0;

static void qdec_nrfx_event_handler(nrfx_qdec_event_t event)
{
    printk("Enetered qdec_nrfx_event_handler \n");
	switch (event.type) {
    case NRF_QDEC_EVENT_SAMPLERDY:
        
       
        printk("SAMPLERDY interrupt \n");
        break;

	case NRF_QDEC_EVENT_REPORTRDY:
        printk("REPORTRDY event handled");
        break;
    default:
      
        printk("unhandled event (0x%x) \n", event.type);
        break;
    }
}

void main(void)

{
    nrfx_qdec_config_t qdec_config = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_200,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_128us,            
        .psela              = pin_a,                                
        .pselb              = pin_b,                                
        .pselled            = pin_led,                              
        .ledpre             = 500,                                   
        .ledpol             = NRF_QDEC_LEPOL_ACTIVE_HIGH,            
        .dbfen              = NRF_QDEC_DBFEN_DISABLE,                
        .sample_inten       = false,                                 
        .interrupt_priority = NRFX_QDEC_DEFAULT_CONFIG_IRQ_PRIORITY  
};
    
    nrfx_err_t err = nrfx_qdec_init(&qdec_config, qdec_nrfx_event_handler);
    if (err != (NRFX_ERROR_BASE_NUM + 0)){
        printk("error\n");
    }
    nrfx_qdec_enable();

    printk("Running...\n");

    while(1)
	{
		__WFI();
	}
}