// ButtonPressWithLag.c

// Note: This program does NOT work acceptably, the purpose is to show why interrrupts are necessary, see the other two programs for further details

// switch on PD2 (pin 4)
// LED on PC4 (pin 27), cycles via delay in main()
// LED on PC5 (pin 28), responds to button press (sort of)

#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1000000UL			// define it now as 1 GHz unsigned long
#endif

#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit))) 

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
	
	DDRD &= ~(1 << PD2);		// clear DDRD bit 2, sets PD2 (pin 4) for input
	PORTD |= (1 << PD2);		// set PD2/INT0 (pin 4) internal pull-up resistor
	
	DDRC |= (1 << PC5) | (1 << PC4);			// set PC5 (pin 28) and PC4 (pin 27) for output
	
	while (1) {					// begin infinite loop
		
		PORTC ^= (1 << PC4);	// toggle LED on PC5
		_delay_ms(1500);		// delay 1.5 sec
		
								// note that because of the delay on the previous line, the following button press exhibits extreme lag
								// this would not be acceptable in any production environment
		
		if(BIT_IS_CLEAR(PIND, PD2)) {			// if switch is pressed (logic low)
			PORTC |= (1 << PC5);				// turn on PC5 LED
		} else if(BIT_IS_SET(PIND, PD2)) {		// else if switch is not pressed (logic low)
			PORTC &= ~(1 << PC5);				// turn off PC5 LED
		} else {
			// ideally should never get here, but may occasionally due to timing
		}
	}
	return(0);					// should never get here, this is to prevent a compiler warning
}



