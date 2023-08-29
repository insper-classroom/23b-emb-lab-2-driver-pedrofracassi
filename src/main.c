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

#define LED1_PIO           PIOA
#define LED1_PIO_ID        ID_PIOA
#define LED1_PIO_IDX       0
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)

#define LED2_PIO           PIOC
#define LED2_PIO_ID        ID_PIOC
#define LED2_PIO_IDX       30
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)

#define LED3_PIO           PIOB
#define LED3_PIO_ID        ID_PIOB
#define LED3_PIO_IDX       2
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)

#define BUT1_PIO           PIOD
#define BUT1_PIO_ID        ID_PIOD
#define BUT1_PIO_IDX       28
#define BUT1_PIO_IDX_MASK  (1 << BUT1_PIO_IDX)

#define BUT2_PIO           PIOC
#define BUT2_PIO_ID        ID_PIOC
#define BUT2_PIO_IDX       31
#define BUT2_PIO_IDX_MASK  (1 << BUT2_PIO_IDX)

#define BUT3_PIO           PIOA
#define BUT3_PIO_ID        ID_PIOA
#define BUT3_PIO_IDX       19
#define BUT3_PIO_IDX_MASK  (1 << BUT3_PIO_IDX)

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

// Do manual: The level driven on an I/O line can be determined by writing in the Set Output Data Register (PIO_SODR) and the Clear Output Data Register (PIO_CODR). 
void _pio_set(Pio *p_pio, const uint32_t mascara) {
	p_pio->PIO_SODR = mascara;
}

void _pio_clear(Pio *p_pio, const uint32_t mascara) {
	p_pio->PIO_CODR = mascara;
}

// The pull-up resistor can be enabled or disabled by writing to the Pull-up Enable Register (PIO_PUER) or Pull-up Disable Register (PIO_PUDR), respectively.
void _pio_pull_up(Pio *p_pio, const uint32_t mascara, int enable) {
	if (enable) {
		p_pio->PIO_PUER = mascara;
	} else {
		p_pio->PIO_PUDR = mascara;
	}
}

// When the I/O line is controlled by the PIO Controller, the pin can be configured to be driven. This is done by writing the Output Enable Register (PIO_OER) and Output Disable Register (PIO_ODR).
void _pio_set_output(Pio *p_pio, const uint32_t mascara, int a, int b, int c) {
	p_pio->PIO_OER = mascara;
}

void _pio_set_input(Pio *p_pio, const uint32_t mascara, const uint32_t a) {
	p_pio->PIO_ODR = mascara;
}

// The level on each I/O line can be read through PIO_PDSR. This register indicates the level of the I/O lines regardless of their configuration, whether uniquely as an input, or driven by the PIO Controller, or driven by a peripheral. 
uint32_t _pio_get(Pio *p_pio, const pio_type_t type, uint32_t mascara) {
	return p_pio->PIO_PDSR & mascara;
}

void _delay_ms(int millis) {
	// F_CPU -> quantos ciclos o processador realiza em 1000ms
	double cycles = cpu_ms_2_cy(millis, F_CPU);
	for (int i = 0; i < cycles; i++) {
		__NOP();
	}
}

// Função de inicialização do uC
void init(void)
{
	sysclk_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	pmc_enable_periph_clk(LED1_PIO_ID);
	_pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(LED2_PIO_ID);
	_pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(LED3_PIO_ID);
	_pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(BUT1_PIO_ID);
	_pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	
	pmc_enable_periph_clk(BUT2_PIO_ID);
	_pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	
	pmc_enable_periph_clk(BUT3_PIO_ID);
	_pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
	
	_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
	_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
	_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
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
	  int but1_val = _pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK);
	  
	  if (but1_val == 0) {
		  for (int i = 0; i < 5; i++) {
			_pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(200);
			_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(200);
		  }
	  }
	  
	  int but2_val = _pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK);
	  
	  if (but2_val == 0) {
		  for (int i = 0; i < 5; i++) {
			_pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
			delay_ms(200);
			_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			delay_ms(200);
			
		  }
	  }
	  
	  int but3_val = _pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK);
	  
	  if (but3_val == 0) {
		  for (int i = 0; i < 5; i++) {
			_pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
			delay_ms(200);
			_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
			delay_ms(200);
		  }
	  }
	
  }
  return 0;
}
