#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "synth.h"

#define CLK_ON  TCCR1B |= (1 << CS11)
#define CLK_OFF TCCR1B &= ~(1 << CS11)

void init(void);

int main(void)
{
  
  // init i/o, timer clocks
  init();

  //CLK_ON;

  while(1) {
    cli();
    unsigned int var = (PIND << 7) | (PINB & 0x07) | ((PINB & 0xE0) >> 2);
    //OCR1A = ~(var);

    var = ((~var) & 0x0FFF);
    
    if (var >= 65)
      CLK_ON;
    else
      CLK_OFF;

    var = var << 3;

    OCR1A = var;

    /*    while (var) {
      PORTA ^= (1 << PA1);
      var--;
      _delay_ms(1);
      }*/

    //_delay_ms(500);
    sei();

  }

  return 0;

}

void init(void)
{
  // toggle ACR1A/B pin on counter match
  TCCR1A |= (1 << COM1B0) | (1 << COM1A0);

  // set top of l/r counter to OCR1A register
  TCCR1B |= (1 << WGM12);

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
