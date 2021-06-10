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


void LEDsconfiguration(void);
void configureSystick(void);
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void)
{
  LEDsconfiguration();
  configureSystick();

  while(1)
  {
    //check if count flag is set - CountFlag is at bit 16. so, 0x8000
    if(SysTick->CTRL & 0x8000)
    {
      GPIOA->ODR |= 0x1; //toggle led
      GPIOA->ODR &= ~(0x1); //toggle led
     
    }
  }
}

/*************************** End of file ****************************/
void LEDsconfiguration()
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  GPIOA->MODER &= ~(10 << 0);  //for PC14, blue led - MODE0
  GPIOA->MODER |= (01 << 0);
  GPIOA->MODER &= ~(10 << 2); //for PC15, green led - MODE1
  GPIOA->MODER |= (01 << 2);

}

//configure sysTick to generate 200ms delay
void configureSystick()
{
  SysTick->LOAD |= 16000000 - 1;    //16MHz
  SysTick->VAL |= 0x2;  //load the smallest value to this register, which is 1
  SysTick->CTRL |= 0x7;
 // SysTick->CTRL |= 0x8000;

}