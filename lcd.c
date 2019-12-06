#include "lcd.h"
#include<avr/io.h>
#include<util/delay.h>

void sendToLCD(uint8_t value, int mode);
void pulseEn();
void initLCD(volatile uint8_t *port, int en, int rs){
    lcd_data_port = port;
    en_pin = en;
    rs_pin = rs;
    _delay_ms(50);
    *lcd_data_port |=(1<<en_pin);
    *lcd_data_port &= ~(1<<rs_pin);

	writeCommand(0x32);
	writeCommand(0x28);
	clearScreen();
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
    *lcd_data_port &= ~(1<<rs_pin);
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

void printStr(char *s){
	int i;
	for(i=0;s[i]!='\0';i++)
		writeData(s[i]);
}

void printCh(char c){
	writeData(c);
}

void clearScreen(){
	writeCommand(0x01);
	writeCommand(0x02);
}

void hideCursor(){
	writeCommand(0x0C);
}

void showCursor(){
	writeCommand(0x0E);
}
void blinkCursor(){
	writeCommand(0x0F);
}

void printInt(int n){
	if(n<0){
		writeData('-');
		n*=-1;
	}
	int x = 10000;
	while(!(n/x)){
		x/=10;
		if(x==0){
			writeData('0');
			return;
		}
	}
	while(x>0){
		writeData('0'+n/x);
		n = n%x;
		x/=10;
	}
}

void moveToLine1(){
	writeCommand(0x80);
}

void moveToLine2(){
	writeCommand(0xC0);
}

void moveToAddress(uint8_t add){
	writeCommand(0x80 | add);
}

void moveToXY(uint8_t x, uint8_t y){
	if(y)
		writeCommand(0xC0 | x);
	else
		writeCommand(0x80 | x);
}
