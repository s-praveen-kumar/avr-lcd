#include<stdint.h>
#ifndef _LCD_H
#define _LCD_H
#define LCD_PULSE_DELAY_MS 2
volatile uint8_t *lcd_data_port;
int rs_pin, en_pin;

void initLCD(volatile uint8_t *port, int en, int rs);
void writeCommand(uint8_t cmd);
void writeData(uint8_t data);
#endif