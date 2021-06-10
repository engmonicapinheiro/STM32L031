#include "timer.h"
#include "stm32l0xx.h"


/* the timer is being used here with polling */
void timer_init()
{
  //bit 0 corresponds to the TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  //the prescaler register
  TIM2->PSC = 30;  //1600-1;   //divide the timer clock by 1600 (16000000 / 1600)

  //access the auto reload register
  TIM2->ARR = 62500-1;//10000 - 1;  //10000 divided by 10000 = 1Hz

  //clear the timer counter
  TIM2->CNT |= 0x0;

  //enable the timer - bit 0
  TIM2->CR1 |= TIM_CR1_CEN;

}