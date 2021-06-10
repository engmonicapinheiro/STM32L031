#include "serial.h"
#include "stm32l0xx.h"

/*
*  PA2 -> USART2TX (from Nucleo Board)
*  PA15 -> USART2RX (from Nucleo Board)
*  PA1 -> green LED for debugging
*/
void USART2_Init()
{
  /* enable clock for APB1 bus*/
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    //enable bit 17
 

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
  USART2->BRR |= (160000 / 96);  //0x341;//(SystemCoreClock / 2400);      //9600 @16MHz   //from reference manual

    /* enable the USART module - bit 0 */
   USART2->CR1 |= USART_CR1_UE;

  /* to enable the transmitter - bit 3*/
  USART2->CR1 |= USART_CR1_TE; 

  USART2->CR1 |= USART_CR1_RE;

 // USART2->CR1 |= USART_CR1_RXNEIE;  //interrupt enable


  //enable interrupt in NVIC
 // NVIC_EnableIRQ(USART2_IRQn);

}

void USART_write(unsigned int ch)
{
  while(!(USART2->ISR & 0x0080));   //while(!(USART2->ISR & USART_ISR_TXE));
  USART2->TDR = (ch & 0xFF);
}


char USART2_read(void)
{
//if the buffer is not 1, wait. wait until the character arrives
  while(!(USART2->ISR &USART_ISR_RXNE)){}   //checks the receive buffer

  return USART2->RDR;
}


//struct __FILE{int handle;};

//FILE __stdout = {1};

//int fputc(int c, FILE *f)
//{
 // return USART_write(c);
//}