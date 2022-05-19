/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include "defines.h"


int freq = (float)1/(0.000058*2);
double pulses;


/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

volatile char but_flag1 = 0;
volatile char echo_flag = 0;
volatile char error_flag = 0;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/
void init(void);
void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

void call_callback(void){
	but_flag1 = 1;
}

void echo_callback(void){
	if (pio_get(ECHO_PIO, PIO_INPUT, ECHO_PIO_IDX_MASK)) {

		if (but_flag1){
			RTT_init(freq, 0 , 0);
			echo_flag = 1;
			} 
			
		else {
			error_flag = 1;
		}
	} 
		
	else {
		echo_flag = 0;
		pulses = rtt_read_timer_value(RTT);
	}
}

void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

	uint16_t pllPreScale = (int) (((float) 32768) / freqPrescale);
	
	rtt_sel_source(RTT, false);
	rtt_init(RTT, pllPreScale);
	
	if (rttIRQSource & RTT_MR_ALMIEN) {
		uint32_t ul_previous_time;
		ul_previous_time = rtt_read_timer_value(RTT);
		while (ul_previous_time == rtt_read_timer_value(RTT));
		rtt_write_alarm_time(RTT, IrqNPulses+ul_previous_time);
	}

	/* config NVIC */
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 4);
	NVIC_EnableIRQ(RTT_IRQn);

	/* Enable RTT interrupt */
	if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN)){
		rtt_enable_interrupt(RTT, rttIRQSource);
	}
	
	else{
		rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);
	}
	
	
}

void RTT_Handler(void) {
	uint32_t ul_status;

	/* Get RTT status - ACK */
	ul_status = rtt_get_status(RTT);

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		error_flag = 1;
	}
	
	/* IRQ due to Time has changed */
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC) {
	}
}

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void pin_toggle(Pio *pio, uint32_t mask) {
	if(pio_get_output_data_status(pio, mask)) {
		pio_clear(pio, mask);
	}
	
	else{
		pio_set(pio,mask);
	}
	
}

void blink_trig(){
	pio_clear(TRIG_PIO, TRIG_PIO_IDX_MASK);
	delay_us(10);
	pio_set(TRIG_PIO, TRIG_PIO_IDX_MASK);

}

float read_distance(){
	float distance = (float)(100.0*pulses*340)/(freq*2);
	if(distance >= 400){
		return -1;
	}
	return distance;
}

/************************************************************************/
/* init                                                              */
/************************************************************************/

// Função de inicialização do uC
void init(void){

	// Initialize the board clock
	sysclk_init();

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;


	// Leds OLED1
	pmc_enable_periph_clk(TRIG_PIO_ID);
	pio_set_output(TRIG_PIO, TRIG_PIO_IDX_MASK, 0, 0, 1);
	
	
	
	// BOTAO DO OLED
	pmc_enable_periph_clk(BUT_PIO1_ID);
	pio_configure(BUT_PIO1, PIO_INPUT, BUT_PIO_1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO1, BUT_PIO_1_IDX_MASK, 60);
	pio_handler_set(BUT_PIO1,BUT_PIO1_ID,BUT_PIO_1_IDX_MASK,PIO_IT_FALL_EDGE,call_callback);
	pio_enable_interrupt(BUT_PIO1, BUT_PIO_1_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO1);
	NVIC_EnableIRQ(BUT_PIO1_ID);
	NVIC_SetPriority(BUT_PIO1_ID, 4);
	
	// ECHO
	pmc_enable_periph_clk(ECHO_PIO_ID);
	pio_configure(ECHO_PIO, PIO_INPUT,ECHO_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_debounce_filter(ECHO_PIO, ECHO_PIO_IDX_MASK, 60);
	pio_handler_set(ECHO_PIO,ECHO_PIO_ID,ECHO_PIO_IDX_MASK,PIO_IT_EDGE,echo_callback);
	pio_enable_interrupt(ECHO_PIO, ECHO_PIO_IDX_MASK);
	pio_get_interrupt_status(ECHO_PIO);
	NVIC_EnableIRQ(ECHO_PIO_ID);
	NVIC_SetPriority(ECHO_PIO_ID, 5);
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/

int main (void)
{
	board_init();
	delay_init();

	// IO Init:
	init();

	// Init OLED
	gfx_mono_ssd1306_init();
	

	while(1) {
		if (error_flag){
			gfx_mono_draw_string("error!!!  ", 5, 10, &sysfont);
			error_flag = 0;
			} 
			
		else if(echo_flag){
			int distance = read_distance();
			
			if(distance == -1){
				gfx_mono_draw_string("too far!", 5, 10, &sysfont);
				error_flag = 0;
				} 
				
			else {
				char dist_str[10];
				sprintf(dist_str, "%d cm    ", distance);
				gfx_mono_draw_string(dist_str, 5, 10, &sysfont);
				}
			but_flag1 = 0;
			} 
			
			else if(but_flag1){
			blink_trig();
		}
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	}
}