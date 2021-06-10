#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void configureRCCRegisters(void);
void configureMODERegistersforGPIOs(void);
void configureInterruptRegisters(void);
void applyInterrupts(void);
//to be replaced by a timer
void delayms(int delay);


#endif //INTERRUPTS_H