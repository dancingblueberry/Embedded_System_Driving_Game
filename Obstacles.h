/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: SM for moving obstacles 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#ifndef __Obstacles_h_
#define __Obstacles_h_

//helper function to update obstacles and remove obstacles that have reached the bottom
void updateObstacles() {
	for ( unsigned char i = 0; i < obstacles_num; ++i ) {
		if ( obstacles[i].y > 7 ) {
			for ( unsigned char j = i; j < obstacles_num - 1; ++j ) {
				obstacles[j].x = obstacles[j+1].x;
				obstacles[j].y = obstacles[j+1].y;
			}
			--i;
			--obstacles_num;
			//break;
		}
	}
}

//task to move objects down
enum Obstacles_States {sm2_init, sm2_wait, sm2_new_obstacle, sm2_finishLine, sm2_gameOver};
int Obstacles_Tick(int state) {
	
	// === Local Variables ===
	static unsigned char obstacle_interval = 10; // amount of time between each new obstacle
	static unsigned char i = 0;
	static unsigned char temp_finishLine = 0; 
	
	// === Transitions ===
	switch (state) {
		case -1:
			state = sm2_init;
			break;
		case sm2_init:
			if ( GameStatus == GS_menu ) {
				state = sm2_init; 
			} else if ( GameStatus == GS_play ) {
				state = sm2_wait;
			}
			i = 0;
			break;
		case sm2_wait:
			if ( GameStatus == GS_menu ) {
				state = sm2_init;
			} else if ( GameStatus == GS_gameOver || GameStatus == GS_won ) {
				state = sm2_gameOver; 
			} else if ( GameStatus == GS_play ) {
				
				if ( i < move_count ) {
					state = sm2_wait;
					
				} else {
					if ( distance >= course_distance ) {
						state = sm2_finishLine;
					} else {
						state = sm2_new_obstacle;
					}
				}
			} else {
				state = sm2_wait; 
			}
			break;
		case sm2_new_obstacle:
			state = sm2_wait;
			i = 0;
			break; 
		case sm2_finishLine: 
// 			if ( GameStatus == GS_menu || GameStatus == GS_menu_rel ) {
// 				state = sm2_init;
// 			} else if ( GameStatus == GS_gameOver || GameStatus == GS_gameOver_rel ) {
// 				state = sm2_gameOver;
// 			} else if ( GameStatus == GS_play || GameStatus == GS_play_rel ) {
// 				state = sm2_finishLine; 
// 			} else {
// 				state = sm2_wait;
// 			} 
			state = sm2_wait; 
			i = 0; 
			break; 
		case sm2_gameOver:
			if ( GameStatus == GS_play ) {
				state = sm2_init;
			} else {
				state = sm2_gameOver;
			}
			break; 
		default:
			state = sm2_init;
			break;
	}

	// === Actions ===
	switch (state) {
		case sm2_init:
			i = 0; 
			obstacles_num = 0; 
			distance = 0; 
			finishLine_pos = -1;
			break;
		case sm2_wait:
			++i;
			break;
		case sm2_new_obstacle:
			//move existing obstacles down 
			for ( unsigned char j = 0; j < obstacles_num; ++j ) {
				++(obstacles[j].y);
			}
			//update obstacles by removing obstacles that have reached the bottom 
			updateObstacles();
			//create new obstacle 
			if ( obstacles_num < max_obstacles_num ) {
				obstacles[obstacles_num].x = rand() % 8;
				obstacles[obstacles_num].y = 0;
				++obstacles_num;
				++distance; 
			}
			break;
		case sm2_finishLine: 
			temp_finishLine = 1; 
			++finishLine_pos; 
			//move existing obstacles down
			for ( unsigned char j = 0; j < obstacles_num; ++j ) {
				++(obstacles[j].y);
			}
			//update obstacles by removing obstacles that have reached the bottom
			updateObstacles();
			
			break; 
		case sm2_gameOver: 
			i = 0;
			obstacles_num = 0;
			finishLine_pos = -1;
			temp_finishLine = 0; 
			break; 
		default:
			break;
	}
	
	finishLine = temp_finishLine; 
	
	return state;
}

#endif //__Obstacles_h_ 
