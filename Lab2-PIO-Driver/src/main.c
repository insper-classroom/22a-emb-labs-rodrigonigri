/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO           PIOC
#define LED_PIO_ID        ID_PIOC
#define LED_PIO_IDX       8
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)


#define BUT_PIO			  PIOA
#define BUT_PIO_ID        ID_PIOA
#define BUT_PIO_IDX		  11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)

// OLED1

#define OLED1_PIO	   	     PIOA
#define OLED1_PIO_ID	     ID_PIOA
#define OLED1_PIO_IDX	     0
#define OLED1_PIO_IDX_MASK (1<<OLED1_PIO_IDX)


#define BUT_PIO1			PIOD
#define BUT_PIO1_ID		ID_PIOD
#define BUT_PIO1_IDX		28
#define BUT_PIO1_IDX_MASK (1u << BUT_PIO1_IDX)

// OLED2

#define OLED2_PIO		PIOC
#define OLED2_PIO_ID	ID_PIOC
#define OLED2_PIO_IDX	30
#define OLED2_PIO_IDX_MASK (1<<OLED2_PIO_IDX)


#define BUT_PIO2			PIOC
#define BUT_PIO2_ID		ID_PIOC
#define BUT_PIO2_IDX		31
#define BUT_PIO2_IDX_MASK (1u << BUT_PIO2_IDX)


// OLED3

#define OLED3_PIO		      PIOB
#define OLED3_PIO_ID	      ID_PIOB
#define OLED3_PIO_IDX	      2
#define OLED3_PIO_IDX_MASK   (1<<OLED3_PIO_IDX)


#define BUT_PIO3			PIOA
#define BUT_PIO3_ID		ID_PIOA
#define BUT_PIO3_IDX		19
#define BUT_PIO3_IDX_MASK (1u << BUT_PIO3_IDX)

/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/



// ############################################################ LAB 2 ############################################################


void _pio_set(Pio *p_pio, const uint32_t ul_mask){
	p_pio->PIO_SODR = ul_mask;
}

void _pio_clear(Pio *p_pio, const uint32_t ul_mask){
	p_pio->PIO_CODR = ul_mask;
}

void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_up_enable){
	if (ul_pull_up_enable){
		p_pio->PIO_PUER = ul_mask;
	}
	else{
		p_pio->PIO_PUDR = ul_mask;
	}
	
}

void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute)
{
	pio_disable_interrupt(p_pio, ul_mask);
	pio_pull_up(p_pio, ul_mask, ul_attribute & PIO_PULLUP);

	if (ul_attribute & (PIO_DEGLITCH | PIO_DEBOUNCE)) {
		p_pio->PIO_IFER = ul_mask;
		} else {
		p_pio->PIO_IFDR = ul_mask;
	}
}

void _pio_set_output(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_default_level, const uint32_t ul_multidrive_enable, const uint32_t ul_pull_up_enable){
	
	p_pio->PIO_PER = ul_mask;
	p_pio->PIO_OER = ul_mask;

	if (ul_multidrive_enable) {
		p_pio->PIO_MDER = ul_mask;
		} 
	else {
		p_pio->PIO_MDDR = ul_mask;
	}

	if (ul_default_level) {
		p_pio->PIO_SODR = ul_mask;
		} 
	else {
		p_pio->PIO_CODR = ul_mask;
	}
	
	
	pio_pull_up(p_pio, ul_mask, ul_pull_up_enable);
	pio_disable_interrupt(p_pio, ul_mask);
}

uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask){
	uint32_t ul_reg;

	if ((ul_type == PIO_OUTPUT_0) || (ul_type == PIO_OUTPUT_1)) {
		ul_reg = p_pio->PIO_ODSR;
		} 
	else {
		ul_reg = p_pio->PIO_PDSR;
	}

	if ((ul_reg & ul_mask) == 0) {
		return 0;
		} 
	else {
		return 1;
	}
}

void _delay_ms(int delay){

	for (int i = 0; i < delay*(30000/5) ; i+=1){ //valor alterado na mão mesmo!
		asm("nop");
	}
}


// ############################################################ FIM LAB 2 ############################################################

// Função de inicialização do uC
void init(void){
	// Initialize the board clock
	sysclk_init();

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	
	//Inicializa PC8 como saída
	_pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	
	_pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_PULLUP);
	
	// OLED1
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED1_PIO_ID);
	
	//Inicializa PC8 como saída
	_pio_set_output(OLED1_PIO, OLED1_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO1_ID);
	
	_pio_set_input(BUT_PIO1, BUT_PIO1_IDX_MASK, PIO_PULLUP);
	
	// OLED2
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED2_PIO_ID);
	
	//Inicializa PC8 como saída
	_pio_set_output(OLED2_PIO, OLED2_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO2_ID);
	
	_pio_set_input(BUT_PIO2, BUT_PIO2_IDX_MASK, PIO_PULLUP);
	
	
	
	
	//OLED3
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED3_PIO_ID);
	
	//Inicializa PC8 como saída
	_pio_set_output(OLED3_PIO, OLED3_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO3_ID);
	
	_pio_set_input(BUT_PIO3, BUT_PIO3_IDX_MASK, PIO_PULLUP);
	
	
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
  while (1)
  {
	  if (!_pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  _pio_clear(LED_PIO, LED_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  _delay_ms(200);                         // delay
			  _pio_set(LED_PIO, LED_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  _delay_ms(200);
			  }                         // delay
	  }
	  
	  if (!_pio_get(BUT_PIO1, PIO_INPUT, BUT_PIO1_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  _pio_clear(OLED1_PIO, OLED1_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  _delay_ms(200);                         // delay
			  _pio_set(OLED1_PIO, OLED1_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  _delay_ms(200);
		  }                         // delay
	  }
	  
	  if (!_pio_get(BUT_PIO2, PIO_INPUT, BUT_PIO2_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  _pio_clear(OLED2_PIO, OLED2_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  _delay_ms(200);                         // delay
			  _pio_set(OLED2_PIO, OLED2_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  _delay_ms(200);
		  }                         // delay
	  }
	  
	  if (!_pio_get(BUT_PIO3, PIO_INPUT, BUT_PIO3_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  _pio_clear(OLED3_PIO, OLED3_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  _delay_ms(200);                         // delay
			  _pio_set(OLED3_PIO, OLED3_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  _delay_ms(200);
		  }                         // delay
	  }
	
  }
  return 0;
}
