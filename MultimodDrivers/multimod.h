#ifndef MULTIMOD_H_
#define MULTIMOD_H_

#include "multimod_uart.h"
#include "multimod_i2c.h"
#include "multimod_BMI160.h"
#include "multimod_OPT3001.h"
#include "multimod_LaunchpadButtons.h"
#include "multimod_LaunchpadLED.h"
#include "multimod_buttons.h"
#include "multimod_joystick.h"
#include "multimod_ST7789.h"
#include "multimod_audioadc.h"
#include "beagle_uart.h"
#include "dma_lcd.h"

static void Multimod_Init() {
    ST7789_Init();
    BMI160_Init();
    OPT3001_Init();
    UART_Init();
    BEAGLE_UART_Init();
    //may need to delete these two
    //LaunchpadButtons_Init();
    //LaunchpadLED_Init();
    //may need to delete these two
    MultimodButtons_Init();
    JOYSTICK_Init();
    JOYSTICK_IntEnable();
}

#endif /* MULTIMOD_H_ */
