#include "Interrupts.h"
#include "stm32l0xx.h"


volatile int ButtonCounter;
volatile int Switch1Counter;
volatile int Switch2Counter;

void configureRCCRegisters()
{
  //RCC for GPIOA
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  //RCC for GPIOB
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  //RCC for GPIOC
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

  //Enable SYSCFG clock
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

}

void configureMODERegistersforGPIOs()
{
  /*resets PA1 moderA (MODE1) register (red led) */
  GPIOA->MODER &= ~(11 << 2); 
  /* resets PA2 register (blue led) */
  GPIOA->MODER &= ~(11 << 4); 
  /* resets PB5 register (green led) */
  GPIOB->MODER &= ~(11 << 10); 


  /* resets PA3 PUPDR register (input port to be used with pull-up) - button */
  GPIOA->MODER &= ~(11 << 6);
  GPIOA->PUPDR &= ~(01 << 6);

  /* resets PB4 PUPDR register (input port to be used with pull-up) - switch 1*/
  GPIOB->MODER &= ~(11 << 8); //MODE3, begins with bit 8
  GPIOB->PUPDR &= ~(01 << 8);

  /* resets PC14 PUPDR register (input port to be used with pull-up) - switch 2*/
  GPIOC->MODER &= ~(11 << 28);    //MODE14, begins with bit 28
  GPIOC->PUPDR &= ~(01 << 28);



  /* sets moderA register for PA1 (red led) */
  GPIOA->MODER |= (01 << 2); 
  /* sets moderA register for PA2 (blue led) */
  GPIOA->MODER |= (01 << 4); 
  /* sets PB5 register (green led) */
  GPIOB->MODER |= (01 << 10); 


  /* config PA3 as pull-up input (button) */
  GPIOA->MODER |= (00 << 6);  //MODE3, begins with bit 6
  GPIOA->PUPDR |= (01 << 6);

   /* sets PB4 PUPDR register (input port to be used with pull-up) - switch 1*/
  GPIOB->MODER |= (00 << 8);    //MODE3, begins with bit 8
  GPIOB->PUPDR |= (01 << 8);

  /* resets PC14 PUPDR register (input port to be used with pull-up) - switch 2*/
  GPIOC->MODER |= (00 << 28);    //MODE14, begins with bit 28
  GPIOC->PUPDR |= (01 << 28);

}


/*
* EXTI0_1_IRQn                = 5,      /*!< EXTI Line 0 and 1 Interrupts                            
* EXTI2_3_IRQn                = 6,      /*!< EXTI Line 2 and 3 Interrupts                            
* EXTI4_15_IRQn               = 7,      /*!< EXTI Line 4 to 15 Interrupts
*/
void configureInterruptRegisters()
{

  //EXTI1 for button at PA3
  //the register here is EXTICR1 at the brackets 
  SYSCFG->EXTICR[0] |= 0x0000; //or 0x0FFF //0000 is the sequence for PA; 0 at MSB position is 3

  //EXTI1 for switch1 at PB4
  SYSCFG->EXTICR[1] |= 0x0001; //0001 is the sequence for PB

  //EXTI4 for switch2 at PC14
  SYSCFG->EXTICR[3] |= 0x0200; //0010 is the sequence for PC

  //unmask EXTI1  - later: join these two lines into one
  EXTI->IMR |= 0x8;       //for PA3 
  EXTI->IMR |= 0x10;       //for PB4
  EXTI->IMR |= 0x4000;    //for PC14

  //select falling edge trigger - later: join these two lines into one
  EXTI->FTSR |= 0x8;      //for PA3   --> EXTI3 --> EXTI2_3_IRQn
  EXTI->FTSR |= 0x10;      //for PB4  --> EXTI4 --> EXTI4_15IRQn
  EXTI->FTSR |= 0x4000;      //for PC14 --> EXTI14 --> EXTI4_15IRQn

  NVIC_EnableIRQ(EXTI2_3_IRQn);
  NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* because this is a callback function, we don't need to write a prototype for it */
void EXTI2_3_IRQHandler()
{

    ButtonCounter++;
    
      //turns red led on (PA1)
    GPIOA->ODR |= 0x02; 

    delayms(1000);   //0.5s delay

    /* turns red led off */
    GPIOA->ODR &= ~(0x02); 

    delayms(1000);   //0.5s delay
  
    //clear interrupt pending flag
    EXTI->PR |= 0x08;


}


void EXTI4_15_IRQHandler()
{

    if(EXTI->PR == 0x10)
    {

    Switch1Counter++;

    //turns blue led on (PA2)
    GPIOA->ODR |= 0x4; 

    delayms(4000);   //0.5s delay

    /* turns blue led off */
    GPIOA->ODR &= ~(0x4); 

    delayms(4000);   //0.5s delay

    EXTI->PR |= 0x10;
  
    
    }
    else if(EXTI->PR == 0x4000)
    {
      Switch2Counter++;
      
      //turns green led on
      GPIOB->ODR |= 0x20; 

      delayms(1000);   //0.5s delay

      /* turns green led off */
      GPIOB->ODR &= ~(0x20); 

      delayms(1000);   //0.5s delay

      EXTI->PR |= 0x4000;
   
    }

}


void applyInterrupts()
{
     __disable_irq();
   configureRCCRegisters();
   configureMODERegistersforGPIOs();
   configureInterruptRegisters();
   __enable_irq();
}


void delayms(int delay)
{
    int i;
  
  for(; delay > 0; delay--)
   for(i = 0; i < 100; i++);
}