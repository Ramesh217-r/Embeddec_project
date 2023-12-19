/*
 * File:   keypad_calculator.c
 * Author: rames
 *
 * Created on 18 December, 2023, 10:40 AM
 */

#include<stdio.h>
#include <xc.h>
#define _XTAL_FREQ 20000000
char keypad();
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/*keypad config*/
#define R1 RB0
#define R2 RB1
#define R3 RB2
#define R4 RB3
#define C1 RB4
#define C2 RB5
#define C3 RB6
#define C4 RB7
/*LCD config*/
#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

void LCD_data(unsigned char data) {
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(10);
    EN = 0;
}

void LCD_Command(unsigned char cmd) {
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(10);
    EN = 0;
}

void intialize() {
    LCD_Command(0x38);
    LCD_Command(0x06);
    LCD_Command(0x0C);
    LCD_Command(0x01);
}
// Global variables to store the operands and result
unsigned int operand1 = 0;
unsigned int operand2 = 0;
unsigned int result = 0;
char operator = ' ';

// Function to perform addition

int addition( int a, int b) {
    return a + b;
}
int subtraction(int a, int b) {
    return a - b;
}
int multiplication(int a, int b) {
    return a * b;
}
int division(int a, int b) {
    return a / b;
}

void main(void) {

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    TRISC = 0x00;
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    char key;
    intialize();
    while (1) {
        key = keypad();
        
        // Check for numeric keys or '=' key
        if (key >= '0' && key <= '9') {
            // If operator is not set, input is considered as the first operand
            
            if (operator == ' ') {
                LCD_Command(0x80);
                LCD_data(key);
                operand1 = operand1 * 10 + (key - '0');
            } else {
                LCD_Command(0x82);
                LCD_data(key);
                operand2 = operand2 * 10 + (key - '0');
            }
        } else if (key == '+' || key == '-' || key == '*' || key == '/') {
            // Handle operator keys
            LCD_Command(0x81);
            LCD_data(key);
            operator = key;
        }else if (key == '=') 
        {
            LCD_Command(0x83);
            LCD_data(key);
            // Calculate result when '=' key is pressed
            switch (operator) {
                case '+':
                    result = addition(operand1, operand2);
                    
                    break;
                // Handle other operators similarly with respective functions
                 case '-':
                     result = subtraction(operand1, operand2);
                     break;
                 case '*':
                     result = multiplication(operand1, operand2);
                     break;
                 case '/':
                     result = division(operand1, operand2);
                     break;
            }
            LCD_Command(0xC0); // Set cursor to the second line
            // Convert integer result to string and display on LCD
            char result_str[10];
            sprintf(result_str,"%d", result);
            for (int i = 0; result_str[i] != '\0'; i++) {
                LCD_data(result_str[i]);
            }
            // Reset variables for next operation
            operand1 = 0;
            operand2 = 0;
            result = 0;
            operator = ' ';
        }
    }
    return;
}

char keypad() {
    while (1) {
        R1 = 1;
        R2 = R3 = R4 = 0;
        if (C1 == 1) {
            while (C1 == 1);
            __delay_ms(10);
            return '7';
        }
        R1 = 1;
        R2 = R3 = R4 = 0;
        if (C2 == 1) {
            while (C2 == 1);
            __delay_ms(10);
            return '8';
        }
        R1 = 1;
        R2 = R3 = R4 = 0;
        if (C3 == 1) {
            while (C3 == 1);
            __delay_ms(10);
            return '9';
        }
        R1 = 1;
        R2 = R3 = R4 = 0;
        if (C4 == 1) {
            while (C4 == 1);
            __delay_ms(10);
            return '/';
        }
        R2 = 1;
        R1 = R3 = R4 = 0;
        if (C1 == 1) {
            while (C1 == 1);
            __delay_ms(10);
            return '4';
        }
        R2 = 1;
        R1 = R3 = R4 = 0;
        if (C2 == 1) {
            while (C2 == 1);
            __delay_ms(10);
            return '5';
        }
        R2 = 1;
        R1 = R3 = R4 = 0;
        if (C3 == 1) {
            while (C3 == 1);
            __delay_ms(10);
            return '6';
        }
        R2 = 1;
        R1 = R3 = R4 = 0;
        if (C4 == 1) {
            while (C4 == 1);
            __delay_ms(10);
            return '*';
        }
        R3 = 1;
        R1 = R2 = R4 = 0;
        if (C1 == 1) {
            while (C1 == 1);
            __delay_ms(10);
            return '1';
        }
        R3 = 1;
        R1 = R2 = R4 = 0;
        if (C2 == 1) {
            while (C2 == 1);
            __delay_ms(10);
            return '2';
        }
        R3 = 1;
        R1 = R2 = R4 = 0;
        if (C3 == 1) {
            while (C3 == 1);
            __delay_ms(10);
            return '3';
        }
        R3 = 1;
        R1 = R2 = R4 = 0;
        if (C4 == 1) {
            while (C4 == 1);
            __delay_ms(10);
            return '-';
        }
        R4 = 1;
        R1 = R2 = R3 = 0;
        if (C1 == 1) {
            while (C1 == 1);
            __delay_ms(10);
            LCD_Command(0x01);
        }
        R4 = 1;
        R1 = R2 = R3 = 0;
        if (C2 == 1) {
            while (C2 == 1);
            __delay_ms(10);
            return '0';
        }
        R4 = 1;
        R1 = R2 = R3 = 0;
        if (C3 == 1) {
            while (C3 == 1);
            __delay_ms(10);
            return '=';
        }
        R4 = 1;
        R1 = R2 = R3 = 0;
        if (C4 == 1) {
            while (C4 == 1);
            __delay_ms(10);
            return '+';
        }
    }
}