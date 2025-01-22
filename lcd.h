#ifndef LCD_H
#define LCD_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "config.h"

/*
	pin 	func
	------------
	1       Vss
	2       Vcc
	3       Vo
	4       RS
	5       R/!W (GND)
	6       ENABLE
	7       D8_0
	8       D8_1
	9       D8_2
	10      D8_3
	11      D8_4	D4_0
	12      D8_5	D4_1
	13      D8_6	D4_2
	14      D8_7	D4_3
	15      Backlight
	16      Backlight
*/
    
#define	LCD_CTRLE_PORT          LATAbits
#define	LCD_CTRLRS_PORT         LATAbits
#define LCD_CTRLRS_PORT4READ	PORTAbits
#define	LCD_E_PIN               LATA0
#define	LCD_RS_PIN              LATA1
#define	LCD_RS_PIN4READ         RA1
//
#define	LCD_CTRLE_TRIS          TRISAbits
#define	LCD_CTRLRS_TRIS         TRISAbits
#define	LCD_E_TRISPIN           TRISA0
#define	LCD_RS_TRISPIN          TRISA1

#define	LCD_DATA_PORT           LATBbits
#define	LCD_D0_PIN              LATB0
#define	LCD_D1_PIN              LATB1
#define	LCD_D2_PIN              LATB2
#define	LCD_D3_PIN              LATB3
//
#define	LCD_DATA_TRIS           TRISBbits
#define	LCD_D0_TRISPIN          TRISB0
#define	LCD_D1_TRISPIN          TRISB1
#define	LCD_D2_TRISPIN          TRISB2
#define	LCD_D3_TRISPIN          TRISB3

#define LCD_LINE1 0x00
#define LCD_LINE2 0x40

void lcd_init_4port(void);
void lcd_write_cmd(unsigned char cmd); // 86us
void lcd_write_chr(unsigned char data); // 86us
void lcd_write_mov(unsigned char pos); // 86us

void lcd_build_chr(unsigned char index, const unsigned char data[8]);

#define lcd_write_fmt(...) \
    do { \
        char buffer[16]; \
        char length = (char)snprintf(buffer, 16, __VA_ARGS__); \
        for(char i = 0; i < length; i++) { \
            lcd_write_chr(buffer[i]); \
        } \
    } while(false)

void lcd_append(void); // 2ms + 86us
void lcd_clear(void); // 2ms + 86us
void lcd_home(void); // 2ms + 86us

#endif	//LCD_H