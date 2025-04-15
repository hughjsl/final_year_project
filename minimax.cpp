#include "minimax.h"
#include <algorithm>
#include <limits>
#include <iostream>   
#include <random>   
#include <ctime>      



double Minimax::scorePosition(TicTacToe& game, int player) {

    if (game.isGameOver()) {
        int winner = game.checkWin();
        if (winner == player) return 1.0; 
        if (winner == 0)      return 0.5;  
        return 0.0;                      
    }

    int opp = otherPlayer(player);

    double bestOtherPayoff = std::numeric_limits<double>::infinity();

    bool anyMoves = false;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (game.isValidMove(x, y)) {
                anyMoves = true;
                game.makeMove(x, y, player);

                double oppScore = scorePosition(game, opp);

                bestOtherPayoff = std::min(bestOtherPayoff, oppScore);

                game.undoMove(x, y);
            }
        }
    }

    if (!anyMoves) {
        return 0.5;
    }

    return 1.0 - bestOtherPayoff;
}

Move Minimax::getBestMove(TicTacToe& game, int player) {
    double bestScore = -1.0; 
    std::vector<Move> bestMoves;

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (game.isValidMove(x, y)) {
                game.makeMove(x, y, player);

                double oppScore = scorePosition(game, otherPlayer(player));
                double myScore  = 1.0 - oppScore;

                game.undoMove(x, y);

                if (myScore > bestScore) {
                    bestScore = myScore;
                    bestMoves.clear();
                    bestMoves.push_back({x, y});
                }
                else if (myScore == bestScore) {
                    bestMoves.push_back({x, y});
                }
            }
        }
    }

    
    if (printEquivalenceSet) {
        std::cout << bestMoves.size()
              << " equivalent moves found, achieving guaranteed payoff = "
              << bestScore
              << std::endl;
    }

    if (s_randomizeEquivalentMoves && bestMoves.size() > 1) {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
        int idx = std::rand() % bestMoves.size();
        return bestMoves[idx];
    } else {
        return bestMoves[0];
    }
}
