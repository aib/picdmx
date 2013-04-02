#include <xc.h>
#include <stdint.h>

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

void dmx_setup(void);
void dmx_isr(void);
int8_t DMXaddrOnBus;

int8_t DMXaddr;
int8_t DMXval;

void main(void)
{
	DMXaddr = 3;
	dmx_setup();
	INTCONbits.GIE = 1;

	while (1) {
//		PORTD ^= 0x20;
		PORTDbits.RD5 = DMXval;
	}
}

void interrupt isr()
{
	if (PIR1bits.RCIF) {
		dmx_isr();
	}	
	PORTD &= ~0x1f;
}

void dmx_isr(void)
{
	//Frame error
	if (RCSTAbits.FERR) {
		PORTDbits.RD1 = 1;

		//Frame sync
		if (RCREG == 0) {
			DMXaddrOnBus = -1;
			PORTDbits.RD2 = 1;
		}
		//Actual frame error
		else {
			PORTDbits.RD3 = 1;
		}
		return;
	}

	//Buffer overflow
	if (RCSTAbits.OERR) {
		PORTDbits.RD0 = 1;
		//Restart EUSART subsystem
		RCSTAbits.SPEN = 0;
		RCSTAbits.SPEN = 1;
		return;
	}

	//Byte received
	uint8_t val = RCREG;

	if (++DMXaddrOnBus == DMXaddr) {
		DMXval = val;
	} else {
		PORTDbits.RD4 = 1;
	}
}

void dmx_setup(void)
{
	BAUDCTLbits.BRG16 = 1;
	TXSTAbits.BRGH = 1;
	SPBRG = 19;
	SPBRGH = 0;

	RCSTAbits.SPEN = 1;

	PIE1bits.RCIE = 1;
	INTCONbits.PEIE = 1;

	RCSTAbits.CREN = 1;

	TRISD = 0;
	PORTD = 0;
}

