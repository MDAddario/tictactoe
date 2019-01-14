// Brute force solve an entire game of perfect information
int fullSolveAlgo(game*, node*, int, int, int);
/* 
 *  make_move() return codes:
 * -3: Illegal move (game inactive)
 * -2: Illegal move (incoherent move)
 * -1: Illegal move (move against rules)
 *  0: Legal move, game continues
 *  1: Game ends in a tie
 *  2: Player 1 wins
 *  3: Player 2 wins
 */

// Generate tree, calculate time, and print outcome
node* tree_generation(int isDebug);

// Free pointer and set to NULL
void free_ptr(int**);

// Play game against perfect opponent
void play_against_cpu(game* ttt, node* nd, int is_CPU_P1);
