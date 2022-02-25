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
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_PULLUP);
	
	// OLED1
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED1_PIO_ID);
	
	//Inicializa PC8 como saída
	pio_set_output(OLED1_PIO, OLED1_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO1_ID);
	
	pio_set_input(BUT_PIO1, BUT_PIO1_IDX_MASK, PIO_PULLUP);
	
	// OLED2
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED2_PIO_ID);
	
	//Inicializa PC8 como saída
	pio_set_output(OLED2_PIO, OLED2_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO2_ID);
	
	pio_set_input(BUT_PIO2, BUT_PIO2_IDX_MASK, PIO_PULLUP);
	
	
	
	
	//OLED3
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(OLED3_PIO_ID);
	
	//Inicializa PC8 como saída
	pio_set_output(OLED3_PIO, OLED3_PIO_IDX_MASK, 1, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO3_ID);
	
	pio_set_input(BUT_PIO3, BUT_PIO3_IDX_MASK, PIO_PULLUP);
	
	
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
	  if (!pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  pio_clear(LED_PIO, LED_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  delay_ms(200);                         // delay
			  pio_set(LED_PIO, LED_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  delay_ms(200);
			  }                         // delay
	  }
	  
	  if (!pio_get(BUT_PIO1, PIO_INPUT, BUT_PIO1_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  pio_clear(OLED1_PIO, OLED1_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  delay_ms(200);                         // delay
			  pio_set(OLED1_PIO, OLED1_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  delay_ms(200);
		  }                         // delay
	  }
	  
	  if (!pio_get(BUT_PIO2, PIO_INPUT, BUT_PIO2_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  pio_clear(OLED2_PIO, OLED2_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  delay_ms(200);                         // delay
			  pio_set(OLED2_PIO, OLED2_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  delay_ms(200);
		  }                         // delay
	  }
	  
	  if (!pio_get(BUT_PIO3, PIO_INPUT, BUT_PIO3_IDX_MASK))
	  {
		  for (int i=0; i<5; i++) {
			  pio_clear(OLED3_PIO, OLED3_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  delay_ms(200);                         // delay
			  pio_set(OLED3_PIO, OLED3_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  delay_ms(200);
		  }                         // delay
	  }
	
  }
  return 0;
}
