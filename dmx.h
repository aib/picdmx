#ifndef DMXR_H__
#define DMXR_H__

#include <stdint.h>

void dmx_setup(void);
void dmx_isr(void);

extern int8_t dmxAddr;
extern int8_t dmxVal;

#endif
