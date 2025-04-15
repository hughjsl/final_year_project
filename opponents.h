#ifndef OPPONENTS_H
#define OPPONENTS_H

#include "tic_tac_toe.h"
#include "minimax.h"

Move getRandomMove(TicTacToe& game, int player);

Move getBuggyMinimaxMove(TicTacToe& game, Minimax& mm, int player);

Move getBuggyMinimaxMove2(TicTacToe& game, Minimax& mm, int player);

#endif
