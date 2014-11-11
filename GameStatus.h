/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM to determine the game status 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __GameStatus_h_ 
#define __GameStatus_h_ 

int GameStatus_Tick( int state ) {
	
	unsigned char input = ~PINA; 
	unsigned char button = (input & 0x04) != 0; //input to start/pause/reset 
	
	// transitions 
	switch ( state ) {
		case -1: 
			state = GS_init; 
			break; 
		case GS_init:
			state = GS_menu; 
			break;
		case GS_menu: 
			if ( button ) {		//make sure button is released 
				state = GS_menu; 
			} else {
				state = GS_menu_rel; 
			}
			break; 
		case GS_menu_rel:
			if ( button ) {		//wait for button press to start playing 
				state = GS_play; 
			} else { 
				state = GS_menu_rel; 
			}
			break; 
		case GS_play: 
			if ( has_collision ) {	//check if player has lost the game 
				state = GS_gameOver; 
			} else if ( finishLine && obstacles_num <= 0 ) {	//check if player won the game
				state = GS_won;
			} else if ( button ) {	//wait for button to be released 
				state = GS_play; 
			} else {
				state = GS_play_rel; 
			}
			break; 
		case GS_play_rel: 
			if ( has_collision ) {	//check if player has lost the game
				state = GS_gameOver; 
			} else if ( finishLine && obstacles_num <= 0 ) {	//check if player won the game
				state = GS_won;
			} else if ( button ) {	//check if reset button is pressed
				state = GS_menu; 
			} else {
				state = GS_play_rel; 
			} 
			break; 
		case GS_won: 
			if ( button ) {		//make sure button is released 
				state = GS_won; 
			} else {
				state = GS_won_rel; 
			}
			break; 
		case GS_won_rel: 
			if ( button ) {		//wait for button press to reset game 
				state = GS_play; 
			} else {
				state = GS_won_rel; 
			}
			break; 
		case GS_gameOver: 
			if ( button ) {		//make sure button is released 
				state = GS_gameOver; 
			} else {
				state = GS_gameOver_rel; 
			}
			break; 
		case GS_gameOver_rel: 
			if ( button ) {		//wait for button press to reset game 
				state = GS_play; 
			} else {
				state = GS_gameOver_rel; 
			}
			break; 
		default: 
			state = GS_init; 
			break; 
	}
	
	// state actions (sets global variable to the current game status) 
	switch ( state ) {
		case GS_init:
			break;
		case GS_menu:
			GameStatus = GS_menu; 
			break;
		case GS_menu_rel: 
			GameStatus = GS_menu;
			break; 
		case GS_play:
			GameStatus = GS_play; 
			break;
		case GS_play_rel: 
			GameStatus = GS_play;			
			break; 
		case GS_won:
			GameStatus = GS_won; 
			break;
		case GS_won_rel: 
			GameStatus = GS_won;
			break; 
		case GS_gameOver:
			GameStatus = GS_gameOver;
			break;
		case GS_gameOver_rel: 
			GameStatus = GS_gameOver; 
			break; 
		default:
			break;
	}
	//GameStatus = state; 
	return state; 
}

#endif //_GameStatus_h_ 
