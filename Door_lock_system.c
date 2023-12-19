/*
 * File:   Door_lock_system.c
 * Author: ramesh
 *
 * Created on 19 December, 2023, 10:56 AM
 */


#include <xc.h>
#include <string.h>

#define _XTAL_FREQ 20000000
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

#define keypad PORTB

#define R1 RB0
#define R2 RB1
#define R3 RB2
#define R4 RB3
#define C1 RB4
#define C2 RB5
#define C3 RB6
#define C4 RB7

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

void initialize() {
    LCD_Command(0x38);
    LCD_Command(0x06);
    LCD_Command(0x0C);
    LCD_Command(0x01);
}

char get_keypad_key(void) {
    while(1){
        R1=1;
        R2=R3=R4=0;
        if(C1==1){
            while(C1==1);
            __delay_ms(10);
            return '1';
        }
        R1=1;
        R2=R3=R4=0;
        if(C2==1){
            while(C2==1);
            __delay_ms(10);
            return '2';
        }
        R1=1;
        R2=R3=R4=0;
        if(C3==1){
            while(C3==1);
            __delay_ms(10);
            return '3';
        }
        R2=1;
        R1=R3=R4=0;
        if(C1==1){
            while(C4==1);
            __delay_ms(10);
            return '4';
        }
        R2=1;
        R1=R3=R4=0;
        if(C2==1){
            while(C1==1);
            __delay_ms(10);
            return '5';
        }
        R2=1;
        R1=R3=R4=0;
        if(C3==1){
            while(C3==1);
            __delay_ms(10);
            return '6';
        }
        R3=1;
        R1=R2=R4=0;
        if(C1==1){
            while(C1==1);
            __delay_ms(10);
            return '7';
        }
        R3=1;
        R1=R2=R4=0;
        if(C2==1){
            while(C2==1);
            __delay_ms(10);
            return '8';
        }
        R3=1;
        R1=R2=R4=0;
        if(C3==1){
            while(C3==1);
            __delay_ms(10);
            return '9';
        }
        R4=1;
        R1=R2=R3=0;
        if(C1==1){
            while(C1==1);
            __delay_ms(10);
            return '*';
        }
        R4=1;
        R1=R2=R3=0;
        if(C2==1){
            while(C2==1);
            __delay_ms(10);
            return '0';
        }
        R4=1;
        R1=R2=R3=0;
        if(C3==1){
            while(C3==1);
            __delay_ms(10);
            return '#';
        }
    }
}

void LCD_str(char str[]) {
    for(int i=0;str[i]!='\0';i++){
        LCD_data(str[i]);
    }
}

void main(void) {
    TRISC=0x00;
    TRISD=0x00;
    PORTC=0x00;
    char keyvalue = 0;
    unsigned int keycount = 0;
     int comparepassword = 0;
    char defaultpassword[10] = "664422";
    char enterpassword[10];
     PORTDbits.RD6=0;

    TRISC = 0x00; // Set PORTC as output for LCD
    PORTC = 0x00; // Initialize PORTC to low
    initialize(); 
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB3=0;
    TRISBbits.TRISB4=1;
    TRISBbits.TRISB5=1;
    TRISBbits.TRISB6=1;
    while (1) {
        LCD_Command(0x80);
        LCD_str("Enter_Password");
        LCD_Command(0xC0);
        while (keycount < 6) {
            keyvalue = get_keypad_key();
            if (keyvalue != 0) {
                enterpassword[keycount] = keyvalue;
                LCD_data('*');
                keycount++;
                __delay_ms(200);
            }
        }
        enterpassword[keycount] = '\0'; // Null-terminate the entered password
        keycount = 0;

        LCD_Command(0x01); // Clear the LCD

        comparepassword = strcmp(defaultpassword, enterpassword);
        if (comparepassword == 0) {
            PORTDbits.RD6=1;
            LCD_Command(0x80);
            LCD_str("Door Opened");
            __delay_ms(2000);
            
            PORTDbits.RD6=0;
            LCD_Command(0x80);
            LCD_str("Door Closed");
            __delay_ms(2000);
            LCD_Command(0x01);
        } else {
            LCD_Command(0x80);
            LCD_str("Wrong Password");     
            __delay_ms(2000);
            LCD_Command(0x01);
        }
    }
}
