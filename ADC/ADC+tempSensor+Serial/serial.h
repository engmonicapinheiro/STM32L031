#ifndef SERIAL_H
#define SERIAL_H

void USART2_Init(void);
void USART_write(unsigned int ch);
char USART2_read(void);

//int fputc(int c, FILE *f);

#endif //SERIAL_H