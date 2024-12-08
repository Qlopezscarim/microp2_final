// multimod_uart.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for UART functions

/************************************Includes***************************************/

#include "../multimod_uart.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>

//#include <driverlib/uartstdio.h>
#include "utils/uartstdio.h"
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

/************************************Includes***************************************/

/*************************************Defines***************************************/
#define delay_0_1_s     1600000
#define UART_CLOCKen_BASE 0x400FE000
#define UART_CLOCKen_OFFSET 0x618
#define RCGCGPIO 0x400FE000
#define PORT_A_BASE 0x40004000
#define PORT_A_GPIOAFSEL 0x420


#define PORT_F 0x40025000
/*************************************Defines***************************************/

/********************************Public Functions***********************************/

// UART_Init
// Initializes UART serial communication with PC
// Return: void
void UART_Init() {
    // This should have been done in lab 0, so it's just copy & paste.

    // Configure UART baud rate
    uint32_t porta_clock_mask = (0b1 << 0);
        uint32_t* gpio_clock = (uint32_t*)(RCGCGPIO + 0x608);
        *(gpio_clock) |= porta_clock_mask;

        // Enable UART0 module
        uint32_t* RCGCUART_reg = (uint32_t*)(UART_CLOCKen_BASE + UART_CLOCKen_OFFSET);
        *(RCGCUART_reg) = 0b1; //provide a clock in run mode to uart 0


        // Configure UART0 pins on port A
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

        UARTStdioConfig(0, 115200, SysCtlClockGet());

}

/********************************Public Functions***********************************/

