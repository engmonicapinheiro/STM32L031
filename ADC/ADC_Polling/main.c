



#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"

void clock_configuration(void);
void GPIO_configuration(void);
void ADC_configuration(void);

/*
*  PA4 is connected to a power supply.
*  Code just to exercise the ADC in bare metal.
*
*  
*  
*/

uint32_t analogValue;

void main(void) 
{
  clock_configuration();
  GPIO_configuration();
  ADC_configuration();


  while(1)
  {
    

    /* clear the finish flag */
    ADC1->ISR &= ~(ADC_ISR_EOC);

    /* start conversion */
    ADC1->CR |= ADC_CR_ADSTART;

    

    /* wait for the conversion to be completed by checking the status register */
    while(!(ADC1->ISR & ADC_ISR_EOC));
    while(ADC1->CR & ADC_CR_ADSTART);

    analogValue = ADC1->DR & 0b111111111111;   //DR is the data register

 
  
  }

}

/*************************** End of file ****************************/
void clock_configuration()
{
  /* enable clock for alternate GPIO */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN; 

  /* enable clock for the ADC - bit 9 at RCC_APB2ENR */
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;


}


void GPIO_configuration()
{
    /* PA0 - output for the blue LED */
  GPIOA->MODER &= ~(11 << 0);
  GPIOA->MODER |=  (01 << 0);

    /* PA1 - output for the green LED */
  GPIOA->MODER &= ~(11 << 2);
  GPIOA->MODER |=  (01 << 2);



}

void ADC_configuration()
{
  /* set PA4 as an analog pin */
  GPIOA->MODER &= ~(00 << 8);
  GPIOA->MODER |= (11 << 8);  //or 0x300;
 

  /* disable the ADC */
  if(ADC1->ISR & ADC_ISR_ADRDY)
  {
    ADC1->ISR |= ADC_ISR_ADRDY;
  }
  
  if(ADC1->CR & ADC_CR_ADEN)
  {
     ADC1->CR |= ADC_CR_ADDIS; 
  }
  
  
  /* wait until ADC is disabled */
  while(ADC1->CR & ADC_CR_ADEN);


  /* enable the ADC voltage regulator */
  //ADC1->CR &= ~(ADC_CR_ADVREGEN);
 // ADC1->CR |= ADC_CR_ADVREGEN;

  /* set low frequency mode */
  //SET_BIT(ADC->CCR,ADC_CCR_LFMEN);

    // ADC Clock = PCLK/2
    MODIFY_REG(ADC1->CFGR2, ADC_CFGR2_CKMODE, ADC_CFGR2_CKMODE_0);

      /* ADC should start at channel 4 */
  ADC1->CHSELR |= (1 << 4); 
  
     // Start calibration
    SET_BIT(ADC1->CR, ADC_CR_ADCAL);

    // Wait until the calibration is finished
    while (READ_BIT(ADC1->CR, ADC_CR_ADCAL));


  /* clear the ready flag */
  ADC1->ISR |= ADC_ISR_ADRDY;

  /* enable the ADC and wait until it is ready */
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY)); //and wait until it is ready

  /* select software start trigger */
 // MODIFY_REG(ADC1->CFGR1, ADC_CFGR1_EXTEN, 0);
  //ADC1->CFGR1 &= ~(ADC_CFGR1_EXTEN);  //extern = 0

    // Select single conversion mode
    CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);

    // Set sample time to 19.5 cycles
  //  MODIFY_REG(ADC1->SMPR, ADC_SMPR_SMP, ADC_SMPR_SMP_2);

}


