// Struct for node in tic tac toe game tree
typedef struct tree_node_struct{

	int value;
	int isTerminal;
	struct tree_node_struct* children[DIMENSION][DIMENSION];

} node;

// Struct for entire tic tac toe game
typedef struct ttt_game_struct{

	int ply;
	int isActive;
	int x_array[DIMENSION][DIMENSION];
	int o_array[DIMENSION][DIMENSION];

} game;

// Initialize all game struct parameters
game* game_init();

// Perform a move and return outcome code, does not increment ply
int make_move(game*, int, int);
/* Return codes:
 * -3: Illegal move (game inactive)
 * -2: Illegal move (out of bounds)
 * -1: Illegal move (tile already occupied)
 *  0: Legal move, game continues
 *  1: Game ends in a tie
 *  2: Player 1 wins (x victory)
 *  3: Player 2 wins (o victory)
 */

// Check to see if game has ended, increment ply, update game state
int check_end(game*);
/* Return codes:
 *  0: Game continues
 *  1: Game tie
 *  2: Player 1 wins (x victory)
 *  3: Player 2 wins (o victory)
 */

// Revoke a move, decrement ply
void rollback(game*, int, int);

// Print the board to the terminal for debugging purposes
void print_game(game*);

// Initialize game tree node attributes
node* node_init();
