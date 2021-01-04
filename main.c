#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm8s003.h"
#include "uart.h"
#include "stm8_utility.h"

int putchar(int c) {
    uart_write(c);
    return 0;
}

void timer4_init() {
  TIM4_CR1 |= _BV(0);//CEN
  TIM4_IER |= _BV(0); //UIE
  TIM4_PSCR = 0b101;
  TIM4_ARR = 255;
}

int main () {
    CLK_CKDIVR = 0;//16mhz
    uart_init(9600);
    enable_interrupts();

    while(1) {

    }
}
