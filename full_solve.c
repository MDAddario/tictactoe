/*********************************************************
* FILE: full_solve.c
*
* Completely solves game and generates game tree.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ttt_game.h"
#include "full_solve.h"

int fullSolveAlgo(game* ttt, node* nd, int result, int isP1, int isDebug){

	// Base case: Terminal node
	if (result){
		nd->value = result;
		nd->isTerminal = 1;
		return result;
	}

	// Given node will have children; store outcomes in array
	// Array entries follow from the return code values
	int* outcomes = (int*)calloc(4, sizeof(int));
	int count = 1;
	int cutoff = 3;

	// Make every possible move
	for (int row = 0; row < DIMENSION; row++)
	for (int col = 0; col < DIMENSION; col++){

		if (isDebug){
			if (ttt->ply <= cutoff){
				for (int k = 1; k < ttt->ply; k++)
					printf("\t");
				printf("Ply %d: Count %d/%d\n", ttt->ply, count++, DIMENSION*DIMENSION);
			}
		}

		// Ignore illegal moves
		if((result = make_move(ttt, row, col)) < 0)
			continue;

		// Find the child
		nd->children[row][col] = node_init();

		// Work with legal moves
		result = fullSolveAlgo(ttt, nd->children[row][col], result, !isP1, isDebug);
		outcomes[result] = 1;
		rollback(ttt, row, col);

		/*
		The following block is useful if the ultimate outcome of the game is desired
		However, to ensure all game paths are explored for a full brute force,
		this block must be disabled 

		// Optimal play pruning
		if (isOptimal){

			if (isP1){
				if (outcomes[2]){
					free_ptr(&outcomes);
					nd->value = 2;
					return 2;
				}
			}
			else{
				if (outcomes[3]){
					free_ptr(&outcomes);
					nd->value = 3;
					return 3;
				}
			}
		}
		*/
	}
	// All moves completed, player will opt to win, then tie, then lose
	if (isP1){

		if (outcomes[2])
			result = 2;
		else if (outcomes[1])
			result = 1;
		else
			result = 3;
	}
	else{

		if (outcomes[3])
			result = 3;
		else if (outcomes[1])
			result = 1;
		else
			result = 2;
	}
	free_ptr(&outcomes);
	nd->value = result;
	return result;
}

node* tree_generation(int isDebug){

	// Setup board and tree
	game* ttt = game_init();
	node* tree_head = node_init();

	// Generate game tree
	clock_t start = clock(), diff;
	int code = fullSolveAlgo(ttt, tree_head, 0, 1, isDebug);
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;

	// Print outcome
	char result[100];
	if (isDebug){
		sprintf(result, "The outcome of %s %dx%d is:", ttt->gamename, DIMENSION, DIMENSION);
		if (code == 1)
			printf("%s A tie!\n", result);
		else if (code == 2)
			printf("%s P1 wins!\n", result);
		else if (code == 3)
			printf("%s P2 wins!\n", result);
		printf("Time taken: %d hours %d minutes %d seconds %d milliseconds\n", 
			msec/3600000, msec/60000, msec/1000, msec%1000);
	}

	return tree_head;
}

void free_ptr(int** ptr){

	free(*ptr);
	*ptr = NULL;
	return;
}
