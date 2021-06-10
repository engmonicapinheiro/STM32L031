/*
File    : main.c
Purpose : create TIM2 tp toggle an LED at 1Hz
System clock is 16MHz
*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"

void LED_init(void);
void timer_init(void);
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
void main(void)
{
  __disable_irq();

  LED_init();
  timer_init();

  __enable_irq();

  while(1) 
  {}

}


void TIM2_IRQHandler(void)
{
  TIM2->SR &= ~(0x1);
  GPIOA->ODR ^= 0x02;

}


void timer_init()
{
  //bit 0 corresponds to the TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  //the prescaler register
  TIM2->PSC = 29;   //divide the timer clock by 30 

  //access the auto reload register
  TIM2->ARR = 62500-1;//10000 - 1;  

  //clear the timer counter
  //TIM2->CNT |= 0x0;

  //enable the timer - bit 0
  TIM2->CR1 |= TIM_CR1_CEN;

  //interrupt enable register
  TIM2->DIER |= TIM_DIER_UIE;

  //enable IRQ
  NVIC_EnableIRQ(TIM2_IRQn);

}


void LED_init()
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  GPIOA->MODER &= ~(10 << 0);  //for PA0, blue led - MODE0
  GPIOA->MODER |= (01 << 0);
  GPIOA->MODER &= ~(10 << 2); //for PA1, green led - MODE1
  GPIOA->MODER |= (01 << 2);
}
/*************************** End of file ****************************/
