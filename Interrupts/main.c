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

///////////////////////////////////////////////////////////////////////////
void delayms(int delay);



int main()
{
  __disable_irq();

  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  //enable clock access for SYSCONFIG
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  //bit 0

  /*resets PA1 moderA (MODE1) register */
  GPIOA->MODER &= ~(11 << 2); 

  //config PA1 as output (red led)
  GPIOA->MODER |= (01 << 2); 


  /* resets PA3 PUPDR register (input port to be used with pull-up) */
  GPIOA->MODER &= ~(11 << 6);
  GPIOA->PUPDR &= ~(01 << 6);


  //config PA3 as pull-up input
  GPIOA->MODER |= (00 << 6);  
  GPIOA->PUPDR |= (01 << 6);

  //the register here is EXTICR1 at the brackets
  SYSCFG->EXTICR[0] = 0x0000;//0x0FFF;  //0000 is the sequence for PA; 0 at MSB position is 3

  //unmask EXTI1
  EXTI->IMR |= 0x8;

  //select falling edge trigger
  EXTI->FTSR |= 0x8;

  NVIC_EnableIRQ(EXTI2_3_IRQn);

  __enable_irq();

  while(1){}
}



void EXTI2_3_IRQHandler(void)
{
    //turns red led on
    GPIOA->ODR |= 0x2; 

    delayms(500);   //0.5s delay

    /* turns red led off */
    GPIOA->ODR &= ~(0x2); 

    delayms(500);   //0.5s delay

    EXTI->PR |= 0x8;
}


void delayms(int delay)
{
    int i;
  
  for(; delay > 0; delay--)
   for(i = 0; i < 100; i++);
}


/*************************** End of file ****************************/
