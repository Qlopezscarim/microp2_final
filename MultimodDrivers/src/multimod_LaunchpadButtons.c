// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_LaunchpadButtons.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

/************************************Includes***************************************/
#define RCGCGPIO 0x400FE000
#define PORT_F 0x40025000

/********************************Public Functions***********************************/

// LaunchpadButtons_Init
// Initializes the GPIO port & pins necessary for the button switches on the
// launchpad.
// Return: void
void LaunchpadButtons_Init() {
    // Enable port F for switches
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Use SW1 & SW2, configure as inputs.
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;  // Unlock GPIOCR register
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;   // Commit the change for PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;           // Re-lock the GPIOCR register

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Pay attention to the pin connected to SW2!!! PF0 is a locked pin.

}

// LaunchpadButtons_ReadSW1
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW1() {
    return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
}

// LaunchpadButtons_ReadSW2
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW2() {
    return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
}



/********************************Public Functions***********************************/
