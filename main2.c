/*
 * File:   main.c
 * Author: local
 *
 * Created on May 15, 2024, 12:26 PM
 */

#include "adc.h"
#include "xc.h"
#include <libpic30.h>
#include <stdbool.h>
#include "buttons.h"

#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int ExtSwitch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled

int main(void) {
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    
    unsigned value = 0;
    
    int time = 0;
    while(1){
        value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        if (value == 0xFFFF)
            continue;
        unsigned char normalizedValue = value >> 2;

        if (normalizedValue >=127){
            if (time >=4){
                LATA = 255;
            }
            else {
                LATA = 1;
                __delay32(2500000);
                LATA = 0;
                __delay32(2500000);
                time++;
            }
        }
        else {
            LATA = 0;
            time = 0;
        }
        if (BUTTON_IsPressed(BUTTON_S3) == true)
            time = 0;
    }
    return 0;
}
