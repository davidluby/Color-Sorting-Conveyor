/*
 * DCMotor_and_IRSensor.c
 *
 * Created: 11/21/2022 10:33:48 AM
 * Author : Brendan/David
 * 
 * WIRING:
 *
 * FOR DC MOTOR:
 * Plug motor input into output from Pin 13. Adjust dutyCycle from 0 to 100 to change brightness of LED or speed of motor.
 * Motor Controller In1 to Pin13. Motor Controller Ground to ground. Motor Controller In2 to +5V.
 *
 * FOR IR SENSOR:
 * Blue to ground. Red to +5V. Black to Pin20.
 */
 
//Header Files 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "LinkedQueue.h"
#include "lqFun.h"
#include "color.h"
#include "stepper.h"
#include "timer.h"

//Define Variables for motor
volatile int counter;
volatile int dutyCycle = 50; //defines double precision variable for duty cycle

//variables for color sensor
volatile int ClrFr;
volatile int HE_flag = 1;

//Define Variables for stepper motor
void initializeStepper();
volatile int incomingClr;
volatile int CurStep;

// Setup the linked Queue
volatile link *head; /* The ptr to the head of the queue */
volatile link *tail; /* The ptr to the tail of the queue */
volatile link *newLink; /* A ptr to a link aggregate data type (struct) */
volatile link *rtnLink; /* same as the above */
volatile element eTest; /* A variable to hold the aggregate data type known as element */
volatile int idx = 0;
volatile int idxPlate = 0;
volatile int history[100]; //	array storing everything thru clr sensor, 1st value 0
volatile int plate[100]; //	array storing each element on plate


volatile int rampFlag = 0;


//Main Function
void main(){
	if (rampFlag == 0){
		//DC Motor PWM Code
		CLKPR = 0x80;
		CLKPR = 0x01;
	
	
	
		//STEPPER MOTOR CODE
		//Set Data Direction Registers for stepper motor
		DDRC = 0xFF; //set portC to output (LED's)
		DDRA = 0xff; //set all pins in portA to output
		DDRH = 0b00000000; //set all pins in port H to input. this is where Hall effect sensor is input (pin7)
		DDRB=0x00;
	
		//After initialization, stepper will be on red. history = 0
		history[idx] = 0;
	
	
	
		//IR Sensor Code-------------------------------------------------------------------------------------------------------------------------------------
		//Set data direction registers
		DDRC = 0b11111111; //port C to output
		DDRD = 0b00000000; //port D to input




		cli();// disable all of the interrupt	
		
		// config the external interrupt
		EIMSK |= (_BV(INT2)); // enable INT2 (hall effect)
		EICRA |= (_BV(ISC21)); // falling edge interrupt
	
		EIMSK |= (_BV(INT1)); // enable INT1 (ir/color sensor combo)
		EICRA |= (_BV(ISC11)); // falling edge interrupt
	
		EIMSK |= (_BV(INT0));	// ir/dc/stepper combo
		EICRA |= (_BV(ISC01));
	
		EIMSK |= (_BV(INT3)); // enable INT3 (pause function)
		EICRA |= (_BV(ISC31)); // falling edge interrupt
	
		EIMSK |= (_BV(INT4)); // enable INT4 (rampdown function)
		EICRA |= (_BV(ISC41)); // falling edge interrupt

		sei();// enable all of the interrupt



	
		initializeStepper();
		CurStep = 3;
	
	
		DDRB = (1 << PB7); //sets OC0A to output pin
		TCCR0A |= _BV(WGM01) | _BV(WGM00);// choose fast PWM mode P126, Mode 3 P128
	
								//*setting bit 1 of TIMSK1 to 1 will enable the timer interrupt to function when the OCF1A flag is set in TIFR1*
		TCCR0A |= _BV(COM0A1);
		TCCR0B |= _BV(CS01); //scales clock
		OCR0A = (dutyCycle/100.0)*255.0; //output compare register value that the pwm will reset at. duty cycle defines percentage of max value (255)
		//End DC Motor PWM Code
	
	
		rtnLink = NULL;
		newLink = NULL;
		setup(&head, &tail);

	
		while (1){
		}
	}	//	end of if statement
	else{
		rampTimer(150); // 350
		
		//	stop dc
		OCR0A = 0;

		int k;
		PORTC = 0b00110011;	//	flash before on plate
		mTimer(3000);
		for (k = 0; k < idxPlate; k++){
			PORTC = plate[k];
			mTimer(2000);
			PORTC = 0;
			mTimer(250);
		}
		DDRC = 0;
		DDRA = 0;
		DDRH = 0;
		DDRB = 0;
		cli();
	}
}// end main


//	hall effect sensor interrupt used to initialize plate at origin
ISR(INT2_vect){
	//	when hall effect sensor at origin, set flag to zero. end stepper init
	HE_flag = 0;
}

//	ir sensor interrupt for color sensor to read and add to linked queue
ISR(INT1_vect){ //Whenever interrupt is triggered, system will immediately jump to this
	// init new link
	
	// if first link condition
	if(isEmpty(&head))
	{setup(&head, &tail);}
	
	// else
	initLink(&newLink);
	enqueue(&head, &tail, &newLink);
	
	
	ClrFr = ClrDet(); // intake new color
	if (ClrFr == 0b10000000)	// red
	{
		incomingClr = 0;
	}
	else if (ClrFr == 0b01000000)	// yellow
	{
		incomingClr = 3;
		
	}
	else if (ClrFr == 0b00100000)	// green
	{
		incomingClr = 1;
	}
	else if (ClrFr == 0b00010000)	// blue
	{
		incomingClr = 2;
	}
	
	//	for debugging color sensor reads
	PORTC = incomingClr+1;

	newLink->e.itemCode = incomingClr; // new
	newLink->e.stage = history[idx]; // old

	idx++;
	history[idx] = incomingClr;	//	on conveyor array
}

//	ir sensor interrupt to stop/start dc so stepper matches color
ISR(INT0_vect){
	eTest = firstValue(&head);

	//	stop dc
	OCR0A = 0;

	//	Call Stepper Rotation Function
	CurStep = stepColor(eTest.itemCode,eTest.stage, CurStep);
	//	remove block pushed to plate and store new color in plate array
	plate[idxPlate] = eTest.itemCode;	//	plate array
	idxPlate++;
	dequeue(&head, &tail, &rtnLink);
	
	//	restart dc
	OCR0A = (dutyCycle/100.0)*255.0;
}

//	pause function interrupt
ISR(INT3_vect){
	//	stop dc
	OCR0A = 0;
	
	int j;
	PORTC = 0b11110000;	//	flash before on conveyor
	mTimer(3000);	
	for (j = idxPlate; j < idx; j++){
		PORTC = history[j]+1;
		mTimer(2000);
		PORTC = 0;
		mTimer(250);	
	}
	
	
	int k;
	PORTC = 0b00001111;	//	flash before on plate
	mTimer(3000);
	for (k = 0; k < idxPlate; k++){
		PORTC = plate[k]+1;
		mTimer(2000);
		PORTC = 0;
		mTimer(250);
	}
	
	//	restart dc
	OCR0A = (dutyCycle/100.0)*255.0;
}

//	rampdown function interrtupt
ISR(INT4_vect){
	
	rampFlag = 1;
	
	main();
}


//Stepper Initialization Function
void initializeStepper(){
	int i = 0;
	while(HE_flag){
		PORTA = 0b00101011; //step 1
		mTimer(11);
		PORTA = 0b00101101; //step 2
		mTimer(11);
		PORTA = 0b00110101; //step 3
		mTimer(11);
		PORTA = 0b00110011; //step 4
		mTimer(11);
		}
	return;
	//}
}//end Initialization Function
