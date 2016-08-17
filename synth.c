#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "synth.h"

#define CLK_ON  TCCR1B |= (1 << CS11)
#define CLK_OFF TCCR1B &= ~(1 << CS11)

void init(void);
void read_keys();

volatile unsigned int keys;

int main(void)
{

  // loop var
  uint8_t p;
  uint8_t key_down = 0;
  // init i/o, timer clocks
  init();

  //CLK_ON;

  while(1) {
    cli();

    //    read_keys();

    /*for( p=0; p< NUM_KEYS; p++ ){

      if( keys & (1 << p) ){
	play_note(p);
	key_down = 1;
      }
      }*/
    


      
    if( ~PIND & (1 << PD0)){
      OCR1A = 3188;
      CLK_ON;
    }
    
    else if( ~PIND & (1 << PD2)){
      OCR1A = 2839;
      CLK_ON;
    }
    
    else if( ~PIND & (1 << PD3)){
      OCR1A = 2530;
      CLK_ON;
    }
    
    else if( ~PINB & (1 << PB0)) {
      OCR1A = 2387;
      CLK_ON;
    }
    
    else if( ~PINB & (1 << PB1)) {
      OCR1A = 2127;
      CLK_ON;
    }
    
    else if( ~PINB & (1 << PB2)) {
      OCR1A = 1895;
      CLK_ON;
    }
    
    else if( ~PINB & (1 << PB5)) {
      OCR1A = 1689;
      CLK_ON;
    }
    
    else if( ~PINB & (1 << PB6)) {
      OCR1A = 1593;
      CLK_ON;
    }
    
    else CLK_OFF;
	   
    
    

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

void read_keys()
{
  keys = (PIND << 7) | ((PINB & 0x07) << 4) | ((PINB & 0xE0) >> 5);
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
