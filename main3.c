/*
 * File:   main.c
 * Author: local
 *
 * Created on 22 maja 2024, 12:38
 */

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

#define FCY         4000000UL
#define LCD_E       LATDbits.LATD4
#define LCD_RW      LATDbits.LATD5
#define LCD_RS      LATBbits.LATB15
#define LCD_DATA    LATE
 
#define LCD_CLEAR   0x01 //0b00000001
#define LCD_HOME    0x02 //0b00000010
#define LCD_ON      0x0C //0b00001100
#define LCD_OFF     0x08 //0b00001000
#define LCD_CONFIG  0x38 //0b00111000
#define LCD_CURSOR 0x80
#define LINE1 0x00
#define LINE2 0x40
#define LCD_CUST_CHAR 0x40
#define LCD_SHIFT_R 0x1D
#define LCD_SHIFT_L 0x1B

void __delay_us(unsigned long us){
    __delay32(us*FCY/1000000);
}
void __delay_ms(unsigned long ms){
    __delay32(ms*FCY/1000);
}

void LCD_sendCommand(unsigned char command){
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    __delay_us(50);
    LCD_E = 0;
}
void LCD_sendData(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_print(unsigned char* string){
    while(*string){
        LCD_sendData(*string++);
    }
}

void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    if (row == 1){
        address = LCD_CURSOR + LINE1 + col;
    }
    if (row == 2){
        address = LCD_CURSOR + LINE2 + col;
    }
    LCD_sendCommand(address);
}

void LCD_saveCustChar(unsigned char slot, unsigned char *array) {
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + (slot*8));
    for(i=0;i<8;i++){
        LCD_sendData(array[i]);
    }
}
// Wybór znaku do wy?wietlenia z tablicy
void LCD_displayCharacter(unsigned char characterCode) {
    LCD_sendData(characterCode);
}
// Funkcja inicjalizuj?ca wy?wietlacz LCD. Wysy?a niezb?dne komendy jak LCD_CONFIG i LCD_ON
void LCD_init(){
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50); // opó?nienia wynikaj? ze specyfikacji wy?wietlacza i czasu przetwarzania poszczególnych komend
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_CLEAR);
    __delay_ms(2);
}

const uint8_t customChars[][8] = {
    {0b00000, 0b01010, 0b01010, 0b00000, 0b10001, 0b01110, 0b00000}, 

#include <libpic30.h>
#include "xc.h"

void LCD_CreateCustomChars() {
    for (int i = 0; i < sizeof(customChars) / 8; i++) {
        LCD_CreateChar(i, customChars[i]);
    }
}

void LCD_AnimateAdvertisement() {
    const char *message = "Buy our product! :)";
    int len = strlen(message);
    char buffer[17];
    
    while (1) {
        for (int i = 0; i < len; i++) {
            strncpy(buffer, message + i, 16);
            buffer[16] = '\0';
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_Print(buffer);
            
            LCD_SetCursor(0, 1);
            LCD_WriteChar(0);
            
            __delay_ms(500);
        }
    }
}

int main(void) {
    LCD_Init();
    
    LCD_CreateCustomChars();
    
    LCD_AnimateAdvertisement();
    
    
    return 0;
}
