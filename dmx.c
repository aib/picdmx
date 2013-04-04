#include <xc.h>
#include "dmx.h"

uint16_t dmxAddrBase;
uint8_t dmxVal[DMX_CHANNELS];

void dmx_isr_impl(void);
int16_t dmxAddrOnBus = -1;

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
		//Frame sync
		if (RCREG == 0) {
			dmxAddrOnBus = 0;
		}
		//Actual frame error
		else {
			//TODO: handle error
		}
		return;
	}

	//Buffer overflow
	if (RCSTAbits.OERR) {
		//TODO: handle error
		//Restart EUSART subsystem
		RCSTAbits.SPEN = 0;
		RCSTAbits.SPEN = 1;
		return;
	}

	//Byte received
	uint8_t val = RCREG;

	if (dmxAddrOnBus >= 0) {
		if (dmxAddrOnBus == 0) {
			if (val != 0) { //DMX type not 0
				dmxAddrOnBus = -1;
			}
		} else {
			uint16_t channel = dmxAddrOnBus - dmxAddrBase;
			if (channel < DMX_CHANNELS) {
				dmxVal[channel] = val;
			}
		}
		++dmxAddrOnBus;
	}
}
