/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM for player's position with joystick 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __Joystick_h_ 
#define __Joystick_h_

//helper function to initialize ADC input
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

//helper function to set which pin to get ADC input from
void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
}

enum Joystick_State {Joy_init, Joy_set_x, Joy_get_x, Joy_set_y, Joy_get_y};
int Joystick_Tick( int state ) {
	
	//initialize 
	unsigned short x_joystick = 512;
	unsigned short y_joystick = 512;
	//values to check for moving left/right/accel/decel
	unsigned short det_left = 812; 
	unsigned short det_right = 212;
	unsigned short det_accel = 312;
	unsigned short det_decel = 712;
	//store player's current position 
	unsigned char column = car_position; 
	
	//transitions 
	switch(state) {
		case -1: 
			state = Joy_init; 
			break; 
		case Joy_init: 
			state = Joy_set_x; 
			break; 
		case Joy_set_x: 
			state = Joy_get_x; 
			break; 
		case Joy_get_x: 
			state = Joy_set_y; 
			break; 
		case Joy_set_y: 
			state = Joy_get_y; 
			break; 
		case Joy_get_y: 
			state = Joy_set_x; 
			break; 
		default: 
			break; 
	}
	
	//state actions
	switch(state) {
		case Joy_init:
			break;
		case Joy_set_x:
			Set_A2D_Pin(0); 
			break;
		case Joy_get_x:
			x_joystick = ADC; 
			break;
		case Joy_set_y:
			Set_A2D_Pin(1); 
			break;
		case Joy_get_y:
			y_joystick = ADC;
			break;
		default:
			break;
	}
	
	if ( x_joystick > det_left ) {
		//go left 
		if ( column > 0 ) {
			--column; 
		}
	} else if ( x_joystick < det_right ) {
		//go right
		if ( column < 7 ) {
			++column; 
		}
	} else {
		//do nothing 
	}
	
	if ( y_joystick < det_accel ) {
		//go accelerate 
		if ( move_count > 10 ) {
			move_count -= 4; 
		}
	} else if ( y_joystick > det_decel ) {
		//go decelerate  
		if ( move_count < 100 ) {
			move_count += 4; 
		}
	} else if ( state == Joy_get_y ) {
		//return move_count to equilibrium_move_count 
		if ( move_count < equilibrium_move_count ) {
			move_count += 4; 
		} else if ( move_count > equilibrium_move_count ) {
			move_count -= 4;
		}
	}
	
	if ( GameStatus == GS_play ) {
		car_position = column; 
	}
	
	return state; 
}

#endif //__Joystick_h_ 
