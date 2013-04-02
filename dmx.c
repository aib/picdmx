#include <xc.h>
#include "dmx.h"

int8_t DMXaddr;
int8_t DMXval;

void dmx_isr_impl(void);
int8_t DMXaddrOnBus;

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

void dmx_isr(void)
{
	if (PIR1bits.RCIF) {
		dmx_isr_impl();
	}	
}

void dmx_isr_impl(void)
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
