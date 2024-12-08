/************************************Includes***************************************/

#include "../beagle_uart.h"

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
// Constants for UART1 using PC4 and PC5
#define delay_0_1_s     1600000
#define UART_CLOCKen_BASE 0x400FE000
#define UART_CLOCKen_OFFSET 0x618
#define RCGCGPIO 0x400FE000
#define PORT_C_BASE 0x40006000
#define PORT_C_GPIOAFSEL 0x420

#define PORT_F 0x40025000

/*************************************Defines***************************************/

/********************************Public Functions***********************************/

// UART_Init
// Initializes UART serial communication with PC
// Return: void
void BEAGLE_UART_Init() {
    uint32_t porta_clock_mask = (0b1 << 2); // Mask for Port C (not A or B)
    uint32_t* gpio_clock = (uint32_t*)(RCGCGPIO + 0x608);
    *(gpio_clock) |= porta_clock_mask;

    // Enable UART1 module
    uint32_t* RCGCUART_reg = (uint32_t*)(UART_CLOCKen_BASE + UART_CLOCKen_OFFSET);
    *(RCGCUART_reg) |= (0b1 << 1); // Enable clock for UART1

    // Configure UART1 pins on Port C (PC4 and PC5)
    GPIOPinConfigure(GPIO_PC4_U1RX);  // UART1 RX Pin
    GPIOPinConfigure(GPIO_PC5_U1TX);  // UART1 TX Pin
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);  // Set PC4 and PC5 for UART1

    // Configure UART1 settings (baud rate, data bits, stop bits, parity)
    /*UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));*/
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));


    // Enable the UART1 interrupt in the UART module
   UARTIntEnable(UART1_BASE, UART_INT_RX);  // Enable RX interrupt and RX timeout interrupt

   // Enable interrupts in the NVIC (Nested Vector Interrupt Controller)
   IntEnable(INT_UART1);  // Enable UART1 interrupt in NVIC

    // Initialize UART for standard I/O with the same baud rate
    UARTStdioConfig(1, 115200, SysCtlClockGet());
}

/********************************Public Functions***********************************/
