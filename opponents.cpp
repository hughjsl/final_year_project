#include "opponents.h"
#include <vector>
#include <cstdlib>   
#include <ctime>
#include <algorithm>
#include <iostream>

static bool g_seeded = false;
static void seedRandomOnce()
{
    if (!g_seeded) {

        std::srand(123456);
        g_seeded = true;
    }
}

Move getRandomMove(TicTacToe& game, int player)
{
    seedRandomOnce();

    std::vector<Move> validMoves;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (game.isValidMove(x, y)) {
                validMoves.push_back({x, y});
            }
        }
    }
    if (validMoves.empty()) {
        return {-1, -1}; 
    }

    int idx = std::rand() % validMoves.size();
    return validMoves[idx];
}

static bool isSpecialBoard(const Board& board, int player)
{
    int other = (player == 1) ? 2 : 1;

    if (board[0][2] == other && board[2][2] == other && board[1][2] == 0) {
        return true;
    }
    return false;
}

Move getBuggyMinimaxMove(TicTacToe& game, Minimax& mm, int player)
{
    if (isSpecialBoard(game.getBoard(), player)) {

        if (game.isValidMove(0,0)) {
            return {0, 0};
        } else if (game.isValidMove(1,1)) {
            return {1,1};
        }
        return getRandomMove(game, player);
    }
    return mm.getBestMove(game, player);
}


static bool isSpecialBoard2(const Board& board, int player)
{

    int other = (player == 1) ? 2 : 1;
    if (board[2][0] == other && board[2][1] == 0 && board[2][2] == other
        && board[1][1] == player) {
        return true;
    }
    return false;
}

Move getBuggyMinimaxMove2(TicTacToe& game, Minimax& mm, int player)
{
    if (isSpecialBoard2(game.getBoard(), player)) {
        // std::cout << "buggy2 special case triggered\n";
        return getRandomMove(game, player);
    }
    return mm.getBestMove(game, player);
}
