#include <zephyr.h>
#include <sys/printk.h>
#include <logging/log.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <nrfx_qdec.h>


const uint32_t pin_a = 26;
const uint32_t pin_b = 27;

// static volatile int16_t acc_data = 0;
// static volatile uint16_t accdbl_data = 0;

static volatile uint16_t m_accdblread;
static volatile int16_t m_accread;
static volatile int16_t m_value=0;
static volatile int16_t m_last_value=0;


static void qdec_nrfx_event_handler(nrfx_qdec_event_t event)
{
   if (event.type == NRF_QDEC_EVENT_REPORTRDY) {
    //    acc_data = event.data.report.acc;
    //    accdbl_data = event.data.report.accdbl;
    //     printk("Data1: %u\n", acc_data);
    //     //printk("Data2: %u\n", accdbl_data);

        m_accdblread = event.data.report.accdbl;
        m_accread = event.data.report.acc;
        if (m_accdblread==0) {
        	m_value += m_accread;
	    }
        if (m_value<0) {
		m_value = 0;
	    } else if (m_value>100) {
		m_value=100;
	    }
        if (m_value != m_last_value) {
            printk("ACC: %i\n", m_accread);
            printk("ACCDBL: %i\n", m_accdblread);

            // uart_printf("report dbl=%u acc=%d",m_accdblread,m_accread);
            //     if (m_accread>0) {
			// uart_printf("\x1B[1;32m"); // GREEN
            //     } else {
			// uart_printf("\x1B[1;31m"); // RED
		    // }
        	// uart_printf(" val=%d\n\r",m_value);
		    // m_last_value = m_value;
		    // uart_printf("\x1B[0m"); // DEFAULT color
	    }
        
   }
   
}

void main(void)

{
    IRQ_CONNECT(QDEC_IRQn, 0, nrfx_isr, nrfx_qdec_irq_handler, 0);
	irq_enable(QDEC_IRQn);

    nrfx_qdec_config_t qdec_config = 
    {                                                                
        .reportper          = NRF_QDEC_REPORTPER_10,                 
        .sampleper          = NRF_QDEC_SAMPLEPER_4096us,            
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
		k_msleep(1);
	}
}

