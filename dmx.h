#ifndef DMX_H__
#define DMX_H__

#include <stdint.h>

#define DMX_CHANNELS 8

void dmx_setup(void);
void dmx_isr(void);

extern uint16_t dmxAddrBase;
extern uint8_t dmxVal[DMX_CHANNELS];

#endif
