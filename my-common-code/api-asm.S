/* 
 * Example of a pure assembly file
 * For most people, this is normally unnecessary, you can use inline
 * ASM in a C file, if you even need to use ASM at all.
 * However, if you reallllly like doing this sort of thing, here's how :)
 */

.syntax unified
.thumb
.section .text

#include <libopencm3/stm32/gpio.h>

.global rev_bytes
.func rev_bytes  /* only used for debugger */
.type rev_bytes, STT_FUNC
rev_bytes:
    rev r0, r0
    bx lr
