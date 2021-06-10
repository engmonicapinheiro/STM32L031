/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2014 - 2019 SEGGER Microcontroller GmbH             *
*                                                                    *
*           www.segger.com     Support: support@segger.com           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* conditions are met:                                                *
*                                                                    *
* - Redistributions of source code must retain the above copyright   *
*   notice, this list of conditions and the following disclaimer.    *
*                                                                    *
* - Neither the name of SEGGER Microcontroller GmbH                  *
*   nor the names of its contributors may be used to endorse or      *
*   promote products derived from this software without specific     *
*   prior written permission.                                        *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED.                                                        *
* IN NO EVENT SHALL SEGGER Microcontroller GmbH BE LIABLE FOR        *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"

void LED_init(void);
void timer_init(void);

/*********************************************************************
*
*  Timer2: APB2 bus     
*  Purpose: to create a 1Hz timer to toggle an LED
*   1Hz = one cycle/second --> 1 second delay
*/

void main(void)
{
   LED_init();
   timer_init();

   while(1)
   {
     //poll the status register
      while((TIM2->SR & 0x1))
      {
        
         TIM2->SR &= ~(0x1);
         GPIOA->ODR ^= 0x1;  //PA1
         GPIOA->ODR ^= 0x2;  //PA0
          
      }
   }

}

void LED_init()
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  GPIOA->MODER &= ~(10 << 0);  //for PA0, blue led - MODE0
  GPIOA->MODER |= (01 << 0);
  GPIOA->MODER &= ~(10 << 2); //for PA1, green led - MODE1
  GPIOA->MODER |= (01 << 2);
}

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


/*************************** End of file ****************************/
