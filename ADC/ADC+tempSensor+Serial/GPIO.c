#include "GPIO.h"
#include "stm32l0xx.h"


void LED_init()
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  GPIOA->MODER &= ~(10 << 0);  //for PA0, blue led - MODE0
  GPIOA->MODER |= (01 << 0);

  GPIOA->MODER &= ~(10 << 2); //for PA1, green led - MODE1
  GPIOA->MODER |= (01 << 2);
}