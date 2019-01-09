/*********************************************************
* FILE: ttt_game.c
* 
* Contains all the basic logic for a ttt game.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt_game.h"

game* game_init(){

	game* ttt = (game*)malloc(sizeof(game));

	strcpy(ttt->gamename, "Tic Tac Toe");
	ttt->ply = 1;
	ttt->isActive = 1;
	ttt->dimension = DIMENSION;

	for (int i = 0; i < DIMENSION; i++)
		for (int j = 0; j < DIMENSION; j++){
			ttt->x_array[i][j] = 0;
			ttt->o_array[i][j] = 0;
		}

	// Additional network functionality
	ttt->player = NULL;
	ttt->next = NULL;
	ttt->tree = NULL;

	return ttt;
}

int make_move(game* ttt, int row, int col){

	// Check for invalid moves
	if (!ttt->isActive)
		return -3;
	if (row < 0 || col < 0 || row >= DIMENSION || col >= DIMENSION)
		return -2;
	if (ttt->x_array[row][col] || ttt->o_array[row][col])
		return -1;

	// Perform move
	if (ttt->ply % 2)
		ttt->x_array[row][col] = 1;
	else
		ttt->o_array[row][col] = 1;

	// Check for game end
	return check_end(ttt);
}

int check_end(game* ttt){

	int sum1, sum2;

	// Check for victories
	if (ttt->ply % 2){
		ttt->ply++;

		// Horizontal and vertical victory
		for (int i = 0; i < DIMENSION; i++){

			sum1 = 0;
			sum2 = 0;
			
			for (int j = 0; j < DIMENSION; j++){

				if (ttt->x_array[i][j])
					sum1++;
				if (ttt->x_array[j][i])
					sum2++;
			}
			if (sum1 == DIMENSION || sum2 == DIMENSION){
				ttt->isActive = 0;
				return 2;
			}
		}

		// Diagonal victory
		sum1 = 0;
		sum2 = 0;

		for (int i = 0; i < DIMENSION; i++){

			if (ttt->x_array[i][i])
				sum1++;
			if (ttt->x_array[DIMENSION-1-i][i])
				sum2++;
		}

		if (sum1 == DIMENSION || sum2 == DIMENSION){
			ttt->isActive = 0;
			return 2;
		}
	}
	else{
		ttt->ply++;

		// Horizontal and vertical victory
		for (int i = 0; i < DIMENSION; i++){

			sum1 = 0;
			sum2 = 0;
			
			for (int j = 0; j < DIMENSION; j++){

				if (ttt->o_array[i][j])
					sum1++;
				if (ttt->o_array[j][i])
					sum2++;
			}
			if (sum1 == DIMENSION || sum2 == DIMENSION){
				ttt->isActive = 0;
				return 3;
			}
		}

		// Diagonal victory
		sum1 = 0;
		sum2 = 0;

		for (int i = 0; i < DIMENSION; i++){

			if (ttt->o_array[i][i])
				sum1++;
			if (ttt->o_array[DIMENSION-1-i][i])
				sum2++;
		}

		if (sum1 == DIMENSION || sum2 == DIMENSION){
			ttt->isActive = 0;
			return 3;
		}
	}

	// Check for tie
	if (ttt->ply >= (DIMENSION * DIMENSION + 1)){
		ttt->isActive = 0;
		return 1;
	}

	// Game continues
	return 0;
}

void rollback(game* ttt, int row, int col){

	ttt->x_array[row][col] = 0;
	ttt->o_array[row][col] = 0;
	ttt->isActive = 1;
	ttt->ply--;

	return;
}

void print_game(game* ttt){

	for (int i = 0; i < DIMENSION; i++){

		printf("[");

		for (int j = 0; j < DIMENSION; j++){

			if (ttt->x_array[i][j])
				printf(" x ");
			else if (ttt->o_array[i][j])
				printf(" o ");
			else
				printf(" . ");
		}
		printf("]\n");
	}
	//printf("Current ply: %d\n", ttt->ply);
	//printf("Game state:  %d\n", ttt->isActive);
	return;
}

void sprint_game(game* ttt, char* response){

	for (int i = 0; i < DIMENSION; i++){

		sprintf(response, "%s[", response);

		for (int j = 0; j < DIMENSION; j++){

			if (ttt->x_array[i][j])
				sprintf(response, "%s x ", response);
			else if (ttt->o_array[i][j])
				sprintf(response, "%s o ", response);
			else
				sprintf(response, "%s . ", response);
		}
		sprintf(response, "%s]\n", response);
	}
	return;
}

node* node_init(){

	node* nd = (node*)malloc(sizeof(node));

	nd->value = 0;
	nd->isTerminal = 0;
	for (int i = 0; i < DIMENSION; i++)
		for (int j = 0; j < DIMENSION; j++)
			nd->children[i][j] = NULL;

	return nd;
}
