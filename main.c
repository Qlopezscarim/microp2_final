#include "MultimodDrivers/multimod.h"


#include <driverlib/uartstdio.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/timer.h>
#include <inc/hw_ints.h>
#include <inc/hw_nvic.h>
#include <driverlib/pwm.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "G8RTOS/G8RTOS.h"
#include <math.h>

#pragma DATA_ALIGN(controlTable, 1024)
uint8_t controlTable[1024]; //NEED FOR DMA

uint8_t buffer_A[256];
uint8_t buffer_B[256];

// y-axis controls z or y
uint8_t joystick_y = 0;
uint8_t sw_1_pressed = 0;
uint8_t sw_2_pressed = 0;

semaphore_t sw_semaphore;
semaphore_t joystick_semaphore;
semaphore_t spi_semaphore;

uint8_t uart_index_g = 0;

bool on_ground = 1;
bool first_draw;

volatile int8_t x_val = 5;
volatile int8_t y_val = 5;

bool is_valid_move(int16_t i_value, int16_t j_value)
{
    bool to_return = 1;
    if(i_value <0 || i_value > 11)
    {
        to_return = 0;
    }
    if(j_value < 1 || j_value > 14)
    {
        to_return = 0;
    }
    return to_return;
}



bool is_valid_move_obs(int16_t i_value, int16_t j_value)
{
    bool to_return = 1;
    if(i_value <0 || i_value > 11)
    {
        to_return = 0;
    }
    if(j_value < 0 || j_value > 14)
    {
        to_return = 0;
    }
    return to_return;
}



//#pragma pack(1)
typedef struct{
    bool red;
    bool blue;
    bool green;
    bool boundary;
    bool blocked;
    bool bottom;
    bool character;
    bool to_update;
    bool obstacle;
    bool lose;
} blocks;
//#pragma pack() emergency savior thing

typedef struct{
    uint8_t type;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t w;
    uint16_t h;
    uint16_t color;
} to_screen;

to_screen to_send_s;

uint16_t get_color(blocks a_block)
{
    return (ST7789_RED*a_block.red | ST7789_GREEN*a_block.green | ST7789_BLUE*a_block.blue);
}

//#define X_MAX                       240
//#define Y_MAX                       320


void idle()
{
    while(1)
    {
        uint32_t status = UARTIntStatus(UART1_BASE, true); // Get interrupt status
        //UARTprintf("HELLO\n");
        if(joystick_y == 1)
        {
        //UARTprintf("joystick pressed: %d\n",joystick_y);
        joystick_y = 0;
        }
        if(sw_1_pressed == 1)
        {
        //UARTprintf("switch one pressed: %d\n",sw_1_pressed);
        sw_1_pressed = 0;
        }
        if(sw_2_pressed == 1)
        {
        //UARTprintf("switched two pressed : %d\n",sw_2_pressed);
        sw_2_pressed = 0;
        }
    }
}



// 0-23
// 2-29




void Read_JoystickPress() {
    // Initialize / declare any variables here

    while(1) {
        // Wait for a signal to read the joystick press
        G8RTOS_WaitSemaphore(&joystick_semaphore);

        // Sleep to debounce
        sleep(10);

        // Read the joystick switch status on the Multimod board.
        uint8_t joy_value = JOYSTICK_GetPress();

        // Toggle the joystick_y flag.
        if(joy_value)
        {
            joystick_y = 1;
        }

        // Clear the interrupt
        GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);

        // Re-enable the interrupt so it can occur again.
        GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_2);
    }
}

void Read_Buttons() {
    // Initialize / declare any variables here
    uint8_t button_values;

    while(1) {
        //UARTprintf("Thread %d running:\n", CurrentlyRunningThread->thread_id);
        // Wait for a signal to read the buttons on the Multimod board.
        G8RTOS_WaitSemaphore(&sw_semaphore);

        // Sleep to debounce
        sleep(10); //reccomended 10ms

        // Read the buttons status on the Multimod board.
        button_values = MultimodButtons_Get();

        // Process the buttons and determine what actions need to be performed.
        //UARTprintf("\n%d\n",button_values);
        if(!(button_values & (1<<1)))
        {
            sw_1_pressed = 1;
        }
        if(!(button_values & (1<<2)))
        {
            sw_2_pressed = 1;
        }


        // Clear the interrupt
        GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);


        // Re-enable the interrupt so it can occur again.
        GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);


    }
}

void Get_Joystick(void) {
    uint32_t joystick_values = JOYSTICK_GetXY();
    //we will interpret the joystick values in here too
    volatile uint16_t x;
    volatile uint32_t y;

    x = (joystick_values) >> 16;
    y = (joystick_values) << 16;
    y = y >> 16;

    x_val = x_val+80;
    y_val = y_val+80; //handling integer rounding (annoying)

    // If joystick axis within deadzone, set to 0. Otherwise normalize it.
    x_val = ((int16_t)x)/400;
    y_val = ((int16_t)y)/400;
    if(x_val > 9)
    {
        x_val = 9;
    }
    if(y_val > 9)
    {
        y_val = 9;
    }


}

void GPIOE_Handler() {
    // Disable interrupt
    GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_4);

    // Signal relevant semaphore
    G8RTOS_SignalSemaphore(&sw_semaphore);

}

void GPIOD_Handler() {
    // Disable interrupt
    GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_2);
    // Signal relevant semaphore
    G8RTOS_SignalSemaphore(&joystick_semaphore);

}

int handle_read(char received, int uart_index);
void handle_draw();


void beagle_data()
{
    uint32_t status = UARTIntStatus(UART1_BASE, true); // Get interrupt status
    UARTIntClear(UART1_BASE, status); // Clear the interrupt

    // Handle the received data
    if (status & UART_INT_RX) {
        while (UARTCharsAvail(UART1_BASE)) {
            char receivedChar = UARTCharGet(UART1_BASE);
            int last = handle_read(receivedChar,uart_index_g);
            //handle read sets last to 1 when finished reading a whole transmission.
            if(last == 0)//we need to keep reading
            {
                uart_index_g++;
                //UARTIntClear(UART1_BASE, status); // Clear the interrupt
            }
            else
            {
                uart_index_g = 0;
                handle_draw();
                UARTprintf("H%d%d",x_val,y_val);
                //x_val = 5;//resetting to base
                //y_val = 5;//resetting to base
                //should reset x_val y_val
            }
        }
    }
}


/*void test_uart_me()
{
    uint32_t status = UARTIntStatus(UART1_BASE, true); // Get interrupt status
    UARTIntClear(UART1_BASE, status);

    if (status & UART_INT_RX) {
            while (UARTCharsAvail(UART1_BASE)) {
                char receivedChar = UARTCharGet(UART1_BASE);
            }
    }
}*/


int handle_read(char received, int uart_index)
{
    if(uart_index == 0)
    {
        to_send_s.type = received;
    }
    else if(uart_index == 1)
    {
        to_send_s.x_start = received;
    }
    else if(uart_index == 2)
    {
        to_send_s.x_start = to_send_s.x_start + (((uint16_t)received)<<8);
    }
    else if(uart_index == 3)
    {
        to_send_s.y_start = received;
    }
    else if(uart_index == 4)
    {
        to_send_s.y_start = to_send_s.y_start + (((uint16_t)received)<<8);
    }
    else if(uart_index == 5)
    {
        to_send_s.w = received;
    }
    else if(uart_index == 6)
    {
        to_send_s.w = to_send_s.w + (((uint16_t)received)<<8);
    }
    else if(uart_index == 7)
    {
        to_send_s.h = received;
    }
    else if(uart_index == 8)
    {
        to_send_s.h = to_send_s.h + (((uint16_t)received)<<8);
    }
    else if(uart_index == 9)
    {
        to_send_s.color = received;
    }
    else if(uart_index == 10)
    {
        to_send_s.color = to_send_s.color + (((uint16_t)received)<<8);
        return 1;
    }
    return 0;
}

void handle_draw()
{
    if(to_send_s.type == 1)
    {
        ST7789_DrawRectangle(to_send_s.x_start,to_send_s.y_start,to_send_s.w,to_send_s.h,to_send_s.color);
        //end_me(to_send_s.x_start,to_send_s.y_start,to_send_s.w,to_send_s.h,to_send_s.color);
    }
    else if(to_send_s.type == 2)
    {
        ST7789_Fill(to_send_s.color);
    }
}


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    Multimod_Init();


    G8RTOS_InitSemaphore(&joystick_semaphore, 0);
    G8RTOS_InitSemaphore(&sw_semaphore, 0);
    G8RTOS_InitSemaphore(&spi_semaphore, 1);




    G8RTOS_Init();


    //G8RTOS_InitFIFO(0);



    G8RTOS_AddThread(idle,255,"idle_thread");
    //G8RTOS_AddThread(Read_JoystickPress,0,"joystick");
    //G8RTOS_AddThread(Read_Buttons,0,"buttons");
    //G8RTOS_AddThread(display_grid,1,"dispgr");
    //G8RTOS_AddThread(game_logic,1,"gamel");
    //G8RTOS_AddThread(check_conditions,1,"hi");



    //G8RTOS_Add_PeriodicEvent(check_conditions,300,2);
    G8RTOS_Add_PeriodicEvent(Get_Joystick,300,5);

    //G8RTOS_Add_PeriodicEvent(Gen_obs,300*9,1200*3);

    //G8RTOS_Add_APeriodicEvent(GPIOE_Handler,5,20);
    //G8RTOS_Add_APeriodicEvent(GPIOD_Handler,5,19);

    G8RTOS_Add_APeriodicEvent(beagle_data,5,22);
    //G8RTOS_Add_APeriodicEvent(test_uart_me,5,22);


    G8RTOS_Launch();


	return 0;
}