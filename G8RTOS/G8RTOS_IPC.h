// G8RTOS_IPC.h
// Date Created: 2023-07-26
// Date Updated: 2023-07-26
// Interprocess communication code for G8RTOS

#ifndef G8RTOS_IPC_H_
#define G8RTOS_IPC_H_

/************************************Includes***************************************/

#include <stdint.h>

#include "./G8RTOS_Semaphores.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/

#define FIFO_SIZE 32 //Really may need to increase this!
#define MAX_NUMBER_OF_FIFOS 1

/*************************************Defines***************************************/

/******************************Data Type Definitions********************************/
/******************************Data Type Definitions********************************/

/****************************Data Structure Definitions*****************************/
/****************************Data Structure Definitions*****************************/

/********************************Public Variables***********************************/
/********************************Public Variables***********************************/
typedef struct{
    uint8_t type;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t w;
    uint16_t h;
    uint16_t color;
} to_screen;

typedef struct G8RTOS_FIFO_t {
    uint8_t buffer[FIFO_SIZE];
    uint8_t *head;
    uint8_t *tail;
    uint32_t lost_data;
    semaphore_t currentSize;
    semaphore_t mutex;
} G8RTOS_FIFO_t;

/****************************Data Structure Definitions*****************************/

/***********************************Externs*****************************************/
/***********************************Externs*****************************************/

/********************************Public Variables***********************************/

//static G8RTOS_FIFO_t FIFOs[MAX_NUMBER_OF_FIFOS];
G8RTOS_FIFO_t FIFOs[MAX_NUMBER_OF_FIFOS];

/********************************Public Functions***********************************/

int32_t G8RTOS_InitFIFO(uint32_t FIFO_index);
int8_t G8RTOS_ReadFIFO(uint32_t FIFO_index);
int32_t G8RTOS_WriteFIFO(uint32_t FIFO_index, uint8_t data);

/********************************Public Functions***********************************/

#endif /* G8RTOS_IPC_H_ */

