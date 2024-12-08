// multimod_spi.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for SPI functions

/************************************Includes***************************************/

#include "../dma_lcd.h"
#include "../multimod_ST7789.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_ssi.h>
#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ssi.h>
#include <inc/hw_gpio.h>
#include <inc/hw_udma.h>


#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <driverlib/pin_map.h>

#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/uart.h"


/********************************Public Functions***********************************/

// SPI_Init
// Initializes specified SPI module. By default the mode
// is set to communicate with the TFT display.
// Param uint32_t "mod": base address of module
// Return: void
void DMA_Init(uint8_t* pui8DMAControlTable, uint8_t* buffer_A, uint8_t* buffer_B) {

    ST7789_UartScreen();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UDMA))
    {
    }

    uDMAEnable();

    uDMAControlBaseSet(pui8DMAControlTable);

    uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_NONE |
                              UDMA_DST_INC_NONE | UDMA_ARB_8);


    /*uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC, (void*)(UART1_BASE + UART_O_DR) , buffer_A,
                           256);*/

    uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT,
                               UDMA_MODE_BASIC, (void*)(UART1_BASE + UART_O_DR) , (void*)(SPI_A_BASE + SSI_O_DR),
                               153600);

    /*uDMAChannelControlSet(UDMA_CHANNEL_SSI0TX | UDMA_PRI_SELECT,
                                  UDMA_SIZE_8 | UDMA_SRC_INC_NONE |
                                  UDMA_DST_INC_8 | UDMA_ARB_8);*/
    //next step is to channel enable this guy and allat

    uDMAChannelEnable(UDMA_CHANNEL_UART1RX);
    UARTDMAEnable(UART1_BASE,UART_DMA_RX);
    //uDMAChannelRequest(UDMA_CHANNEL_UART1RX);


}


