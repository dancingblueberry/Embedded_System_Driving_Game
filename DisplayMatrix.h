/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM to display the game to the LED matrix 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __DisplayMatrix_h_ 
#define __DisplayMatrix_h_ 

//helper function to transmit data to register
enum PORT_TRANSMIT { PORT_C_A, PORT_C_B };
void transmit_data(unsigned char data, unsigned char p) {
	switch(p) {
		case PORT_C_A:
			for (int i = 0; i < 8 ; ++i) {
				// Sets SRCLR to 1 allowing data to be set
				// Also clears SRCLK in preparation of sending data
				PORTC = 0x08;
				// set SER = next bit of data to be sent.
				PORTC |= ((data >> i) & 0x01);
				// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
				PORTC |= 0x02;
			}
			// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
			PORTC |= 0x04;
			// clears all lines in preparation of a new transmission
			PORTC = 0x00;
			break;
		case PORT_C_B:
			for (int i = 0; i < 8 ; ++i) {
				// Sets SRCLR to 1 allowing data to be set
				// Also clears SRCLK in preparation of sending data
				PORTC = 0x08 << 4;
				// set SER = next bit of data to be sent.
				PORTC |= ((data >> i) & 0x01) << 4;
				// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
				PORTC |= 0x02 << 4;
			}
			// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
			PORTC |= 0x04 << 4;
			// clears all lines in preparation of a new transmission
			PORTC = 0x00;
			break;
		default:
			break;
	}
}

//task to combine LED matrix outputs
enum Display_States {sm3_init, sm3_display_player, sm3_display_obstacle, sm3_finish_line, sm3_game_over, sm3_won};
int Display_Tick(int state) {
	
	unsigned char column_val;
	unsigned char column_sel;
	static unsigned char obstacle_count = 0;
	
	// === transitions ===
	switch (state) {
		case sm3_init:
			state = sm3_display_player;
			break;
		case sm3_display_player:
			//if there are obstacles to display, then they will be displayed
			if ( GameStatus == GS_gameOver ) {
				state = sm3_game_over; 
				transmit_data(0xFF, PORT_C_A);
				transmit_data(0xFF, PORT_C_B);
			} else if ( GameStatus == GS_won ) {
				state = sm3_won; 
			} else if ( obstacles_num > 0 ) {
				state = sm3_display_obstacle;
			} else {
				state = sm3_display_player;
			}
			obstacle_count = 0;
			break;
		case sm3_display_obstacle:
			//after displaying last obstacle, go back to display player
			if ( obstacle_count >= obstacles_num ) {
				if ( finishLine ) {
					state = sm3_finish_line; 
				} else {
					state = sm3_display_player;
				}
			} else {
				state = sm3_display_obstacle;
			}
			break;
		case sm3_finish_line: 
			state = sm3_display_player; 
			break; 
		case sm3_game_over: 
			if ( !(GameStatus == GS_gameOver ) ) {
				state = sm3_display_player; 
			} else {
				state = sm3_game_over; 
			}
			break; 
		case sm3_won: 
			if ( !(GameStatus == GS_won ) ) {
				state = sm3_display_player;
			} else {
				state = sm3_won;
			}
			break; 
		default:
			state = sm3_init;
			break;
	}
	
	// === state actions ===
	switch(state) {
		case sm3_init:
			column_val = 0x00;
			column_sel = (char)(~(0xFF));
			break;
		case sm3_display_player:
			// clear matrix
			transmit_data(0xFF, PORT_C_A);
			transmit_data(0xFF, PORT_C_B);
			
			//display user position
			column_val = 0x01 << car_position;
			column_sel = (char)(~(0x80));
			PORTD = column_val;
			transmit_data(column_sel, PORT_C_A);	//display player as blue 
			break;
		case sm3_display_obstacle:
			// clear matrix
			transmit_data(0xFF, PORT_C_A);
			transmit_data(0xFF, PORT_C_B);
			
			//display obstacles one by one
			column_val = 0x01 << obstacles[obstacle_count].x;
			column_sel = ~(0x01 << obstacles[obstacle_count].y);
			PORTD = column_val;
			transmit_data(column_sel, PORT_C_B);	//display obstacles as red 
			++obstacle_count;
			break;
		case sm3_finish_line: 
			// clear matrix
			transmit_data(0xFF, PORT_C_A);
			transmit_data(0xFF, PORT_C_B);
 			
			column_val = 0xFF; 
			column_sel = ~(0x01 << finishLine_pos); 
			PORTD = column_val; 
			transmit_data(column_sel, PORT_C_A);
			transmit_data(column_sel, PORT_C_B); 
			break; 
		case sm3_game_over: 
			//enable all red LEDs for dramatic death 
			PORTD = 0x00; 
			transmit_data(0xFF, PORT_C_B);
			//transmit_data(0x00, PORT_C_B);
			break; 
		case sm3_won:
			//enable all red LEDs for dramatic win! 			
			PORTD = 0x00;
			transmit_data(0xFF, PORT_C_A);
			//transmit_data(0x00, PORT_C_A);
			break; 
		default:
			break;
	}
	return state;
}

#endif //__DisplayMatrix_h_ 
