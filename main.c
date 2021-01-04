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

//TIM1
//(CC1IF) je treba spucat v interruptu
//TI1FP1 je input pin
//CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIM1_CCER1).

//TIM2
//PD4 -> TIM2 CH1

volatile uint16_t delta = 0;
volatile uint16_t tmp = 0;

void input_capture() __interrupt(TIM1_CC_ISR) {
  uint8_t end = TIM1_SR1 & _BV(1);//CC1IF
  uint16_t ccr = TIM1_CCR1H << 8 | TIM1_CCR1L;

  //PD_ODR ^= _BV(3);

  if(PC_IDR & _BV(6)) {
    TIM1_CCER1 |= _BV(1);
    tmp = ccr;
    //PD_ODR |= _BV(3);
  }
  else {
    delta = ccr - tmp;
    TIM1_CCER1 &= ~_BV(1);
    //PD_ODR &= ~_BV(3);
  }
}

void timer1_init() {
  TIM1_IER |= _BV(1); //CC1IE
  TIM1_EGR |= _BV(1);//CC1G
  TIM1_CCMR1 |= _BV(0) | (0b1111<<4); //4x filter, TI1FP1
  TIM1_CCER1 |= _BV(0); //Enable capture
  //TIM1_PSCR |= 0xffff; //<- IDK nevem zakaj ne dela
  TIM1_PSCRL |= 15;


  TIM1_CR1 |= _BV(TIM1_CR1_CEN) | _BV(TIM1_CR1_ARPE);
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
    timer1_init();
    enable_interrupts();

    //PD_DDR |= _BV(3);
    //PD_CR1 |= _BV(3); //OUTPUT
    PC_CR1 |= _BV(6); //PULLUP

    while(1) {
      //util_delay_milliseconds(100);
      printf("%u\n\r", delta);
      //printf("Hello world!\n\r");
    }
}
