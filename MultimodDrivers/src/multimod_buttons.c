// multimod_buttons.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for button functions

/************************************Includes***************************************/

#include "../multimod_buttons.h"

#include "../multimod_i2c.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_i2c.h>

/********************************Public Functions***********************************/

//void switch_interupt()
//{
    //GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
    //UARTprintf("This actually triggered lol!\n");
//}

// Buttons_Init
// Initializes buttons on the multimod by configuring the I2C module and
// relevant interrupt pin.
// Return: void
void MultimodButtons_Init() {
    // Initialize this function & the relevant interrupt pin
    I2C_Init(I2C_A_BASE);

    //GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Wait for the GPIO port to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}

    // Configure Pin 4 as input
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);

    // Configure pin 4 for falling edge interrupts
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);

    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);

    //GPIOIntRegister(GPIO_PORTE_BASE,switch_interupt);

    //IntEnable(INT_GPIOE);

}

// MultimodButtons_Get
// Gets the input to GPIO bank 1, [0..7].
// Return: uint8_t 
uint8_t MultimodButtons_Get() {
    // complete this function
    I2C_WriteSingle(I2C_A_BASE,BUTTONS_PCA9555_GPIO_ADDR,0x0);
    uint8_t button_read = I2C_ReadSingle(I2C_A_BASE,BUTTONS_PCA9555_GPIO_ADDR);
    return button_read;
}

