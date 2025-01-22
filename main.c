/**
 * File:   main.c
 * Author: Jim
 *
 * Created on January 20, 2025, 3:56 PM
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "config.h"
#include "lcd.h"

#define EXIT_SUCCESS 0

const float slat = 0.45f;
const float slon = 0.15f;

const float R1 = 1.0f;
const float R2 = 2.0f;
const float K2 = 5.0f;
const float K1 = 16.0f * K2 * 3.0f / (8.0f * (R1 + R2));

void draw(float A, float B) {
    const float cosA = cos(A), sinA = sin(A);
    const float cosB = cos(B), sinB = sin(B);
    
    char chars[6][8];
    char depth[16][16];
    
    memset(chars, 0, sizeof(chars));
    memset(depth, 0, sizeof(depth));
    
    for (float theta=0; theta < 2 * M_PI; theta += slat) {
        const float costheta = cos(theta), sintheta = sin(theta);
        const float circlex = R2 + R1 * costheta;
        const float circley = R1 * sintheta;
        
        for(float phi=0; phi < 2 * M_PI; phi += slon) {
            const float cosphi = cos(phi), sinphi = sin(phi);
            
            const float L = cosphi * costheta * sinB - cosA * costheta * sinphi - sinA * sintheta + cosB * (cosA * sintheta - costheta * sinA * sinphi);
            if (L <= 0.0f) {
                continue;
            }
            const float x = circlex * (cosB * cosphi + sinA * sinB * sinphi) - circley * cosA * sinB;
            const float y = circlex * (sinB * cosphi - sinA * cosB * sinphi) + circley * cosA * cosB;
            const float z = K2 + cosA * circlex * sinphi + circley * sinA;
            
            unsigned char xp = (unsigned char) (16.0f / 2.0f + K1 * x / z);
            unsigned char yp = (unsigned char) (16.0f / 2.0f - K1 * y / z);
            if (xp < 0 || yp < 0 || xp > 16 || yp > 16) {
                continue;
            }
            if ((unsigned char)(127.0f / z) < depth[xp][yp]) {
                continue;
            }
            depth[xp][yp] = (unsigned char)(127.0f / z);
            
            const unsigned char block = 3 * (yp / 8) + (xp / 5);
            chars[block][yp % 8] |= (0x10 >> (xp % 5));
            
            CLRWDT();
        }
    }
    
    lcd_clear();
    for(unsigned char block = 0; block < 6; block++) {
        lcd_build_chr(block, (unsigned char *)chars[block]);
        if (block / 3) {
            lcd_write_mov(LCD_LINE2 + 7 + (block % 3));
        }
        else {
            lcd_write_mov(LCD_LINE1 + 7 + (block % 3));
        }
        lcd_write_chr(block);
    }
}

int main(int argc, char** argv) {
    TRISAbits.RA2 = 0;
    LATAbits.LA2 = 1;
    lcd_init_4port();
    LATAbits.LA2 = 0;
    
    unsigned char elapsed_T = 0;
    
    while(true) {
        elapsed_T ++;
        draw(0.2f * (float)elapsed_T, 0.1f * (float)elapsed_T);
        LATAbits.LA2 = (elapsed_T % 2) == 0; // blink!
        CLRWDT();
    }
    
    return (EXIT_SUCCESS);
}
