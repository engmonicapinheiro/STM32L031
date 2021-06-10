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


void LEDplay(int value);
char USART2_read(void);
void USART2_Init(void);
void delayMs(int delay);
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
void main(void)
{
  USART2_Init();

  char ch;

  while(1)
  {
    ch = USART2_read();
    LEDplay(ch);
  }

}

/*************************** End of file ****************************/
/*
*  PA2 -> USART2TX (from Nucleo Board)
*  PA15 -> USART2RX (from Nucleo Board)
*  PA1 -> green LED for debugging
*/
void USART2_Init()
{
  /* enable clock for APB1 bus*/
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    //enable bit 17

  /* enable clock for alternate GPIO */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;  

  /* PA1 - output for the LED */
  GPIOA->MODER &= ~(11 << 2);
  GPIOA->MODER |=  (01 << 2);

  /* PA2 (TX) shall use the alternate function AF4 (from table at datasheet) */
  GPIOA->AFR[0] |= 0x400; //AF4 is 0100 in AFSEL2 

  GPIOA->MODER &= ~(01 << 4);
  GPIOA->MODER |=  (10 << 4);   //(MODE2)
 

  /* PA15 (RX) shall use the alternate function AF4 (from table at datasheet) */
  GPIOA->AFR[1] |= 0x40000000; //AF4 is 0100 in AFSEL10

  GPIOA->MODER &= ~(01 << 30);
  GPIOA->MODER |=  (10 << 30);   //(MODE15)


  /* to set the baud rate 
  * 14400 -> 0x008B
  * 1200 -> (160000 / 96)
  */
  USART2->BRR |= 0x008B;   //(SystemCoreClock / 2400);      //9600 @16MHz   //from reference manual

    /* enable the USART module - bit 0 */
   USART2->CR1 |= USART_CR1_UE;

  /* to enable the transmitter - bit 3*/
  USART2->CR1 |= USART_CR1_TE; 

  /* to enable the receiver */
  USART2->CR1 |= USART_CR1_RE;

 // USART2->CR1 |= USART_CR1_RXNEIE;  //interrupt enable


  //enable interrupt in NVIC
 // NVIC_EnableIRQ(USART2_IRQn);

}

char USART2_read(void)
{
//if the buffer is not 1, wait. wait until the ccharacter arrives
  while(!(USART2->ISR &USART_ISR_RXNE)){}   //checks the receive buffer

  return USART2->RDR;
}

void USART_write(unsigned int ch)
{
  while(!(USART2->ISR & 0x0080));   //while(!(USART2->ISR & USART_ISR_TXE));
  USART2->TDR = (ch & 0xFF);
}


void delayMs(int delay)
{
  unsigned int i;

  for(; delay > 0; delay--)
  {
    for(i = 0; i < 3195; i++);
  }
}

void LEDplay(int value)
{
    value %= 16;

    for(; value > 0; value--)
    {    
       //LED on
       GPIOA->ODR |= 0x2;
       delayMs(10);

       GPIOA->ODR &= ~(0x2);
       delayMs(10);
    }

    delayMs(100);
}