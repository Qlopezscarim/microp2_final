// multimod_spi.h
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// spi header file

#ifndef DMA_LCD_H_
#define DMA_LCD_H_

/************************************Includes***************************************/

#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>

#include <driverlib/ssi.h>
#include <driverlib/udma.h>
#include <inc/hw_udma.h>

/************************************Includes***************************************/

/*************************************Defines***************************************/

#define SPI_A_BASE          SSI0_BASE
#define SPI_A_GPIO_BASE     GPIO_PORTA_BASE
#define SPI_A_PIN_MISO      GPIO_PIN_4
#define SPI_A_PIN_MOSI      GPIO_PIN_5
#define SPI_A_PIN_CLK       GPIO_PIN_2

/*************************************Defines***************************************/

/******************************Data Type Definitions********************************/
/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
/****************************Data Structure Definitions*****************************/

/***********************************Externs*****************************************/
/***********************************Externs*****************************************/

/********************************Public Variables***********************************/
/********************************Public Variables***********************************/

/********************************Public Functions***********************************/

void DMA_Init(uint8_t* pui8DMAControlTable, uint8_t* buffer_A, uint8_t* buffer_B);

/********************************Public Functions***********************************/

/*******************************Private Variables***********************************/
/*******************************Private Variables***********************************/

/*******************************Private Functions***********************************/
/*******************************Private Functions***********************************/

#endif /* MULTIMOD_SPI_H_ */


