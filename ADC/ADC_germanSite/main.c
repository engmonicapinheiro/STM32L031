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


uint32_t read_analog(uint32_t channel);
void init_analog(void);

uint32_t analogRead;
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main()
{
  init_analog();
  while(1)
  {
     analogRead = read_analog(1);
  }


}
/*************************** End of file ****************************/
void init_analog()
{
    // Enable clock for ADC
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADCEN);

    // Configure PA1 as analog input for ADC_IN1
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE1, GPIO_MODER_MODE1_0 + GPIO_MODER_MODE1_1);

    // Disable the ADC
    if (READ_BIT(ADC1->ISR, ADC_ISR_ADRDY))
    {
        SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);
    }
    if (READ_BIT(ADC1->CR, ADC_CR_ADEN))
    {
        SET_BIT(ADC1->CR, ADC_CR_ADDIS);
    }

    // Wait until ADC is disabled
    while (READ_BIT(ADC1->CR, ADC_CR_ADEN));

    // Enable ADC voltage regulator
    MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, 0);
    MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, ADC_CR_ADVREGEN);

    // Delay 1-2 ms
   // delay_ms(2);

    // Set low frequency mode (required if ADC clock is < 3,5MHz)
    SET_BIT(ADC->CCR,ADC_CCR_LFMEN);

    // ADC Clock = PCLK/2
    MODIFY_REG(ADC1->CFGR2, ADC_CFGR2_CKMODE, ADC_CFGR2_CKMODE_0);

    // Start calibration
    SET_BIT(ADC1->CR, ADC_CR_ADCAL);

    // Wait until the calibration is finished
    while (READ_BIT(ADC1->CR, ADC_CR_ADCAL));

    // Clear the ready flag
    SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);

    // Enable the ADC and wait until is ready
    SET_BIT(ADC1->CR, ADC_CR_ADEN);
    while (!READ_BIT(ADC1->ISR, ADC_ISR_ADRDY));

    // Select software start trigger
    MODIFY_REG(ADC1->CFGR1, ADC_CFGR1_EXTEN, 0);

    // Select single conversion mode
    CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);

    // Set sample time to 19.5 cycles
    MODIFY_REG(ADC1->SMPR, ADC_SMPR_SMP, ADC_SMPR_SMP_2);
}

// Read from an analog input of ADC1
uint32_t read_analog(uint32_t channel)
{   
    // Select the channel
    WRITE_REG(ADC1->CHSELR, 1UL<<channel);

    // Clear the finish flag
    CLEAR_BIT(ADC1->ISR, ADC_ISR_EOC);

    // Start a conversion
    SET_BIT(ADC1->CR, ADC_CR_ADSTART);

    // Wait until the conversion is finished
    while (!READ_BIT(ADC1->ISR, ADC_ISR_EOC));
    while (READ_BIT(ADC1->CR, ADC_CR_ADSTART));

    // Return the lower 12 bits of the result
    return ADC1->DR & 0b111111111111;
}