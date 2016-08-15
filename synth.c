#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "synth.h"

void init(void);

int main(void)
{
  
  // init i/o, timer clocks
  init();

  // ~400 Hz
  OCR1AH = 0x00;
  OCR1AL = 0x00;
  
  sei();

  while(1) {
  }

  return 0;

}

void init(void)
{
  // toggle ACR1A/B pin on counter match
  TCCR1A |= (1 << COM1B0) | (1 << COM1A0);

  // set top of l/r counter to OCR1A register
  TCCR1B |= (1 << WGM12);

  // set timer prescaler to clk/1024
  TCCR1B |= (1 << CS12);

  // set our left and right channels to outputs
  DDRA |= (1 << PA0) | (1 << PA1);

  // all of port d as inpus
  DDRD |= 0x00;
  // leave OCR1A/B as ouputs
  DDRB |= 0x18;

  // configure input pullups
  PORTD |= 0xFF;
  PORTB |= 0xE7;



}
