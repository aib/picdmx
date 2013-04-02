#include <xc.h>
#include "dmx.h"

int8_t dmxAddr;
int8_t dmxVal;

void dmx_isr_impl(void);
int8_t dmxAddrOnBus;

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
	PORTD &= ~0x1f;

	//Frame error
	if (RCSTAbits.FERR) {
		PORTDbits.RD1 = 1;

		//Frame sync
		if (RCREG == 0) {
			dmxAddrOnBus = -1;
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

	if (++dmxAddrOnBus == dmxAddr) {
		dmxVal = val;
		PORTDbits.RD4 = 1;
	}
}
