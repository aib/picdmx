#ifndef DMX_H__
#define DMX_H__

#include <stdint.h>

void dmx_setup(void);
void dmx_isr(void);

extern int8_t dmxAddr;
extern uint8_t dmxVal;

#endif
