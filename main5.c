#include <libpic30.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "buttons.h"
#include "lcd.h"
#include "xc.h"
#define TOTAL_TIME (5)
#define FCY 4000000UL

volatile unsigned int seconds = TOTAL_TIME;

void delay_1ms(void) {
    __delay32(FCY / 1000);
}

int main(void) {
    int side = 0;
    char buffer[32];
    int isStarted = 0;
    LCD_Initialize();
    unsigned int white = seconds;
    unsigned int black = seconds;
    int winner = 0;
    unsigned int elapsed = 0; // Counter for elapsed time in ms

    while (1) {
        if (BUTTON_IsPressed(BUTTON_S3) == true) {
            side = 1; // Switch to white's turn
            isStarted = 1;
        } 
        else if (BUTTON_IsPressed(BUTTON_S4) == true) {
            side = 2; // Switch to black's turn
            isStarted = 1;
        } 
        else if (BUTTON_IsPressed(BUTTON_S6) == true) {
            break; // Exit the loop
        } 
        else if (BUTTON_IsPressed(BUTTON_S5) == true) {
            side = 0; // Pause both timers
            isStarted = 0;
        }

        if (elapsed >= 1000) { // Update every 1 second
            unsigned char whiteMin = white / 60;
            unsigned char whiteSec = white % 60;
            unsigned char blackMin = black / 60;
            unsigned char blackSec = black % 60;

            LCD_ClearScreen();
            sprintf(buffer, "White %02u:%02u\nBlack %02u:%02u", whiteMin, whiteSec, blackMin, blackSec);
            LCD_PutString(buffer, strlen(buffer));

            elapsed = 0; // Reset elapsed time counter

            if (isStarted) {
                if (side == 1) {
                    if (white == 0) {
                        winner = 2;
                        break;
                    }
                    white--;
                } else if (side == 2) {
                    if (black == 0) {
                        winner = 1;
                        break;
                    }
                    black--;
                }
            }
        }

        if (isStarted == 1) {
            delay_1ms();
            elapsed++;
        }
    }

    switch (winner) {
        case 1:
            LCD_ClearScreen();
            sprintf(buffer, "Flag\nWhite");
            LCD_PutString(buffer, strlen(buffer));
            break;
        case 2:
            LCD_ClearScreen();
            sprintf(buffer, "Flag\nBlack");
            LCD_PutString(buffer, strlen(buffer));
            break;
        default:
            LCD_ClearScreen();
            sprintf(buffer, "END");
            LCD_PutString(buffer, strlen(buffer));
            break;
    }
    delay_1ms(); // Delay to allow the final message to be seen
    return 0;
}
