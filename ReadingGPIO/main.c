

#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"


void delay(unsigned int n_intervals);


/*
* Reads the value of a GPIO connected to a button.
* pressing the button will turn the LED off.
* This code is just to exercise the registers associated with GPIO
*  at the STM32L031.
*   
*/


void main(void)
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;    //enables PORTA clock
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;   //clock enable for GPIOB
  
  GPIOA->MODER &= ~(0b11 << 5);   
  GPIOA->MODER |=  (0b00 << 5);  //port A set as input = 0

  GPIOB->MODER &= ~(0b11 << 6);
  GPIOB->MODER |= (0b01 <<6);

  while(1)
  {

    if(GPIOA->IDR & 0x10 ) //if I press the button (input = 1) if PA4,5,6,7 is true
    {
      GPIOB->ODR &=~ 0x8; //turn off led
    }
    else
    {
      GPIOB->ODR |= 0x8; //turn on the LED
    }
  
  }
}


void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
	for(i = 250; i != 0; i--)
	;
}

/*************************** End of file ****************************/
