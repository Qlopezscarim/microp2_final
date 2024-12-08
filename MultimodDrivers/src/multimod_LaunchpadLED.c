// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_LaunchpadLED.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>

#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

/************************************Includes***************************************/

#define RCGCGPIO 0x400FE000

/********************************Public Functions***********************************/

// LaunchpadButtons_Init
// Initializes the GPIO port & pins necessary for the button switches on the
// launchpad. Also configures it so that the LEDs are controlled via PWM signal.
// Initial default period of 400.
// Return: void
void LaunchpadLED_Init() {
    // Enable clock to port F
    //enable portF and pin 4 as input to read in switch value

    // Enable PWM module

    // Configure necessary pins as PWM

    // Configure necessary PWM generators in count down mode, no sync

    // Set generator periods

    // Set the default pulse width (duty cycles).

    // Enable the PWM generators

    // Enable PWM output

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));
        //SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

        //red
        GPIOPinConfigure(GPIO_PF1_M1PWM5);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
        //red

        //blue
        GPIOPinConfigure(GPIO_PF2_M1PWM6);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
        //blue

        //green
            GPIOPinConfigure(GPIO_PF3_M1PWM7);
            GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
        //green


        //red
        PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
        //red
        //blue
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
        //blue
        //green
        //PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
        //green

        // Set the PWM frequency
        uint32_t load = 400;  // Set defualt to 400

        //red
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, load);
        //red
        //blue
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, load);
        //blue
        //green
        //PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, load);
        //green

        //red
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);  // 50% duty cycle
        //red
        //blue
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);  // 50% duty cycle
        //blue
        //green
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);  // 50% duty cycle
        //green

        // Enable PWM output on PWM1, generator 2, output 5 (PF1)
        //red
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
        //red
        //blue
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
        //blue
        //green
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
        //green

        // Enable the PWM generator
        //red
        PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        //red
        //blue
        PWMGenEnable(PWM1_BASE, PWM_GEN_3);
        //blue
        //green
        //PWMGenEnable(PWM1_BASE, PWM_GEN_1);
        //green

        //PWMSyncTimeBase(PWM1_BASE, PWM_GEN_2_BIT | PWM_GEN_3_BIT | PWM_GEN_1_BIT);
}

// LaunchpadLED_PWMSetDuty
// Sets the duty cycle of the PWM generator associated with the LED.
// Return: void
void LaunchpadLED_PWMSetDuty(LED_Color_t LED, float duty) {
    uint32_t pulse_width = (duty * PWM_Per);


    // If pulse width < 1, set as 1
    if (pulse_width < 1) {
        pulse_width = 1;
    }

    // If pulse width > set period, cap it
    if (pulse_width > PWM_Per) {
        pulse_width = PWM_Per - 1;
    }

    //uint32_t pwmClock = SysCtlClockGet() / 64;
    //uint32_t load = pulse_width*(pwmClock / 400) - 1;

    // Depending on chosen LED(s), adjust corresponding duty cycle of the PWM output
    // Your code below.

    if(LED == RED)
    {
    //red
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, pulse_width);
    //red
    }
    else if(LED == BLUE)
    {
    //blue
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, pulse_width);
    //blue
    }
    else
    {
    //green
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, pulse_width);
    //green
    }

    return;
}

/********************************Public Functions***********************************/
