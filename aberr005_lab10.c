/*	Amanda Berryhill  |  aberr005@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab10 - Custom Lab 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "timer.h"

// === global variables === 
//global variable for the game's status
enum GameStatus_State { GS_init, GS_menu, GS_menu_rel, GS_play, GS_play_rel, GS_pause, GS_pause_rel, GS_gameOver, GS_gameOver_rel, GS_won, GS_won_rel } GameStatus;

//globals for player/joystick task 
unsigned char car_position = 0;
unsigned char move_count = 50; 
unsigned char equilibrium_move_count = 50; 

//globals for obstacle task 
typedef struct obstacle {	//struct for position of falling obstacles 
	unsigned char x;
	unsigned char y;
} obstacle;
obstacle obstacles[8];	//array of current obstacles 
unsigned char obstacles_num = 0; // current number of obstacles
const unsigned char max_obstacles_num = 8; // max number of obstacles at once
unsigned short distance = 0;		// how far the player has travel 
unsigned short course_distance = 100;		// length of course (where the finish line is) 
unsigned short elapsedTime = 0;		// how much time has elapsed since starting the game 
unsigned char finishLine = 0;		// boolean: true if the finish line should be visible 
signed char finishLine_pos = -1;	// where the finish line is on the LED matrix 

//globals for collision task 
unsigned char has_collision = 0; // boolean: true if player has collided with an obstacle

#include "Joystick.h"	// SM for player's position with joystick 
#include "Obstacles.h"	// SM for moving obstacles 
#include "Collision.h"	// SM for determining if the player and obstacles collided 
#include "GameStatus.h"	// SM to determine the game status 
#include "ElapsedTime.h"	// SM to count how long it takes to complete the course 
#include "DisplayMatrix.h"	// SM to display the game to the LED matrix 
#include "DisplayLCD.h"		// SM to display to the LCD 

//struct for task
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsed_time;
	int (*Tick_Fcn) (int);
} task;

//global variables
const unsigned char numTasks = 7;
task tasks[7];

int main() {
	
	DDRA = 0xF0; PORTA = 0x0F; 
	DDRB = 0xFF; PORTB = 0x00; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char i = 0; 
// 	get joystick input 
	tasks[i].state = -1; 
	tasks[i].period = 30; 
	tasks[i].elapsed_time = tasks[i].period; 
	tasks[i].Tick_Fcn = &Joystick_Tick; 
	++i; 
	
// 	falling obstacles  
	tasks[i].state = -1;
	tasks[i].period = 10;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &Obstacles_Tick;
	++i; 

// 	check for collision/death 
	tasks[i].state = -1;
	tasks[i].period = 10;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &Collision_Tick;
	++i;
	
// 	determine game status 
	tasks[i].state = -1;
	tasks[i].period = 8;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &GameStatus_Tick;
	++i;
	
	// 	determine game status
	tasks[i].state = -1;
	tasks[i].period = 1000;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &ElapsedTime_Tick;
	++i;
	
//  output to matrix 
	tasks[i].state = -1;
	tasks[i].period = 1;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &Display_Tick;
	++i; 
	
// 	display LCD output
	tasks[i].state = -1;
	tasks[i].period = 100;
	tasks[i].elapsed_time = tasks[i].period;
	tasks[i].Tick_Fcn = &DisplayLCD_Tick;
	
	LCD_init(); 
	ADC_init();
	unsigned long periodGCD = 1; 
	TimerSet(periodGCD);
	TimerOn();
	
	while (1) {
		for ( unsigned char i = 0; i < numTasks; ++i ) {
			if ( tasks[i].elapsed_time >= tasks[i].period ) {
				tasks[i].state = tasks[i].Tick_Fcn( tasks[i].state ); 
				tasks[i].elapsed_time = 0; 
			}
			tasks[i].elapsed_time += periodGCD; 
		}
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	return 0;
}
