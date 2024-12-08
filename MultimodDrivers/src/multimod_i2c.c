// multimod_i2c.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for I2C functions

/************************************Includes***************************************/

#include "../multimod_i2c.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_i2c.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// I2C_Init
// Initializes specified I2C module
// Param uint32_t "mod": base address of module
// Return: void
void I2C_Init(uint32_t mod) {
    // Note:    The multimod board uses multiple I2C modules
//              to communicate with different devices. You can use
    //          the 'mod' parameter to choose which module to initialize
    //          and use.

    // Enable clock to relevant I2C and GPIO modules

    // Configure pins for I2C module

    // Configure I2C SCL speed, set as master

    if(mod == I2C_A_BASE)
    {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


    GPIOPinConfigure(0x00001803);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
    }


}

// I2C_WriteSingle
// Writes a single byte to an address.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t "byte": byte to send
// Return: void
void I2C_WriteSingle(uint32_t mod, uint8_t addr, uint8_t byte) {
    // Set the address in the slave address register

    // Input data into I2C module

    // Trigger I2C module send

    // Wait until I2C module is no longer busy
    if (mod == I2C_A_BASE)
    {
        I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); //initiating a writes to the slave
        I2CMasterDataPut(I2C1_BASE, byte);
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C1_BASE));
    }


    return;
}

// I2C_ReadSingle
// Reads a single byte from address.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Return: uint8_t
uint8_t I2C_ReadSingle(uint32_t mod, uint8_t addr) {
    uint8_t to_return;
    // Set the address in the slave address register

    // Trigger I2C module receive

    // Wait until I2C module is no longer busy

    // Return received data
    if (mod == I2C_A_BASE)
    {
        I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); //we now read
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C1_BASE));
        to_return = I2CMasterDataGet(I2C1_BASE);
    }
    return to_return;
}

// I2C_WriteMultiple
// Write multiple bytes to a device.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t* "data": pointer to an array of bytes
// Param uint8_t "num_bytes": number of bytes to transmit
// Return: void
void I2C_WriteMultiple(uint32_t mod, uint8_t addr, uint8_t* data, uint8_t num_bytes) {
    // Set the address in the slave address register
    if(mod == I2C_A_BASE)
    {
    I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); //we now read

    // Input data into I2C module
    I2CMasterDataPut(I2C1_BASE, *(data));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    for (int x = 1;x<(num_bytes-1);x++)
        {
            I2CMasterDataPut(I2C1_BASE, *(data+x));
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
            while(I2CMasterBusy(I2C1_BASE));
         }
                I2CMasterDataPut(I2C1_BASE, *(data+num_bytes-1));
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
                while(I2CMasterBusy(I2C1_BASE));
    }
    // Trigger I2C module send

    // Wait until I2C module is no longer busy

    // While num_bytes > 1
        // Input data into I2C module
        // Trigger I2C module send
        // Wait until I2C module is no longer busy

    // Input last byte into I2C module

    // Trigger I2C module send

    // Wait until I2C module is no longer busy


    return;
}

// I2C_ReadMultiple
// Read multiple bytes from a device.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t* "data": pointer to an array of bytes
// Param uint8_t "num_bytes": number of bytes to read
// Return: void
void I2C_ReadMultiple(uint32_t mod, uint8_t addr, uint8_t* data, uint8_t num_bytes) {
    // Set the address in the slave address register

    // Trigger I2C module receive
    if (mod == I2C_A_BASE)
            {
                I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); //we now read
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
                while(I2CMasterBusy(I2C1_BASE));
                *(data) = I2CMasterDataGet(I2C1_BASE);

                for (int x = 1;x<(num_bytes-1);x++)
                {
                    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
                    while(I2CMasterBusy(I2C1_BASE));
                    *(data+x) = I2CMasterDataGet(I2C1_BASE);
                }

                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
                while(I2CMasterBusy(I2C1_BASE));
                *(data+num_bytes-1) = I2CMasterDataGet(I2C1_BASE);

            }

    // Wait until I2C module is no longer busy

    // Read received data

    // While num_bytes > 1
        // Trigger I2C module receive
        // Wait until I2C module is no longer busy
        // Read received data

    // Trigger I2C module receive
    // Wait until I2C module is no longer busy
    // Read last byte


    return;
}

/********************************Public Functions***********************************/

