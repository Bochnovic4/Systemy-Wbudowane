/*
 * File:   main.c
 * Author: local
 *
 * Created on March 20, 2024, 12:03 PM
 */
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
#define BIT_VALUE(n, p) (n>>p)&1

char currentS6 = 0, prevS6 = 0, currentS7 = 0, prevS7 = 0;
unsigned program = 1;
unsigned resetFlag = 0;
int main(void) {
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while (1) {
        if (BUTTON_IsPressed(BUTTON_S3)) {
            program++;
            LATA = 0;
            if (program > 9) {
                program = 1;
            }
            resetFlag = 1;
            __delay32(1000000);
        } else if (BUTTON_IsPressed(BUTTON_S6)) {
            program--;
            LATA = 0;
            if (program < 1) {
                program = 9;
            }
            resetFlag = 1;
            __delay32(1000000);
        }
 
        switch (program) {
            case 1:
                program1();
                break;
            case 2:
                program2();
                break;
            case 3:
                program3();
                break;
            case 4:
                program4();
                break;
            case 5:
                program5();
                break;
            case 6:
                program6();
                break;
            case 7:
                program7();
                break;
            case 8:
                program8();
                break;
            case 9:
                program9();
                break;
        }
    }
    return 0;
}

void program1(){
    static unsigned i = 0;
    if (resetFlag){
        i = 0;
        resetFlag = 0;
    }
    LATA = i;
    __delay32(1000000);
    i++;
}

void program2(){
    static unsigned i = 255;
    if (resetFlag){
        i = 255;
        resetFlag = 0;
    }
    LATA = i;
    __delay32(1000000);
    i--;
}

int IntToGray(unsigned char input){
        return (input>>1)^input;
}

void program3(){
    static unsigned char i = 0;
    if (resetFlag){
        i = 0;
        resetFlag = 0;
    }
    LATA = IntToGray(i);
    __delay32(1000000);
    i++;
}

void program4(){
    static unsigned char i = 255;
    if (resetFlag){
        i = 255;
        resetFlag = 0;
    }
    LATA = IntToGray(i);
    __delay32(1000000);
    i--;
}

unsigned char IntToBcd(unsigned char bin)
{
    unsigned char bcd = 0;
    bcd = ( bin % 10 );
    bcd += ( bin / 10 ) << 4;
    return bcd;
}

void program5(){
    static unsigned char i = 0;
    if (resetFlag){
        i = 0;
        resetFlag = 0;
    }
    if (i > 99){
        i = 0;
    }
    LATA = IntToBcd(i);
    __delay32(1000000);
    i++;
}


void program6(){
    static unsigned char i = 99;
    if (resetFlag){
        i = 99;
        resetFlag = 0;
    }
    if (i < 0){
        i = 99;
    }
    LATA = IntToBcd(i);
    __delay32(1000000);
    i--;
}

void program7(){
    static unsigned char counter = 0;
    static int flag = 1;
    if (resetFlag){
        counter = 255;
        flag = 1;
        resetFlag = 0;
    }
    if (counter == 0) flag = 1;
    else if (counter == 5) flag = -1;
 
    counter += flag;
 
    LATA = 7 << counter;
    __delay32(1000000);
}

void program8(){
    static unsigned start = 0b00000001;
    static unsigned end = 0b10000000;
    static unsigned queue = 0b00000000;
    if (resetFlag){
        start = 0b00000001;
        end = 0b10000000;
        queue = 0b00000000;
        resetFlag = 0;
    }
    LATA = start + queue;
    __delay32(1000000);
    start = start << 1;

    if(start == end){
        end = end >> 1;
        queue = queue >> 1;
        queue += 0b10000000;
        LATA = queue;
        __delay32(1000000);

        start = 0b00000001;

        if(start + queue == 0b11111111){
            LATA = 0b11111111;
            __delay32(1000000);
            start = 0b00000001;
            end = 0b10000000;
            queue = 0b00000000;
        }   
    }
}

unsigned LF(unsigned n){
    return BIT_VALUE(n,0)^BIT_VALUE(n,1)^BIT_VALUE(n,4)^BIT_VALUE(n,5);
}

 void program9(){
    static unsigned int state = 1;
    if (resetFlag){
        state = 1;
        resetFlag = 0;
    }
    unsigned int x1 = LF(state);
    state = (state >> 1) | (x1 << 5);

    LATA = state;

    __delay32(1000000);
}