/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM to count how long it takes to complete the course 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __ElapsedTime_h_ 
#define __ElapsedTime_h_ 

enum ElapsedTime_State {ET_init, ET_inc, ET_stop}; 
int ElapsedTime_Tick(int state) {
	
	//transitions 
	switch(state) {
		case -1: 
			state = ET_init; 
			break; 
		case ET_init: 
			state = ET_inc; 
			break; 
		case ET_inc: 
			if ( GameStatus == GS_play ) {
				state = ET_inc; 
			} else {
				state = ET_stop; 
			}
			break; 
		case ET_stop: 
			if ( GameStatus == GS_play ) {
				state = ET_inc; 
				elapsedTime = 0; 
			} else {
				state = ET_stop; 
			}
			break; 
		default:
			state = ET_init; 
			break; 
	}
	
	//state actions 
	switch(state) {
		case ET_init:
			elapsedTime = 0; 
			break;
		case ET_inc:
			++elapsedTime; 
			break;
		case ET_stop:
			break;
		default:
			break;
	}
	
	return state; 
}

#endif //__ElapsedTime_h_ 
