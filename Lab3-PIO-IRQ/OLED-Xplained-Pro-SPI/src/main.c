#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"


// --- DEFINES ---

// LED PLACA
#define LED_PIO           PIOC
#define LED_PIO_ID        ID_PIOC
#define LED_PIO_IDX       8
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)

// BOTAO 1
#define BUT_PIO1			PIOD
#define BUT_PIO1_ID			ID_PIOD
#define BUT_PIO1_IDX		28
#define BUT_PIO1_IDX_MASK	(1u << BUT_PIO1_IDX)

// BOTAO 2
#define BUT_PIO2			PIOC
#define BUT_PIO2_ID		ID_PIOC
#define BUT_PIO2_IDX		31
#define BUT_PIO2_IDX_MASK (1u << BUT_PIO2_IDX)

// // BOTAO 3
#define BUT_PIO3			PIOA
#define BUT_PIO3_ID		ID_PIOA
#define BUT_PIO3_IDX		19
#define BUT_PIO3_IDX_MASK (1u << BUT_PIO3_IDX)


// --- FUNCOES ---
void pisca_led(int n, int t){
	for (int i=0;i<n;i++){
		pio_clear(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(t);
		pio_set(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(t);
	}
}

// --- VARIAVEIS GLOBAL ---

volatile char but_flag=0;
volatile char but_flag_2=0;
volatile char but_flag_3=0;

// --- PROTOTYPE ---

void pisca_led(int n, int t);


// --- FUNCOES ---

void but_callback(void){	
	    if (pio_get(BUT_PIO1, PIO_INPUT, BUT_PIO1_IDX_MASK)) {
			but_flag = 0;
		    // PINO == 1 --> Borda de subida
		  } 
		  else {
		    // PINO == 0 --> Borda de descida
			but_flag=1;
	    }
}

void but_callback_2(void) {
	if (but_flag_2) {
		but_flag_2 = 0;
		} 
		else {
		but_flag_2 =1;
	}
}

void but_callback_3(void){
	if (pio_get(BUT_PIO3, PIO_INPUT, BUT_PIO3_IDX_MASK)) {
		but_flag_3 = 0;
		// PINO == 1 --> Borda de subida
	}
	else {
		// PINO == 0 --> Borda de descida
		but_flag_3 = 1;
	}
}


void init(){
	// Initialize the board clock
	sysclk_init();
	
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	
	//Inicializa PC8 como sa�da
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO1_ID);
	pmc_enable_periph_clk(BUT_PIO2_ID);
	pmc_enable_periph_clk(BUT_PIO3_ID);
	
	pio_set_input(BUT_PIO1, BUT_PIO1_IDX_MASK, PIO_PULLUP);
	pio_set_input(BUT_PIO2, BUT_PIO2_IDX_MASK, PIO_PULLUP);
	pio_set_input(BUT_PIO3, BUT_PIO3_IDX_MASK, PIO_PULLUP);
	
	// FUNCAO DE INTERRUPCAO
	pio_handler_set(BUT_PIO1,BUT_PIO1_ID,BUT_PIO1_IDX_MASK, PIO_IT_EDGE, but_callback);
	pio_handler_set(BUT_PIO2,BUT_PIO2_ID,BUT_PIO2_IDX_MASK, PIO_IT_RISE_EDGE, but_callback_2);
	pio_handler_set(BUT_PIO3,BUT_PIO3_ID,BUT_PIO3_IDX_MASK, PIO_IT_EDGE, but_callback_3);
	
	// Ativa interrup��o e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT_PIO1, BUT_PIO1_IDX_MASK);
	pio_enable_interrupt(BUT_PIO2, BUT_PIO2_IDX_MASK);
	pio_enable_interrupt(BUT_PIO3, BUT_PIO3_IDX_MASK);
	
	
	pio_get_interrupt_status(BUT_PIO1);
	pio_get_interrupt_status(BUT_PIO2);
	pio_get_interrupt_status(BUT_PIO3);
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr�ximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO1_ID);
	NVIC_EnableIRQ(BUT_PIO2_ID);
	NVIC_EnableIRQ(BUT_PIO3_ID);
	
	NVIC_SetPriority(BUT_PIO1_ID, 4); // Prioridade 4
	NVIC_SetPriority(BUT_PIO2_ID, 4);
	NVIC_SetPriority(BUT_PIO3_ID, 4);
}

int main (void){
	init();
	board_init();
	sysclk_init();
	delay_init();

	// Init OLED
	gfx_mono_ssd1306_init();

	gfx_mono_draw_string("delay:", 35,16, &sysfont);
	
	
	int delay = 500;
	char str[128];
	sprintf(str, "%d", delay);
	gfx_mono_draw_string(str , 100, 16, &sysfont);
	/* Insert application code here, after the board has been initialized. */
	
	gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
	gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
	
	while(1) {
		
		if (!but_flag_2){
			pisca_led(1, delay);
			}
		
		
		//pisca_led(delay,2 );
		if(but_flag){
			int i=0;
			while(i<=200 && but_flag == 1){
				delay_ms(1);
				i++;
			}
			
			if (i>200){
				delay += 100;
				sprintf(str, "%d", delay);
				gfx_mono_draw_string(str , 100, 16, &sysfont);
				
				
				gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
				gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
				gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);

			}
			else if (i<200){
				delay -= 100;
				sprintf(str, "%d", delay);
				gfx_mono_draw_string(str , 100, 16, &sysfont);
				
				
				gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
				gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
				gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
			}
			
		
			
			but_flag_2 = 0;
		}
		if(but_flag_3){
			delay -=100;
			sprintf(str, "%d", delay);
			gfx_mono_draw_string(str , 100, 16, &sysfont);
			gfx_mono_draw_string(str , 100, 16, &sysfont);
			gfx_mono_draw_filled_rect(10, 5, 110, 10, GFX_PIXEL_CLR);
			gfx_mono_draw_rect(10,5,110, 10, GFX_PIXEL_SET);
			gfx_mono_draw_filled_rect(10, 5, 11*(delay/100), 10, GFX_PIXEL_SET);
		}
		
	}
}
