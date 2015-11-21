// Any_Pin_Interrupt.c

// switch on PD2/PCINT18 (pin 4)
// LED on PC4 (pin 27), cycles via delay in main()
// LED on PC5 (pin 28), responds to button press (sort of)

#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1000000UL			// define it now as 1 MHz unsigned long
#endif

#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function
#include <avr/interrupt.h>

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
	
	DDRD &= ~(1 << PD2);		// clear DDRD bit 2, sets PD2 (pin 4) for input
	PORTD |= (1 << PD2);		// set PD2/INT0 (pin 4) internal pull-up resistor
	
	DDRC |= (1 << PC5) | (1 << PC4);			// set PC5 (pin 28) and PC4 (pin 27) for output
	
	/*
	PCICR - Pin Change Interrupt Control Register
	
	bit         7       6       5       4       3         2          1        0
	name        -       -       -       -       -       PCIE2      PCIE1    PCIE0
	set to      0       0       0       0       0         1          0        0
	
	bit 7 = 0
	bit 6 = 0
	bit 5 = 0
	bit 4 = 0
	bit 3 = 0
	PCIE2 = 1     enable pin change interrupt on pins PCINT23 through PCINT16
	PCIE1 = 0     don't enable pin change interrupt on pins PCINT14 through PCINT8
	PCIE0 = 0     don't enable pin change interrupt on pins PCINT7 through PCINT0
	*/
	PCICR = 0b00000100;
	
	/*
	PCMSK2 - Pin Change Mask Register 2
	
	bit           7           6           5           4          3            2            1           0
	name       PCINT23     PCINT22     PCINT21     PCINT20     PCINT19     PCINT18      PCINT17     PCINT16
	set to        0           0           0           0           0           1            0           0
	
	PCINT23 = 0   don't enable PCINT23 pin change interrupt
	PCINT22 = 0   don't enable PCINT22 pin change interrupt
	PCINT21 = 0   don't enable PCINT21 pin change interrupt
	PCINT20 = 0   don't enable PCINT20 pin change interrupt
	PCINT19 = 0   don't enable PCINT19 pin change interrupt
	PCINT18 = 1   enable PCINT18 pin change interrupt
	PCINT17 = 0   don't enable PCINT17 pin change interrupt
	PCINT16 = 0   don't enable PCINT16 pin change interrupt
	*/
	PCMSK2 = 0b00000100;
	
	sei();				// enable interrupts
	
	while (1) {					// begin infinite loop
		PORTC ^= (1 << PC4);	// toggle LED on PC4
		_delay_ms(1500);		// delay 1.5 sec
	}
	return(0);					// should never get here, this is to prevent a compiler warning
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ISR(PCINT2_vect) {
	if(BIT_IS_CLEAR(PIND, PD2)) {			// if switch is pressed (logic low)
		PORTC |= (1 << PC5);				// turn on PC5 LED
	} else if(BIT_IS_SET(PIND, PD2)) {		// else if switch is not pressed (logic low)
		PORTC &= ~(1 << PC5);				// turn off PC5 LED
	} else {
		// ideally should never get here, but may occasionally due to timing
	}
}



