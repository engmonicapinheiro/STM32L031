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


void ConfigureGPIO();
void Delayms(int delay);

/*********************************************************************/

//LED connected to PB3
//port to be analysed with oscilloscope in PA3
//the bus is an AHB

void main(void)
{

  ConfigureGPIO();

  while(1)
  {
    GPIOA->ODR |= 0x08; //finally writes 1 to the pin
    GPIOB->ODR |= 0x08;

    Delayms(1000);

    GPIOA->ODR &= ~(0x08);
    GPIOB->ODR &= ~(0x08);

    Delayms(1000);
    
  }
}

/*************************** End of file ****************************/

void ConfigureGPIO()
{
    /* reset the RCC IO enable register before setting it */
  RCC->IOPENR &= ~(1 << 0);  //resets bit 0 for GPIOA
  RCC->IOPENR &= ~(1 << 1);  //resets bit 1 for GPIOB

  /* sets the RCC IO enable register */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

  GPIOA->MODER &= ~(11 << 6);  //resets moderA register
  GPIOB->MODER &= ~(11 << 6);  //resets moderB register

  GPIOA->MODER |= (01 << 6);  //sets moderA register
  GPIOB->MODER |= (01 << 6);  //sets moderB register
}

void Delayms(int delay)
{
  int i;
  
  for(; delay > 0; delay--)
   for(i = 0; i < 57; i++);
}