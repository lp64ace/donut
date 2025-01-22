/*
 * File:   lcd.c
 * Author: Jim
 *
 * Created on January 20, 2025, 5:57 PM
 */

#include "lcd.h"

union {
	unsigned char data;
	struct {
		unsigned char bit0:1;
		unsigned char bit1:1;
		unsigned char bit2:1;
		unsigned char bit3:1;
		unsigned char bit4:1;
		unsigned char bit5:1;
		unsigned char bit6:1;
		unsigned char bit7:1;
	} bits;
} _lcd_ch;

static void lcd_delay_43us(void) {
    __delay_us(43);
}
static void lcd_delay_2ms(void) {
    __delay_ms(2);
}

static void latch_data(void){
	LCD_CTRLE_PORT.LCD_E_PIN = 1;
	lcd_delay_43us();
	LCD_CTRLE_PORT.LCD_E_PIN = 0;
	lcd_delay_43us();
}

static void lcd_load_4lsb(void) {
    LCD_DATA_PORT.LCD_D0_PIN = 0;
	if(_lcd_ch.bits.bit0)
		LCD_DATA_PORT.LCD_D0_PIN = 1;
	LCD_DATA_PORT.LCD_D1_PIN = 0;
	if(_lcd_ch.bits.bit1)
		LCD_DATA_PORT.LCD_D1_PIN = 1;
	LCD_DATA_PORT.LCD_D2_PIN = 0;
	if(_lcd_ch.bits.bit2)
		LCD_DATA_PORT.LCD_D2_PIN = 1;
	LCD_DATA_PORT.LCD_D3_PIN = 0;
	if(_lcd_ch.bits.bit3)
		LCD_DATA_PORT.LCD_D3_PIN = 1;
	latch_data();
}

static void lcd_load_4msb(void){
	LCD_DATA_PORT.LCD_D0_PIN = 0;
	if(_lcd_ch.bits.bit4)
		LCD_DATA_PORT.LCD_D0_PIN = 1;
	LCD_DATA_PORT.LCD_D1_PIN = 0;
	if(_lcd_ch.bits.bit5)
		LCD_DATA_PORT.LCD_D1_PIN = 1;
	LCD_DATA_PORT.LCD_D2_PIN = 0;
	if(_lcd_ch.bits.bit6)
		LCD_DATA_PORT.LCD_D2_PIN = 1;
	LCD_DATA_PORT.LCD_D3_PIN = 0;
	if(_lcd_ch.bits.bit7)
		LCD_DATA_PORT.LCD_D3_PIN = 1;
	latch_data();
}

void lcd_write_cmd(unsigned char cmd) {
    LCD_CTRLE_PORT.LCD_E_PIN = 0;
	LCD_CTRLRS_PORT.LCD_RS_PIN = 0;
    _lcd_ch.data = cmd;
	lcd_load_4msb();
	lcd_load_4lsb();
}

void lcd_write_chr(unsigned char data) {
    LCD_CTRLE_PORT.LCD_E_PIN = 0;
	LCD_CTRLRS_PORT.LCD_RS_PIN = 1;
    _lcd_ch.data = data;
	lcd_load_4msb();
	lcd_load_4lsb();
}

void lcd_write_mov(unsigned char pos) {
    if (pos) {
        lcd_write_cmd(pos|0x80);
    }
    else {
        lcd_home();
    }
}

void lcd_build_chr(unsigned char index, const unsigned char data[8]) {
    lcd_write_cmd(0x06);
    lcd_write_cmd((index * 8)|0x40);
    for(int i = 0; i < 8; i++) {
		lcd_write_chr(data[i]);
	}
}

void lcd_init_4port(void) {
    /** Set LCD pins as outputs */
    LCD_CTRLE_TRIS.LCD_E_TRISPIN = 0;
    LCD_CTRLRS_TRIS.LCD_RS_TRISPIN = 0;
    LCD_DATA_TRIS.LCD_D0_TRISPIN = 0;
    LCD_DATA_TRIS.LCD_D1_TRISPIN = 0;
    LCD_DATA_TRIS.LCD_D2_TRISPIN = 0;
    LCD_DATA_TRIS.LCD_D3_TRISPIN = 0;
    /** Initialize LCD pin values */
    LCD_CTRLRS_PORT.LCD_RS_PIN = 0;
	LCD_CTRLE_PORT.LCD_E_PIN = 0;
    {
        /** Delay until LCD is fully initialized */
		for(_lcd_ch.data=0;_lcd_ch.data<32;_lcd_ch.data++) {
            lcd_delay_2ms();
			CLRWDT();
        }
	}
    
    _lcd_ch.data = 0x02;
	lcd_load_4lsb();
	lcd_delay_2ms();

    latch_data(); /* again (the same data) */
    lcd_delay_2ms();
    
    _lcd_ch.data = 0x08;
    lcd_load_4lsb();
    lcd_delay_2ms();
    
    lcd_append();
    lcd_clear();
}

void lcd_append(void) {
    lcd_write_cmd(0x0c);
    lcd_delay_2ms();
}

void lcd_clear(void) {
    lcd_write_cmd(0x01);
    lcd_delay_2ms();
}

void lcd_home(void) {
    lcd_write_cmd(0x02);
    lcd_delay_2ms();
}
