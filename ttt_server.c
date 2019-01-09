/*********************************************************
* FILE: ttt_server.c
* 
* The brains of superplay. Stores a linked list for the
* users, for the games, generates game tree and the CPU
* plays optimally against the users.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <stdlib.h>
#include <time.h>
#include "ttt_game.h"
#include "full_solve.h"

user *user_list_head = NULL;
game *game_list_head = NULL;

// Login function to run every time
user* login(char* username, char* password, int* result){
	
	/* Codes:
	 * result = 0 -> Successful login			-> return pointer to user
	 * result = 1 -> Password incorrect			-> return NULL
	 * result = 2 -> User does not exist		-> return pointer to last user in linked list
	 * result = 3 -> No users exist in system	-> return NULL
	 */
	
	// Check to see if no users exist
	if (user_list_head == NULL){
		*result = 3;
		return NULL;
	}
	// Find user in linked list
	user* p = user_list_head;
	while (1){
		
		if (strcmp(p->username, username) == 0){
			if (strcmp(p->password, password) == 0){
				*result = 0;
				return p;
			}
			else{
				*result = 1;
				return NULL;
			}
		}
		if (p->next == NULL)
			break;
		p = p->next;
	}
	// User not in list
	*result = 2;
	return p;
}

// Search linked list for given game
game* find_game(char* gamename, int* game_status){
	
	/* Codes:
	 * game_status = 0 -> Game found				-> return pointer to game
	 * game_status = 2 -> Game does not exist		-> return pointer to last game in linked list
	 * game_status = 3 -> No games exist in system	-> return NULL
	 */
	 
	// Ensure there are active games
	if (game_list_head == NULL){
		*game_status = 3;
		return NULL;
	}
	
	// Find game in linked list
	game* g = game_list_head;
	while (1){
		
		if (strcmp(g->gamename, gamename) == 0){
			*game_status = 0;
			return g;
		}
		if (g->next == NULL)
			break;
		g = g->next;
	}
	// Game not in list
	*game_status = 2;
	return g;
}

// The main attraction
int main(){

	// Build game tree
	int isDebug = 0;
	node* tree_head = tree_generation(isDebug);

	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[2000];

	unsigned short int port = MYPORT;
	
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1){
		printf("Could not create socket.");
		return 1;
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );
	
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		perror("Bind failed. Error");
		return 1;
	}

	listen(socket_desc , 3);

	printf("Game server ready on port %d.\n", port);
	
	while (1){
		
		c = sizeof(struct sockaddr_in);

		// Accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0){
			perror("Accept failed");
			return 1;
		}

		char temp[200];
		memset(client_message, '\0', 200);
		int bytes_read = 0;
		while (bytes_read < 200){
			read_size = recv(client_sock, temp, 200, 0);
			if (read_size <= 0){
				puts("Client disconnected");
				fflush(stdout);
				close(client_sock);
				close(socket_desc);
				return 0;
			}
			memcpy(client_message+bytes_read, temp, read_size);
			bytes_read += read_size;
		}

		char response[2000];
		memset(response, '\0', 2000);
		char *command = strtok( client_message, "," );
		char *username = strtok( NULL, "," );
		char *password = strtok( NULL, ",");

		if (command == NULL || username == NULL || password == NULL){
			sprintf(response, "PLEASE ENTER A USERNAME AND PASSWORD.\n");
			write(client_sock , response , 2000);  
			close(client_sock);
			continue;
		}
		
		// Login credentials
		int result;
		user* p = login(username, password, &result);
		
		// Bad password
		if (result == 1){
			sprintf(response, "PASSWORD INCORRECT FOR USER %s.\n", username);
			write(client_sock , response , 2000);  
			close(client_sock);
			continue;
		}
		
		if (strcmp(command, "LOGIN") == 0){
			
			// Successful login
			if (result == 0){
				sprintf(response, "LOGIN SUCCESSFUL FOR USER %s", username);
			}
			// First user ever
			else if (result == 3){
				user* pioneer = (user*)malloc(sizeof(user));
				strcpy(pioneer->username, username);
				strcpy(pioneer->password, password);
				pioneer->next = NULL;
				
				user_list_head = pioneer;
				strcpy(response, "NEW USER CREATED OK.");
			}
			// Create new user
			else{
				user* new_user = (user*)malloc(sizeof(user));
				strcpy(new_user->username, username);
				strcpy(new_user->password, password);
				new_user->next = NULL;
				
				p->next = new_user;
				strcpy(response, "NEW USER CREATED OK.");
			}
		}

		else if (strcmp(command, "CREATE") == 0){

			// Login validate
			if (result){
				sprintf(response, "USER %s DOES NOT EXIST.\n", username);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// Parse rest of query string
			char *game_name = strtok( NULL, ",");
			
			// Validate game_name
			if (game_name == NULL){
				sprintf(response, "GAMENAME INVALID.\n");
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// Create game
			game* ttt = game_init();
			strcpy(ttt->gamename, game_name);
			ttt->player = p;
			ttt->next = NULL;
			ttt->tree = tree_head;

			// Scan through current existing games
			int game_status;
			game* g = find_game(game_name, &game_status);

			// Gamename already exists
			if (game_status == 0){
				sprintf(response, "GAME ALREADY EXISTS WITH NAME %s.\n", game_name);
				free(ttt);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}
			// First ever game
			else if (game_status == 3)
				game_list_head = ttt;
			// Append game to linked list
			else
				g->next = ttt;

			sprint_game(ttt, response);
			sprintf(response, "%sGAME %s SUCCESFULLY CREATED.\n", response, game_name);
		}
		
		else if (strcmp(command, "MOVE") == 0){

			// Login validate
			if (result){
				sprintf(response, "USER %s DOES NOT EXIST.\n", username);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// Parse rest of query string
			char *game_name = strtok( NULL, ",");
			char *square = strtok( NULL, ",");
			
			// Validate game_name
			if (game_name == NULL){
				sprintf(response, "INVALID GAME NAME.");
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}
			
			// Scan current games
			int game_status;
			game* ttt = find_game(game_name, &game_status);
			
			// Ensure the game exists
			if (game_status){
				sprintf(response, "GAME %s DOES NOT EXIST.", game_name);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}
			
			// Does the player belong to the game?
			if (strcmp(ttt->player->username, username) != 0){
				sprintf(response, "USER %s DOES NOT BELONG TO GAME %s.", username, game_name);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			sprintf(response, "%sPlayer: X\n", response);
			sprintf(response, "%sComputer: O\n", response);

			// Game over check
			if (!ttt->isActive){
				sprint_game(ttt, response);
				sprintf(response, "%sGAME OVER.", response);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// Validate move
			if (square == NULL){
				sprint_game(ttt, response);
				sprintf(response, "%sINVALID MOVE.", response);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}
			if (strlen(square) != 2){
				sprint_game(ttt, response);
				sprintf(response, "%sINVALID MOVE.", response);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// Decipher move
			int row = (int)*(square);
			int col = (int)*(square+1);
			row -= 48;
			col -= 48;

			// Make move
			if (make_move(ttt, row, col) < 0){
				sprint_game(ttt, response);
				sprintf(response, "%sINVALID MOVE.", response);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			ttt->tree = ttt->tree->children[row][col];

			// Game over check
			if (!ttt->isActive){
				sprint_game(ttt, response);
				sprintf(response, "%sGAME OVER.", response);
				write(client_sock , response , 2000);  
				close(client_sock);
				continue;
			}

			// CPU turn
			int move_found = 0;

			for (int i = 0; i < DIMENSION; i++){
				for (int j = 0; j < DIMENSION; j++){

					if (ttt->tree->children[i][j] != NULL){

						if (ttt->tree->children[i][j]->value == ttt->tree->value){
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
			make_move(ttt, row, col);
			ttt->tree = ttt->tree->children[row][col];
			sprint_game(ttt, response);

			// Game over check
			if (!ttt->isActive){
				sprintf(response, "%sGAME OVER.", response);
			}
		}

		write(client_sock , response , 2000);  
		close(client_sock);
	}
	close(socket_desc);	
	return 0;
}
