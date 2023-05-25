/*
 * mTimer.h
 * Date   : 11/9/2022
 * Author : David Luby
 * Device : ATmega 2560
*/

/*
	The following script is intended for use as a header file with an ATmega
	2560 microcontroller. The function is intended to delay script advancement
	while some process settles, i.e., debouncing. It accepts an input in
	seconds corresponding to the amount of time it will delay and returns
	nothing
	
	The function requires the avr/io.h and avr/interrupt.h headers
*/


void mTimer(int count){
	
	//	prescaling to 8 MHz
	CLKPR = 0x80;// to enable prescale bits using CLKPR register P48
	CLKPR = 0x01;//system frequency prescale 16000000/2=8000000 P48
	
	
	// initializing loop counter
	int i;
	i = 0;
	
	/*
		Function uses 16-bit Timer/Counter Control 1 Register B (TCCR1B).
		TCCR1B bit values control timer parameters (waveform generation mode
		and prescaler). Page 133.
		 
		Setting waveform generation mode to Clear Timer on Compare Match (CTC,
		WGM12, 0100). WGM is spread over two registers. Page 145.
		
		Setting the prescaler to 8 mHz (CS11, 010). CS means clock select.
		Page 157.
	*/
	TCCR1B |= _BV(WGM12);
	TCCR1B |= _BV(CS11);
	
	// initizlizng Timer/Counter equal to zero
	TCNT1 = 0x0000;
	
	// initializing Output Compare Register equal to 1000 cycles
	OCR1A |= 0x03E8;
	
	/*
		initializing Timer Interrupt Mask Register as Output Compare Match
		Interrupt Enable. Page 162.
	*/
	//TIMSK1 = TIMSK1 | 0b00000010;// DISABLE WHEN OTHER INTERRUPTS USED
	
	/*
		clearing Timer Interrupt Flag Register by writing 1 to OCF1A bit. Page
		163.
	*/
	TIFR1 |= _BV(OCF1A);
	
	
	
	// enter loop
	while (i < count){
		if ((TIFR1 & 0x02) == 0x02){
			TIFR1 |= _BV(OCF1A);
			i++;
		}
	}
	return;
}


void rampTimer(int count){
	
	//	prescaling to 8 MHz
	CLKPR = 0x80;
	CLKPR = 0x01;
	
	int i;
	i = 0;

	TCCR3B |= _BV(WGM32);
	TCCR3B |= _BV(CS32);
	
	TCNT3 = 0x0000;
	
	OCR3A |= 0x03E8;

	TIFR3 |= _BV(OCF3A);
	
	while (i < count){
		if ((TIFR3 & 0x02) == 0x02){
			TIFR3 |= _BV(OCF3A);
			i++;
		}
	}
	return;
}