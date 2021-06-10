
#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"

/*********************************************************************
*
*  Project: just blink a LED at PB3    
* using the BSRR register.
* board: STM32L031K6T6
*   date: May 2020
*/


void delay(unsigned int n_intervals);




void main(void) 
{
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;   //clock enable for GPIOB


  /* set the direction register:
   * configure the desired IO as output  */
  GPIOB->MODER &= ~(0b11 << 6);
  GPIOB->MODER |= (0b01 <<6);

  while(1)
  {
    GPIOB->BSRR |= 0x08;  //set PB3 high
    delay(1000);
    GPIOB->BSRR |= 0x80000; //reset PB3 = 0
    delay(1000);
  }


}



void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
	for(i = 250; i != 0; i--)
	;
}
