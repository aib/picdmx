#include <xc.h>

#include "dmx.h"

#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config DEBUG = OFF
//#pragma config FOSC = INTRC_CLKOUT
#pragma config FOSC = HS
#pragma config FCMEN = OFF
#pragma config MCLRE = OFF
#pragma config WDTE = OFF
#pragma config CP = OFF
#pragma config LVP = OFF
#pragma config PWRTE = OFF

void main(void)
{
	TRISD = 0;
	PORTD = 0;
	dmxAddrBase = 3;
	dmx_setup();

	OPTION_REGbits.T0CS = 0;
	OPTION_REGbits.PSA = 0;
	OPTION_REGbits.PS = 2;

	INTCONbits.GIE = 1;

	while (1) {
		if (TMR0 < dmxVal[0]) {
			PORTDbits.RD0 = 1;
		} else {
			PORTDbits.RD0 = 0;
		}
		if (TMR0 < dmxVal[1]) {
			PORTDbits.RD1 = 1;
		} else {
			PORTDbits.RD1 = 0;
		}
		if (TMR0 < dmxVal[2]) {
			PORTDbits.RD2 = 1;
		} else {
			PORTDbits.RD2 = 0;
		}
		if (TMR0 < dmxVal[3]) {
			PORTDbits.RD3 = 1;
		} else {
			PORTDbits.RD3 = 0;
		}
		if (TMR0 < dmxVal[4]) {
			PORTDbits.RD4 = 1;
		} else {
			PORTDbits.RD4 = 0;
		}
		if (TMR0 < dmxVal[5]) {
			PORTDbits.RD5 = 1;
		} else {
			PORTDbits.RD5 = 0;
		}
	}
}

void interrupt isr()
{
	dmx_isr();
}
