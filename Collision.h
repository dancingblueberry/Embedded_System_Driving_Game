/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM for determining if the player and obstacles collided 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __Collision_h_ 
#define __Collision_h_ 

//state machine to check for obstacle colliding with player
//input: obstacles
//output: has_collision
enum Collision_States { col_init, col_check };
int Collision_Tick( int state ) {

	//transitions
	switch( state ) {
		case -1:
			state = col_init;
			break;
		case col_init:
			state = col_check;
			break;
		case col_check:
			state = col_check;
			unsigned char i; 
			for ( i = 0; i < obstacles_num; ++i ) {
				if ( (obstacles[i].y >= 7) && (obstacles[i].x == car_position) ) {
					//raise flag if collision is found 
					has_collision = 1;
					break;
				}
			}
			//lower flag if no collision is found 
			if ( i >= obstacles_num ) {
				has_collision = 0; 
			}
			break;
		default:
			state = col_init;
			break;
	}

	//state actions
	switch( state ) {
		case col_init:
			has_collision = 0;
			break;
		case col_check:
			break;
		default:
			break;
	}

	return state;
}

#endif //__Collision_h_ 
