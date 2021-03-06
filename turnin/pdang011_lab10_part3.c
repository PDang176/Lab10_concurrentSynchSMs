/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Video Link: https://drive.google.com/open?id=19rxCap_8Lg7lTCfnZlUAR82dweX5KMCv 
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char blinkingLED;
unsigned char threeLEDS;
unsigned char speaker;

enum BL_States{BL_SMStart, BL_OFF, BL_ON}BL_State;

void TickFct_BlinkLed(){
	//Transitions
	switch(BL_State){
		case BL_SMStart:
			BL_State = BL_ON;
			break;
		case BL_OFF:
			BL_State = BL_ON;
			break;
		case BL_ON:
			BL_State = BL_OFF;
			break;
		default:
			BL_State = BL_SMStart;
			break;
	}
	//State Actions
	switch(BL_State){
		case BL_SMStart:
			break;
		case BL_OFF:
			blinkingLED = 0x00;
			break;
		case BL_ON:
			blinkingLED = 0x08;
			break;
		default:
			break;
	}
}

enum TL_States{TL_SMStart, TL_LED0, TL_LED1, TL_LED2}TL_State;

void TickFct_ThreeLeds(){
	//Transitions
	switch(TL_State){
		case TL_SMStart:
			TL_State = TL_LED0;
			break;
		case TL_LED0:
			TL_State = TL_LED1;
			break;
		case TL_LED1:
			TL_State = TL_LED2;
			break;
		case TL_LED2:
			TL_State = TL_LED0;
			break;
		default:
			TL_State = TL_SMStart;
			break;
	}
	//State Actions
	switch(TL_State){
                case TL_SMStart:
			break;
                case TL_LED0:
			threeLEDS = 0x01;
			break;
                case TL_LED1:
			threeLEDS = 0x02;
			break;
                case TL_LED2:
			threeLEDS = 0x04;
			break;
                default:
                        break;
        }
}

enum PS_States{PS_SMStart, PS_OFF, PS_ON}PS_State;

void TickFct_PlaySound(){
	//Transitions
	switch(PS_State){
		case PS_SMStart:
			PS_State = PS_OFF;
			break;
		case PS_OFF:
			PS_State = (~PINA & 0x04) ? PS_ON : PS_OFF;
			break;
		case PS_ON:
			PS_State = (~PINA & 0x04) ? PS_ON : PS_OFF;
		default:
			PS_State = PS_SMStart;
			break;
	}
	//State Actions
	switch(PS_State){
                case PS_SMStart:
			break;
                case PS_OFF:
			speaker = 0x00;
			break;
                case PS_ON:
			speaker = ~speaker & 0x10;
			break;
                default:
			break;
	}
}

enum CL_States{Combine_LED}CL_State;

void TickFct_CombineLeds(){
	//Transitions
	switch(CL_State){
		case Combine_LED:
			CL_State = Combine_LED;
			break;
		default:
			CL_State = Combine_LED;
			break;
	}
	//State Actions
	switch(CL_State){
		case Combine_LED:
			PORTB = blinkingLED | threeLEDS | speaker;
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned long BL_elapsedTime = 0;
	unsigned long TL_elapsedTime = 0;
	const unsigned long timerPeriod = 2;

	TimerSet(timerPeriod);
	TimerOn();

	BL_State = BL_SMStart;
	TL_State = TL_SMStart;
	PS_State = PS_SMStart;
	CL_State = Combine_LED;
	blinkingLED = 0x00;
	threeLEDS = 0x00;
    /* Insert your solution below */
    while (1) {
	if(BL_elapsedTime >= 1000){
		TickFct_BlinkLed();
		BL_elapsedTime = 0;
	}
	if(TL_elapsedTime >= 300){
		TickFct_ThreeLeds();
		TL_elapsedTime = 0;
	}
	TickFct_PlaySound();
	TickFct_CombineLeds();
	while(!TimerFlag){}
	TimerFlag = 0;
	BL_elapsedTime += timerPeriod;
	TL_elapsedTime += timerPeriod;
    }
    return 1;
}
