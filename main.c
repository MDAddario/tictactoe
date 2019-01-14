/*********************************************************
* FILE: main.c
*
* Main program that allows the player to play
* against the perfect computer.
*
* AUTHOR: Michael Lindner-D'Addario
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ttt_game.h"
#include "optimal_play.h"
#include "full_solve.h"

int main(){

	// Generate game tree
	int isDebug = 0;
	node* tree_head = tree_generation(isDebug);

	// Pick who goes first
	int is_CPU_P1;
	char answer = 0;
	while (answer != 'y' && answer != 'n'){
		printf("Do you wish to start? (y/n):\n");
		scanf("%c", &answer);
	}
	if (answer == 'y')
		is_CPU_P1 = 0;
	else
		is_CPU_P1 = 1;

	// Challenge the computer!
	play_against_cpu(NULL, tree_head, is_CPU_P1);

	return 0;
}
