#include <asf.h>
#include <defines.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

volatile char but_flag_1;
volatile char but_flag_2;
volatile char but_flag_3;
volatile char rtt_alarm_flag = 0;
volatile char rtc_alarm_flag = 0;
volatile char rtc_flag_count = 0;

typedef struct  {
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t week;
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
} calendar;

/--------------------------Prototypes --------------------------/

void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);
void pin_toggle(Pio *pio, uint32_t mask);

/-------------------- Init & Butcallbacks -----------------------/

void but1_callback(void)
{
	but_flag_1 = !but_flag_1;
}
	
void but2_callback(void)
{
	but_flag_2 = !but_flag_2;
}
	
void but3_callback(void)
{
	but_flag_3 = !but_flag_3;
}

void io_init(void)
{

	// Inicializando LED 1
	pmc_enable_periph_clk(OLED_1_PIO_ID);
	pio_set_output(OLED_1_PIO, OLED_1_PIO_IDX_MASK, 0, 0, 0);
	
	//Inicializando botao 1
	pmc_enable_periph_clk(BUT_PIO_1_ID);
	pio_configure(BUT_PIO_1, PIO_INPUT, BUT_PIO_1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(BUT_PIO_1, BUT_PIO_1_IDX_MASK, PIO_PULLUP);
	pio_handler_set(BUT_PIO_1,BUT_PIO_1_ID,	BUT_PIO_1_IDX_MASK,PIO_IT_RISE_EDGE,but1_callback);
	pio_enable_interrupt(BUT_PIO_1, BUT_PIO_1_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_1);
	NVIC_EnableIRQ(BUT_PIO_1_ID);
	NVIC_SetPriority(BUT_PIO_1_ID, 4);
	
	// Inicializando LED 2
	pmc_enable_periph_clk(OLED_2_PIO_ID);
	pio_set_output(OLED_2_PIO, OLED_2_PIO_IDX_MASK, 1, 0, 0);
	
	//Inicializando botao 2
	pmc_enable_periph_clk(BUT_PIO_2_ID);
	pio_configure(BUT_PIO_2, PIO_INPUT, BUT_PIO_2_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(BUT_PIO_2, BUT_PIO_2_IDX_MASK, PIO_PULLUP);
	pio_handler_set(BUT_PIO_2,BUT_PIO_2_ID,	BUT_PIO_2_IDX_MASK,PIO_IT_FALL_EDGE,but2_callback);
	pio_enable_interrupt(BUT_PIO_2, BUT_PIO_2_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_2);
	NVIC_EnableIRQ(BUT_PIO_2_ID);
	NVIC_SetPriority(BUT_PIO_2_ID, 4);
	
	
	// Inicializando LED 3
	pmc_enable_periph_clk(OLED_3_PIO_ID);
	pio_set_output(OLED_3_PIO, OLED_3_PIO_IDX_MASK, 0, 0, 0);
	
	//Inicializando botao 3
	pmc_enable_periph_clk(BUT_PIO_3_ID);
	pio_configure(BUT_PIO_3, PIO_INPUT, BUT_PIO_3_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(BUT_PIO_3, BUT_PIO_3_IDX_MASK, PIO_PULLUP);
	pio_handler_set(BUT_PIO_3,BUT_PIO_3_ID,	BUT_PIO_3_IDX_MASK,PIO_IT_RISE_EDGE,but3_callback);
	pio_enable_interrupt(BUT_PIO_3, BUT_PIO_3_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO_3);
	NVIC_EnableIRQ(BUT_PIO_3_ID);
	NVIC_SetPriority(BUT_PIO_3_ID, 4);
}

/------------------------ TC initiation -------------------------/

void TC0_Handler(void) {
	volatile uint32_t status = tc_get_status(TC0, 0);
	pin_toggle(OLED_1_PIO, OLED_1_PIO_IDX_MASK);  
}

void TC1_Handler(void) {
	volatile uint32_t status = tc_get_status(TC0, 1);
	pin_toggle(OLED_2_PIO, OLED_2_PIO_IDX_MASK);
}

void TC2_Handler(void) {
	volatile uint32_t status = tc_get_status(TC0, 2);
	pin_toggle(OLED_3_PIO, OLED_3_PIO_IDX_MASK);
}

void pin_toggle(Pio *pio, uint32_t mask) {
	if(pio_get_output_data_status(pio, mask))
	pio_clear(pio, mask);
	else
	pio_set(pio,mask);
}

void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configura o PMC */
	pmc_enable_periph_clk(ID_TC);

	/** Configura o TC para operar em  freq hz e interrupçcão no RC compare */
	tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC, TC_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq);

	/* Configura NVIC*/
	NVIC_SetPriority(ID_TC, 4);
	NVIC_EnableIRQ((IRQn_Type) ID_TC);
	tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);
}

/------------------------ RTT initiation -------------------------/

void RTT_Handler(void) {
	uint32_t ul_status;

	/* Get RTT status - ACK */
	ul_status = rtt_get_status(RTT);

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		RTT_init(10, 50, RTT_MR_ALMIEN);
		rtt_alarm_flag = !rtt_alarm_flag;
	}
	
	/* IRQ due to Time has changed */
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC) {
	}
}

static float get_time_rtt(){
	uint ul_previous_time = rtt_read_timer_value(RTT);
}

static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

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
	if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN))
	rtt_enable_interrupt(RTT, rttIRQSource);
	else
	rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);
	
}

/------------------------ RTC initiation -------------------------/

void RTC_Handler(void) {
	uint32_t ul_status = rtc_get_status(RTC);
	
	/* seccond tick */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		// o código para irq de segundo vem aqui
		rtc_flag_count = 1;
	}
	
	/* Time or date alarm */
	if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
		// o código para irq de alame vem aqui
		rtc_alarm_flag = 1;
	}

	rtc_clear_status(RTC, RTC_SCCR_SECCLR);
	rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
	rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
	rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
	rtc_clear_status(RTC, RTC_SCCR_CALCLR);
	rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type) {
	/* Configura o PMC */
	pmc_enable_periph_clk(ID_RTC);

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(rtc, 0);

	/* Configura data e hora manualmente */
	rtc_set_date(rtc, t.year, t.month, t.day, t.week);
	rtc_set_time(rtc, t.hour, t.minute, t.second);

	/* Configure RTC interrupts */
	NVIC_DisableIRQ(id_rtc);
	NVIC_ClearPendingIRQ(id_rtc);
	NVIC_SetPriority(id_rtc, 4);
	NVIC_EnableIRQ(id_rtc);

	/* Ativa interrupcao via alarme */
	rtc_enable_interrupt(rtc,  irq_type);
}

char* time_to_string(int n, char type) {
	int retnum;
	if (type == 'h') {
		retnum = n % 24;
		} else if (type == 'm' || type == 's') {
		retnum = n % 60;
		} else {
		retnum = 0;
	}
	
	char* retval = (char*)malloc(2 * sizeof(char));
	
	if (retnum <= 9) {
		sprintf(retval, "0%d", retnum);
		} else {
		sprintf(retval, "%d", retnum);
	}
	
	return retval;
}


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	io_init();

  // Init OLED
	gfx_mono_ssd1306_init();
	 
	 
	 // Pisca led 1
	 int freq_led_1 = 5;
	 TC_init(TC0, ID_TC0, 0, freq_led_1);
	 tc_start(TC0, 0);
	 
	 // Pisca led 2
	 int freq_led_2 = 10;
	 TC_init(TC0, ID_TC1, 1, freq_led_2);
	 tc_start(TC0, 1);
	 
	 // Pisca led 3
	 int freq_led_3 = 1;
	 TC_init(TC0, ID_TC2, 2, freq_led_3);
	 tc_start(TC0, 2);
	 
	 RTT_init(10, 50, RTT_MR_ALMIEN);
	 
	/** Configura RTC */
	calendar rtc_initial = {2018, 3, 19, 12, 15, 45 ,1};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN);
	     
	/* Leitura do valor atual do RTC */
	uint32_t current_hour, current_min, current_sec;
	uint32_t current_year, current_month, current_day, current_week;
	rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
	rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
	     
	/* configura alarme do RTC para daqui 1 segundos */
	rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
	rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 1);
		 
	 char str[128];
	 sprintf(str, "%d %d %d", freq_led_1, freq_led_2, freq_led_3);
	 gfx_mono_draw_string(str , 10, 5, &sysfont);
	 
	 
	 int i = 10;
	 
  /* Insert application code here, after the board has been initialized. */
	while(1) {
		
		if (rtt_alarm_flag) {
			pio_clear(OLED_1_PIO, OLED_1_PIO_IDX_MASK);
			pio_clear(OLED_2_PIO, OLED_2_PIO_IDX_MASK);
			pio_clear(OLED_3_PIO, OLED_3_PIO_IDX_MASK);
		}
		
		else {
			if (but_flag_1) {
				pio_clear(OLED_1_PIO, OLED_1_PIO_IDX_MASK);
			}
			if (but_flag_2) {
				pio_set(OLED_2_PIO, OLED_2_PIO_IDX_MASK);
			}
			if (but_flag_3) {
				pio_clear(OLED_3_PIO, OLED_3_PIO_IDX_MASK);
			}	
		}
		
		if (rtc_flag_count) {
			/*gfx_mono_draw_line(0, 0, 128, 32, GFX_PIXEL_CLR);
			uint32_t current_hour, current_min, current_sec;
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
			char time_display[128];
			sprintf(time_display, "%s:%s:%s", time_to_string(current_hour, 'h'), time_to_string(current_min, 'm'), time_to_string(current_sec, 's'));
			gfx_mono_draw_string(time_display, 10, 16, &sysfont);
			rtc_flag_count = 0;*/
			
			//gfx_mono_draw_filled_rect(10, 16, 64, 32, GFX_PIXEL_CLR);
		}
		gfx_mono_draw_string("oo", i, 16, &sysfont);
	
	}
}

