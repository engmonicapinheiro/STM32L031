# 1 "C:\\Users\\monic\\Desktop\\VoltmeterWithSTM32\\STM32L031K6T6\\MultipleInterrupts\\STM32L0xx\\Source\\STM32L0xx_Startup.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "C:\\Users\\monic\\Desktop\\VoltmeterWithSTM32\\STM32L031K6T6\\MultipleInterrupts\\STM32L0xx\\Source\\STM32L0xx_Startup.s"
# 47 "C:\\Users\\monic\\Desktop\\VoltmeterWithSTM32\\STM32L031K6T6\\MultipleInterrupts\\STM32L0xx\\Source\\STM32L0xx_Startup.s"
  .syntax unified

  .global Reset_Handler
  .extern _vectors

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08





Reset_Handler:


  ldr r0, =__RAM_segment_end__
  ldr r1, =0x7
  bics r0, r1
  mov sp, r0




  ldr r0, =SystemInit
  blx r0
# 99 "C:\\Users\\monic\\Desktop\\VoltmeterWithSTM32\\STM32L031K6T6\\MultipleInterrupts\\STM32L0xx\\Source\\STM32L0xx_Startup.s"
  ldr r0, =VTOR_REG



  ldr r1, =_vectors

  str r1, [r0]



  b _start
