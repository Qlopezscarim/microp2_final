// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_OPT3001.h"

#include <stdint.h>
#include "../multimod_i2c.h"

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// OPT3001_Init
// Initializes OPT3001, configures it to continuous conversion mode.
// Return: void
void OPT3001_Init(void) {
    // Initialize I2C module
    I2C_Init(I2C_A_BASE);

    // Set the correct configuration byte for continuous conversions
    OPT3001_WriteRegister(OPT3001_CONFIG_ADDR, 0b1100111000010000); //defualt but in continous conversions
    return;
}

// OPT3001_WriteRegister
// Writes to a register in the OPT3001.
// Param uint8_t "addr": Register address of the OPT3001.
// Param uint16_t "data": 16-bit data to write to the register.
// Return: void
void OPT3001_WriteRegister(uint8_t addr, uint16_t data) {
    // Read the datasheet!
    // Complete this function
    uint8_t to_send[3];
    to_send[0] = addr;
    to_send[1] = data >> 8;
    to_send[2] = data;
    I2C_WriteMultiple(I2C_A_BASE,OPT3001_ADDR , to_send, 3);
    return;

}

// OPT3001_ReadRegister
// Reads from a register in the OPT3001.
// Param uint8_t "addr": Register address of the OPT3001.
// Return: uint16_t
uint16_t OPT3001_ReadRegister(uint8_t addr) {
    // Complete this function
    uint8_t data_got[2];
    I2C_WriteSingle(I2C_A_BASE,OPT3001_ADDR ,addr);
    I2C_ReadMultiple(I2C_A_BASE,OPT3001_ADDR,data_got,2);
    uint16_t to_return = data_got[0]<<8|data_got[1];
    return to_return;
}

// OPT3001_GetResult
// Gets conversion result, calculates byte result based on datasheet
// and configuration settings.
// Return: uint32_t
uint32_t OPT3001_GetResult(void) {
    // Check if data is ready first


    uint16_t result = OPT3001_ReadRegister(OPT3001_RESULT_ADDR);

    result = LUX((result >> 12 & 0xF), (result & 0x0FFF));

    return result;
}

// OPT3001_SetLowLimit
// Sets the low limit register.
// Param uint16_t "exp": Exponential bound
// Param uint16_t "result": Conversion bound
// Return: void
void OPT3001_SetLowLimit(uint16_t exp, uint16_t result) {
    OPT3001_WriteRegister(OPT3001_LOWLIMIT_ADDR, (exp << OPT3001_RESULT_E_S | (result & 0xFFF)));

    return;
}

// OPT3001_SetHighLimit
// Sets the high limit register.
// Param uint16_t "exp": Exponential bound
// Param uint16_t "result": Conversion bound
// Return: void
void OPT3001_SetHighLimit(uint16_t exp, uint16_t result) {
    OPT3001_WriteRegister(OPT3001_HIGHLIMIT_ADDR, (exp << OPT3001_RESULT_E_S | (result & 0xFFF)));

    return;
}

// OPT3001_GetChipID
// Gets the chip ID of the OPT3001.
// Return: uint16_t
uint16_t OPT3001_GetChipID(void) {
    return OPT3001_ReadRegister(OPT3001_DEVICEID_ADDR);
}

/********************************Public Functions***********************************/
