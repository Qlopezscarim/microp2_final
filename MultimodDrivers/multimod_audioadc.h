#ifndef AUDIOADC
#define AUDIOADC

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/adc.h>
#include <driverlib/interrupt.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <inc/hw_gpio.h>
#include <driverlib/pwm.h>

void audio_in_init(void);
void read_audio_in(uint32_t* results);

#endif
