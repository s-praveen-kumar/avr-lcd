#include "lcd.h"
#include<avr/io.h>
#include<util/delay.h>

void sendToLCD(uint8_t value, int mode);
void pulseEn();
void initLCD(volatile uint8_t *port, int en, int rs){
    lcd_data_port = port;
    en_pin = en;
    rs_pin = rs;
    *lcd_data_port |=(1<<en_pin);
	_delay_ms(50);
    //Todo: Change these commands
    writeCommand(0x32);
	writeCommand(0x28);
	writeCommand(0x01);
	writeCommand(0x0F);
	writeCommand(0x06);
}

void sendToLCD(uint8_t value, int mode){
    uint8_t temp = *lcd_data_port & 0x0F;
    if(mode)
        temp |= (mode<<rs_pin);
    else
        temp &= ~(mode<<rs_pin);
    temp |= value&0xF0; //Upper nibble
    *lcd_data_port = temp;
    pulseEn();

    temp &= 0x0F;
    temp |= value<<4;  //Lower nibble
    *lcd_data_port = temp;
    pulseEn();
}

void writeCommand(uint8_t cmd){
    sendToLCD(cmd,0);
}

void writeData(uint8_t data){
    sendToLCD(data,1);
}

void pulseEn(){
    *lcd_data_port&=~(1<<en_pin);
    _delay_ms(LCD_PULSE_DELAY_MS);
    *lcd_data_port|=(1<<en_pin);
    _delay_ms(LCD_PULSE_DELAY_MS);
}
