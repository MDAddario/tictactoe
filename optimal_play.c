/*********************************************************
* FILE: optimal_play.c
*
* Algorithm that the computer follows to read the
* game tree and play perfectly.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ttt_game.h"
#include "optimal_play.h"

void play_against_cpu(game* ttt, node* nd, int is_CPU_P1){

	// Create game if start
	if (ttt == NULL){
		ttt = game_init();
	}

	print_game(ttt);

	// Ensure game not over
	if (!ttt->isActive)
		return;

	int row, col;

	// Player turn
	if (!is_CPU_P1){

		printf("Its the players turn!\n");
		printf("Please input row and column:\n");
		scanf("%d %d", &row, &col);

		while(make_move(ttt, row, col) < 0){
			printf("Please input a valid move:\n");
			scanf("%d %d", &row, &col);
		}
	}
	// CPU turn
	else{

		printf("Its the CPUs turn!\n");

		int move_found = 0;

		for (int i = 0; i < DIMENSION; i++){
			for (int j = 0; j < DIMENSION; j++){

				if (nd->children[i][j] != NULL){

					if (nd->children[i][j]->value == nd->value){
						move_found = 1;
						row = i;
						col = j;
						break;
					}
				}
			if (move_found)
				break;
			}
		}
		printf("%d %d\n", row, col);
		make_move(ttt, row, col);
	}
	play_against_cpu(ttt, nd->children[row][col], !is_CPU_P1);

	return;
}