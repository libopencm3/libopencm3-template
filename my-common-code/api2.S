/*

  Simple assembly demo

  To remove it, just remove api2.c, api2.h and all api2/my_func2
  remnants from my-project.c and Makefile.

*/

#include <libopencm3/stm32/gpio.h>

.syntax unified
.cpu cortex-m4
.thumb
.section .text

.global my_func2
.type my_func2,%function
my_func2:
.fnstart
  ldr r1, = GPIOA
  // store input to GPIOA
  str r0, [r1, # GPIO_ODR(0)]
  // load from GPIOA
  ldr r0, [r1, # GPIO_IDR(0)]

  // store bit 0 to bit 6 of GPIOB6
  ldr r1, = BBIO_PERIPH(GPIO_ODR(GPIOB), 6)
  str r0, [r1]

  bx lr
.fnend
