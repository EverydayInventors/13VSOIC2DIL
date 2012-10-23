/*

  ATtiny13V 8SOICto8DIL Board Larson Scanner Test Program
  Programmed by David Dahl, Everyday Inventors, LLC.
  Copyright 2011, 2012 Free Software Foundation

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
*/

/*

  This program does a simple software PWM of the LED briteness using timer 0
  overflow and an ISR.

  Code expects 5 LEDs to be connected to bits 1 to 5 of port B each with a 
  1k ohm resistor to ground.

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include <stdint.h>

#define FADE_RATE 20  /* iterations of ISR needed to fade LED values by 1 */
#define LED_DELAY 26  /* delay in ms between updating LED values */
#define MAX_LEDS 5

/*[ Global Variables ]-------------------------------------------------------*/

/* intensity of LEDs (off {0..255} fully on) read by ISR */
volatile uint8_t led_value[ MAX_LEDS ];

/* port bits for each LED */
volatile uint8_t led_bit[ MAX_LEDS ];

/*[ Main Program ]===========================================================*/
int main()
{
  int led_index;
  
  /*-- disable watchdog --*/
  MCUSR &= ~_BV( WDRF );
  wdt_disable();

  /*-- intialize variables --*/
  for( led_index = 0; led_index < MAX_LEDS ; led_index++ ) {
	led_value[ led_index ] = 0;
    led_bit[ led_index ] = _BV( led_index );
  }

  /*-- set clock prescaler div factor to 1 --*/
  CLKPR = 0b10000000;   /* enable prescaler change */
  CLKPR = 0b00000000;   /* change prescale div factor to 1 */

  /*-- Set data direction of port B pins with LEDs as output --*/
  DDRB = 0;
  for( led_index = 0; led_index < MAX_LEDS; led_index++ )
    DDRB |= led_bit[ led_index ];
  
  /*-- set timer 0 normal mode --*/
  TCCR0A &= ~( _BV(WGM01) | _BV(WGM00) );
  TCCR0B &= ~_BV( WGM02 );
  
  /* no prescale */
  TCCR0B = ( TCCR0B & ~(_BV(CS01) | _BV(CS02)) ) | _BV( CS00 );
  
  /* enable timer 0 overflow interrupt */
  TIMSK0 |= _BV( TOIE0 );
  
  /*-- enable interrupts --*/
  sei();
  
  /*-- main loop --*/
  led_index = 0;
  while(1) {
    for( led_index = 0; led_index < MAX_LEDS; led_index++ ) {
      led_value[ led_index ] = 0xFF;
      _delay_ms( LED_DELAY );
      led_value[ led_index ] = 0xFE;
    }

    for( led_index = MAX_LEDS - 2; led_index > 0; led_index-- ) {
      led_value[ led_index ] = 0xFF;
	  _delay_ms( LED_DELAY );
      led_value[ led_index ] = 0xFE;
    }
  }
  
  return 0;
}

/*[ ISR for Timer ]==========================================================*/
ISR( TIM0_OVF_vect )
{
  static uint8_t led_count = 0;
  static uint8_t index = 0;
  static int fadecount = 0;

  /* fade leds */
  if( fadecount++ > FADE_RATE ) {
    for( index = 0; index < MAX_LEDS; index++ )
      if( led_value[ index ] > 0 && led_value[ index ] < 0xFF )
        led_value[ index ]--;
    fadecount = 0;
  }

  /* software PWM of LEDs */
  for( index = 0; index < MAX_LEDS; index++ )    
    if( led_count < led_value[ index ] )
      PORTB |= led_bit[ index ]; /* turn LED on */
    else
      PORTB &= ~led_bit[ index ]; /* turn LED off */

  led_count++;
}

