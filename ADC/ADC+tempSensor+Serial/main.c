#include <stdio.h>
#include <stdlib.h>

#include "stm32l0xx.h"
#include "timer.h"
#include "adc.h"
#include "serial.h"
#include "GPIO.h"


unsigned int TempValue;
unsigned int voltage;
unsigned int celsius;


void main(void) 
{
  LED_init();
  ADC_configuration();
  timer_init();
  USART2_Init();

  

  while(1)
  {
    TempValue = readADC();


    voltage = TempValue / 4095 * 3.3;

    celsius = (voltage - 0.76) / 0.0025 + 25;

      USART_write('>');

     
     

    //  USART_write(voltage);
    //  USART_write('\n');

   // printf("%d, %.2f\370C\r\n", TempValue, celsius);



 
  
  }
 
}


