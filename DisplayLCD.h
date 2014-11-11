/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM to display to the LCD 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __DisplayLCD_h_
#define __DisplayLCD_h_

#include "io.c"
#include <avr/pgmspace.h> // Built in avr library

//top left most corner
const uint8_t Car_TLL[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b01111,
	0b10000,
	0b10000,
	0b01111,
	0b00000,
};

//top middle left
const uint8_t Car_TML[] PROGMEM = {
	0b01111,
	0b10000,
	0b10000,
	0b00000,
	0b00000,
	0b11000,
	0b00111,
	0b11000,
};

//top middle right
const uint8_t Car_TMR[] PROGMEM = {
	0b11110,
	0b00001,
	0b00001,
	0b00000,
	0b00000,
	0b00011,
	0b11100,
	0b00011,
};

//top right most corner
const uint8_t Car_TRR[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b11110,
	0b00001,
	0b00001,
	0b11110,
	0b00000,
};

// The following function can be found in the lcd_lib.c file found on the accompanying website
// The only change I made was the function names to our pre-built functions (LCD_WriteCommand and LCD_WriteData)
// as they have the same functionality.
void LCDdefinechar(const uint8_t *pc,uint8_t char_code){
	uint8_t a, pcc;
	uint16_t i;
	a=(char_code<<3)|0x40;
	for (i=0; i<8; i++){
		pcc=pgm_read_byte(&pc[i]);
		LCD_WriteCommand(a++);
		LCD_WriteData(pcc);
	}
}

void init_CustomCar() {
	//initialize characters for custom car 
	LCDdefinechar(Car_TLL,0);
	LCDdefinechar(Car_TML,1);
	LCDdefinechar(Car_TMR,2);
	LCDdefinechar(Car_TRR,3);
}

void display_CustomChar() {
	//write car to LCD
	LCD_Cursor(1);
	LCD_WriteData(0b00001000);
	LCD_Cursor(2);
	LCD_WriteData(0b00001001);
	LCD_Cursor(3);
	LCD_WriteData(0b00001010);
	LCD_Cursor(4);
	LCD_WriteData(0b00001011);
}

//helper function to display the elapsed time and distance travelled 
void display_Short( unsigned short input ) {

	unsigned short temp = input; 

	LCD_Cursor(26);
	LCD_WriteData( (temp%10)  + '0' );
	temp /= 10;

	LCD_Cursor(25);
	LCD_WriteData( (temp%10)  + '0' );
	temp /= 10;

	LCD_Cursor(24);
	LCD_WriteData( (temp%10)  + '0' );
	temp /= 10;
}

enum DisplayLCD_States {DisplayLCD_init, DisplayLCD_wait, DisplayLCD_InGame, DisplayLCD_GameOver, DisplayLCD_Won};
int DisplayLCD_Tick( int state ) {
	
	//transitions
	switch( state ) {
		case -1:
			state = DisplayLCD_init;
			break;
		case DisplayLCD_init:
			state = DisplayLCD_wait;
			LCD_DisplayString(6, "DRIVE      Press start");
			init_CustomCar();
			display_CustomChar();
			LCD_Cursor(0);
			break;
		case DisplayLCD_wait:
			if ( GameStatus == GS_gameOver ) {
				state = DisplayLCD_GameOver; 
				LCD_DisplayString(6, "GAME OVER! Press reset"); 
				display_CustomChar();
				LCD_Cursor(0); 
			} else if ( GameStatus == GS_play ) {
				state = DisplayLCD_InGame; 
				LCD_DisplayString(6, "DRIVE      In game...");
				display_CustomChar();
				LCD_Cursor(0);
			} else if ( GameStatus == GS_won ) {
				state = DisplayLCD_Won; 
				LCD_DisplayString(6, "YOU WON!");
				LCD_Cursor(0);  
			} else {
				state = DisplayLCD_wait;
			}
			break;
		case DisplayLCD_InGame: 
			if ( GameStatus == GS_play ) {
				state = DisplayLCD_InGame; 
			} else if ( GameStatus == GS_menu ) {
				state = DisplayLCD_init; 
			} else if ( GameStatus == GS_gameOver ) {
				state = DisplayLCD_GameOver; 
				LCD_DisplayString(6, "YOU LOST!  Dist:");
				display_CustomChar();
				display_Short(distance);
				LCD_Cursor(0); 
			} else if ( GameStatus == GS_won ) {
				state = DisplayLCD_Won;
				LCD_DisplayString(6, "YOU WON!   Time:");
				display_CustomChar();
				display_Short(elapsedTime);
				LCD_Cursor(0);
			}
			break; 
		case DisplayLCD_GameOver: 
			if ( GameStatus == GS_gameOver ) {
				state = DisplayLCD_GameOver; 
			} else if ( GameStatus == GS_menu ) {
				state = DisplayLCD_init; 
			} else {
				state = DisplayLCD_wait; 
			}
			break;
		case DisplayLCD_Won:
			if ( GameStatus == GS_won ) {
				state = DisplayLCD_Won;
			} else {
				state = DisplayLCD_wait; 
			}
			break;
		default:
			break;
	}
	
	//state actions
	switch( state ) {
		case DisplayLCD_init:
			break;
		case DisplayLCD_wait:
			break;
		case DisplayLCD_GameOver: 
			break; 
		case DisplayLCD_Won: 
			break; 
		default:
			break;
	}
	
	return state;
}

#endif //__DisplayLCD_h_ 
