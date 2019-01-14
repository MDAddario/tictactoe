/*********************************************************
* FILE: full_solve.c
*
* Completely solves game and generates game tree.
*
* AUTHOR: Michael Lindner-D'Addario
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
		sprintf(result, "The outcome of Tic-tac-toe %dx%d is:", DIMENSION, DIMENSION);
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
