#include "../multimod_audioadc.h"


#define FS 64000

void audio_in_init(void) {

    // Enable gpio port
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    uint32_t load = SysCtlClockGet()/FS;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, load);

    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, load);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, load/2);  // 50% duty cycle
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);


    PWMGenIntTrigEnable(PWM1_BASE,PWM_GEN_2,PWM_TR_CNT_ZERO);



    PWMGenEnable(PWM1_BASE, PWM_GEN_2);



    // Disable adc
    SysCtlPeripheralDisable(SYSCTL_PERIPH_ADC1);

    // Enable adc module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    // Set pins as ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_1);

    // Configure ADC sequences
    ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PWM2|ADC_TRIGGER_PWM_MOD1, 0);
    //ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH2| ADC_CTL_IE | ADC_CTL_END); // AIN2



    // Enable ADC sequence
    ADCSequenceEnable(ADC1_BASE, 1);

    ADCIntClear(ADC1_BASE, 1);



}

void read_audio_in(uint32_t* results)
{

    // Wait until conversion is complete
    //while(!ADCIntStatus(ADC1_BASE, 1, 0));

    // Clear ADC interrupt flag
    //ADCIntClear(ADC1_BASE, 1);

    // Read from x and y coordinates
    ADCSequenceDataGet(ADC1_BASE, 1, results);
}
