#ifndef MINIMAX_H
#define MINIMAX_H

#include "tic_tac_toe.h"
#include <vector>

struct Move {
    int x;
    int y;
};


// config variable to randomise between set of equivalent moves
// if false, first move from equivalence class of optimal moves is chosen
// arbitary but deterministic
static bool s_randomizeEquivalentMoves = true;
static bool printEquivalenceSet = false;


//
//   scorePosition(game, player):
//     if the board is terminal:
//       if winner == player => 1.0
//       else if winner == 0 => 0.5 (draw)
//       else => 0.0
//     else:
//       // player's turn
//       // consider all valid moves => pick the one that forces
//       // the minimum payoff for the other player
//       // then the player's payoff = 1.0 - other player's min payoff
//
class Minimax {
public:
    // returns best move for player, when they are next to move
    Move getBestMove(TicTacToe& game, int player);

    // returns current players best guaranteed payoff
    double scorePosition(TicTacToe& game, int player);

private:

    // return the other player
    int otherPlayer(int p) {
        return (p == 1) ? 2 : 1;
    }
};

#endif
