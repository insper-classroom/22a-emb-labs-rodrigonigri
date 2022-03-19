#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************/
/* defines                                                              */
/************************/

// PLACA SAME70-XPLD
#define LED_PIO					PIOC                 
#define LED_PIO_ID				ID_PIOC              
#define LED_PIO_IDX				8                    
#define LED_PIO_IDX_MASK		(1 << LED_PIO_IDX)
#define BUT_PIO					PIOA
#define BUT_PIO_ID				ID_PIOA
#define BUT_PIO_IDX				11
#define BUT_PIO_IDX_MASK		(1u << BUT_PIO_IDX) 

// OLED 1
#define OLED_PIO_1				PIOA
#define OLED_PIO_ID_1			ID_PIOA
#define OLED_PIO_IDX_1			0
#define OLED_PIO_IDX_MASK_1		(1<<OLED_PIO_IDX_1)
#define BUT_PIO_1				PIOD
#define BUT_PIO_ID_1			ID_PIOD
#define BUT_PIO_IDX_1			28
#define BUT_PIO_IDX_MASK_1		(1u << BUT_PIO_IDX_1)

// OLED 2
#define OLED_PIO_2				PIOC
#define OLED_PIO_ID_2			ID_PIOC
#define OLED_PIO_IDX_2			30
#define OLED_PIO_IDX_MASK_2		(1<<OLED_PIO_IDX_2)


// OLED 3
#define OLED_PIO_3				PIOB
#define OLED_PIO_ID_3			ID_PIOB
#define OLED_PIO_IDX_3			2
#define OLED_PIO_IDX_MASK_3		(1<<OLED_PIO_IDX_3)



/**
 *  Informacoes para o RTC
 *  poderia ser extraida do _DATE_ e _TIME_
 *  ou ser atualizado pelo PC.
 */


typedef struct  {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t week;
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
} calendar;

/************************/
/* prototype                                                            */
/************************/
void io_init(void);
void draw_time(void);
void pisca_led(int n, int t);
void LED_init(int estado);
void OLED_1_init(int estado);
void OLED_3_init(int estado);
void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);
void pin_toggle(Pio *pio, uint32_t mask);
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);
void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);

volatile char but_flag;
volatile char but_flag_1;
volatile char but_flag_2;
volatile char but_flag_3;
volatile char esta_piscando;
volatile char break_piscando;
volatile char flag_tc = 0;
volatile char flag_rtt_alarm = 0;
volatile char flag_rtc_alarm = 0;
volatile char flag_rtc_count = 0;

/************************/
/* Handlers                                                             */
/************************/

/**
* \brief Interrupt handler for the RTC. Refresh the display.
*/
void RTC_Handler(void) {
	uint32_t ul_status = rtc_get_status(RTC);
	
	/* seccond tick */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		flag_rtc_count = 1;
	}
	
	/* Time or date alarm */
	if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
		// o c�digo para irq de alame vem aqui
		flag_rtc_alarm = 1;
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

void TC0_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrup��o foi satisfeita.
	* Isso � realizado pela leitura do status do perif�rico
	**/
	volatile uint32_t status = tc_get_status(TC0, 0);

	/* Muda o estado do LED (pisca) */
	pin_toggle(LED_PIO, LED_PIO_IDX_MASK);  
}

void TC1_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrup��o foi satisfeita.
	* Isso � realizado pela leitura do status do perif�rico
	**/
	volatile uint32_t status = tc_get_status(TC0, 1);

	/* Muda o estado do LED (pisca) */
	pin_toggle(OLED_PIO_1, OLED_PIO_IDX_MASK_1);  
}

void LED_init(int estado) {
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, estado, 0, 0);
};

void OLED_1_init(int estado) {
	pmc_enable_periph_clk(OLED_PIO_ID_1);
	pio_set_output(OLED_PIO_1, OLED_PIO_IDX_MASK_1, estado, 0, 0);
};

void OLED_3_init(int estado) {
	pmc_enable_periph_clk(OLED_PIO_ID_3);
	pio_set_output(OLED_PIO_3, OLED_PIO_IDX_MASK_3, estado, 0, 0);
};

void pin_toggle(Pio *pio, uint32_t mask) {
	if(pio_get_output_data_status(pio, mask))
	pio_clear(pio, mask);
	else
	pio_set(pio,mask);
}

static float get_time_rtt(){
	uint ul_previous_time = rtt_read_timer_value(RTT);
}
/** 
 * Configura RTT
 *
 * arg0 pllPreScale  : Frequ�ncia na qual o contador ir� incrementar
 * arg1 IrqNPulses   : Valor do alarme 
 * arg2 rttIRQSource : Pode ser uma 
 *     - 0: 
 *     - RTT_MR_RTTINCIEN: Interrup��o por incremento (pllPreScale)
 *     - RTT_MR_ALMIEN : Interrup��o por alarme
 */
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

void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configura o PMC */
	pmc_enable_periph_clk(ID_TC);

	/** Configura o TC para operar em  freq hz e interrup�c�o no RC compare */
	tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC, TC_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq);

	/* Configura NVIC*/
	NVIC_SetPriority(ID_TC, 4);
	NVIC_EnableIRQ((IRQn_Type) ID_TC);
	tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);
}

void RTT_Handler(void) {
	uint32_t ul_status;

	/* Get RTT status - ACK */
	ul_status = rtt_get_status(RTT);
	
	/* IRQ due to Time has changed */
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC) {
	}

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		flag_rtt_alarm = 1;
	}
}

void but_callback_1(void){
	if (but_flag_1) {
		but_flag_1 = 0;
		} 
		else {
		but_flag_1 =1;
	}}



/************************/
/* fun��es                                                              */
/************************/

void io_init(void)
{

	// Configura led
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_PIO_IDX_MASK, PIO_DEFAULT);
	
	pmc_enable_periph_clk(OLED_PIO_ID_1);
	pio_configure(OLED_PIO_1, PIO_OUTPUT_0, OLED_PIO_IDX_MASK_1, PIO_DEFAULT);
	
	pmc_enable_periph_clk(OLED_PIO_ID_2);
	pio_configure(OLED_PIO_2, PIO_OUTPUT_0, OLED_PIO_IDX_MASK_2, PIO_DEFAULT);
	
	pmc_enable_periph_clk(OLED_PIO_ID_3);
	pio_configure(OLED_PIO_3, PIO_OUTPUT_0, OLED_PIO_IDX_MASK_3, PIO_DEFAULT);
	
	// Inicializa clock do perif�rico PIO responsavel pelo botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID_1);


	// Configura PIO para lidar com o pino do bot�o como entrada
	// com pull-up
	pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT_PIO_IDX_MASK, 60);
	
	pio_configure(BUT_PIO_1, PIO_INPUT, BUT_PIO_IDX_MASK_1, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO_1, BUT_PIO_IDX_MASK_1, 60);
	


	// Configura interrup��o no pino referente ao botao e associa
	// fun��o de callback caso uma interrup��o for gerada
	// a fun��o de callback � a: but_callback()
	
	pio_handler_set(BUT_PIO_1,
	BUT_PIO_ID_1,
	BUT_PIO_IDX_MASK_1,
	PIO_IT_RISE_EDGE,
	but_callback_1);
	
	// Ativa interrup��o e limpa primeira IRQ gerada na ativacao
	
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO);
	
	pio_enable_interrupt(BUT_PIO_1, BUT_PIO_IDX_MASK_1);
	pio_get_interrupt_status(BUT_PIO_1);
	
		
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr�ximo de 0 maior)
	
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4);
	
	NVIC_EnableIRQ(BUT_PIO_ID_1);
	NVIC_SetPriority(BUT_PIO_ID_1, 4);
	
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
	
	LED_init(1);
	TC_init(TC0, ID_TC0, 0, 5);
	tc_start(TC0, 0);
	
	OLED_1_init(1);
	TC_init(TC0, ID_TC1, 1, 4);
	tc_start(TC0, 1);
	
	RTT_init(4, 16, RTT_MR_ALMIEN);  
	
	/* Configura Leds */
	OLED_3_init(1);
	
	/** Configura RTC */
	calendar rtc_initial = {2018, 3, 19, 12, 15, 45 ,1};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN | RTC_IER_SECEN);
	
	
  /* Insert application code here, after the board has been initialized. */
	while(1) {
		
		
		if (!but_flag_1) {
			/* Leitura do valor atual do RTC */
			uint32_t current_hour, current_min, current_sec;
			uint32_t current_year, current_month, current_day, current_week;
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
			rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);

			/* configura alarme do RTC para daqui 20 segundos */
			rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
			if (current_sec + 20 >= 60) {
				rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min+1, 1, current_sec + 20 - 60);
			}
			else {
				rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 20);
			}
		}
		
		if (flag_rtt_alarm) {
			// Inverte sinal do LED2 e reprograma o alarme
			pin_toggle(OLED_PIO_2, OLED_PIO_IDX_MASK_2);
			RTT_init(4, 16, RTT_MR_ALMIEN);
		}
		
		if (flag_rtc_count) {
			gfx_mono_draw_line(0, 0, 128, 32, GFX_PIXEL_CLR);
			uint32_t current_hour, current_min, current_sec;
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
			char time_display[8];
			sprintf(time_display, "%2d:%2d:%2d", current_hour, current_min,current_sec);
			gfx_mono_draw_string(time_display, 25, 16, &sysfont);
		}

		if (flag_rtc_alarm) {
			// Pisca o LED3
			pin_toggle(OLED_PIO_3, OLED_PIO_IDX_MASK_3);
			delay_ms(100);
			pin_toggle(OLED_PIO_3, OLED_PIO_IDX_MASK_3);
		}
		
		
		flag_rtt_alarm=0;
		flag_rtc_alarm=0;
		flag_rtc_count=0;
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
		
	}
}