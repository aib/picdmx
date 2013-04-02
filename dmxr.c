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
	dmxAddr = 3;
	dmx_setup();
	INTCONbits.GIE = 1;

	while (1) {
//		PORTD ^= 0x20;
		PORTDbits.RD5 = dmxVal;
	}
}

void interrupt isr()
{
	PORTD &= ~0x1f;
	dmx_isr();
}
