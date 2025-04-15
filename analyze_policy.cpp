#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdint>
#include <cmath>

#include "tic_tac_toe.h"
#include "minimax.h"
#include "qlearning.h"

// return other player
static int otherPlayer(int p) {
    return (p == 1) ? 2 : 1;
}

// returns all optimal moves from given state according to minimax
std::vector<Move> getAllOptimalMoves(TicTacToe& game, Minimax& mm, int player) {
    double bestScore = -1.0; 
    std::vector<Move> bestMoves;

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (game.isValidMove(x, y)) {
                game.makeMove(x, y, player);

                double oppScore = mm.scorePosition(game, otherPlayer(player));
                double myScore = 1.0 - oppScore;
                game.undoMove(x, y);

                if (myScore > bestScore) {
                    bestScore = myScore;
                    bestMoves.clear();
                    bestMoves.push_back({x, y});
                } else if (std::fabs(myScore - bestScore) < 1e-12) {

                    bestMoves.push_back({x, y});
                }
            }
        }
    }
    return bestMoves;
}

Board decodeBoard(const std::string& stateStr) {

    Board board(3, std::vector<int>(3, 0));
    if (stateStr.size() < 9) {
        return board; // safe fallback
    }

    int idx = 0;
    for (int y = 2; y >= 0; --y) {
        for (int x = 0; x < 3; ++x) {
            char c = stateStr[idx++];
            board[y][x] = (c - '0');
        }
    }
    return board;
}


bool isNonTerminal(const Board& board) {
    TicTacToe game;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] != 0) {
                game.makeMove(x, y, board[y][x]);
            }
        }
    }
    if (game.isGameOver()) {
        return false;
    }
    int count1 = 0;
    int count2 = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == 1) count1++;
            if (board[y][x] == 2) count2++;
        }
    }

    if (count1 == count2 || count1 == (count2 + 1)) {
        return true;
    }
    return false;
}

int findNextPlayer(const Board& board) {
    int count1 = 0;
    int count2 = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == 1) count1++;
            if (board[y][x] == 2) count2++;
        }
    }
    return (count1 == count2) ? 1 : 2;
}


void analyzePolicyFile(const std::string& filename, Minimax& mm) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "Could not open " << filename << " for reading.\n";
        return;
    }

    std::unordered_map<std::string, std::array<double, 9>> Q;
    uint64_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (uint64_t i = 0; i < size; i++) {
        uint64_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string state(len, '\0');
        in.read(&state[0], len);

        std::array<double, 9> qvals;
        in.read(reinterpret_cast<char*>(qvals.data()), 9 * sizeof(double));

        Q[state] = qvals;
    }
    in.close();

    std::cout << "\nAnalyzing " << filename << " ... loaded " << Q.size() << " states\n";

    int totalStates = 0;
    int matchCount  = 0;
    int mismatchCount = 0;
    double sumDiff      = 0.0;  
    double sumAbsDiff   = 0.0;

    for (auto& kv : Q) {
        const std::string& stateStr = kv.first;
        const std::array<double, 9>& qvals = kv.second;

        Board board = decodeBoard(stateStr);

        if (!isNonTerminal(board)) {
            continue;
        }

        TicTacToe game;
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (board[y][x] != 0) {
                    game.makeMove(x, y, board[y][x]);
                }
            }
        }

        int nextPlayer = findNextPlayer(board);

        double bestQVal = -std::numeric_limits<double>::infinity();
        int bestAction = -1;
        for (int a = 0; a < 9; a++) {
            int ax = a % 3;
            int ay = a / 3;
            if (game.isValidMove(ax, ay)) {
                if (qvals[a] > bestQVal) {
                    bestQVal = qvals[a];
                    bestAction = a;
                }
            }
        }
        if (bestAction < 0) {
            continue;
        }
        int qMoveX = bestAction % 3;
        int qMoveY = bestAction / 3;

        std::vector<Move> mmMoves = getAllOptimalMoves(game, mm, nextPlayer);

        totalStates++;

        bool isMatch = false;
        for (auto& m : mmMoves) {
            if (m.x == qMoveX && m.y == qMoveY) {
                isMatch = true;
                break;
            }
        }
        if (isMatch) {
            matchCount++;
        } else {
            mismatchCount++;

            TicTacToe qGame = game;
            qGame.makeMove(qMoveX, qMoveY, nextPlayer);
            double qPayoff = mm.scorePosition(qGame, nextPlayer);

            Move mmRep = mmMoves[0];
            TicTacToe mGame = game;
            mGame.makeMove(mmRep.x, mmRep.y, nextPlayer);
            double mmPayoff = mm.scorePosition(mGame, nextPlayer);

            double diff = qPayoff - mmPayoff;
            sumDiff    += diff;
            sumAbsDiff += std::fabs(diff);
        }
    }

    std::cout << "  Total non-terminal states in Q: " << totalStates << "\n";
    std::cout << "  Agreement with Minimax eq-class: " << matchCount << "\n";
    std::cout << "  Mismatches: " << mismatchCount << "\n";
    if (mismatchCount > 0) {
        double avgDiff    = sumDiff / mismatchCount;
        double avgAbsDiff = sumAbsDiff / mismatchCount;
        std::cout << "  avg(QPayoff - MinimaxPayoff) over mismatches: " << avgDiff << "\n";
        std::cout << "  avg absolute difference: " << avgAbsDiff << "\n";
    }
}

int main() {
    std::cout << "Extended Analyze Policy: compares Q’s single best move vs. Minimax’s entire equivalence class.\n";

    Minimax mm;

    analyzePolicyFile("q_policy.dat", mm);
    analyzePolicyFile("player1_policy.dat", mm);
    analyzePolicyFile("player2_policy.dat", mm);
    analyzePolicyFile("q_policy_random.dat", mm);
    analyzePolicyFile("q_policy_buggy.dat", mm);
    analyzePolicyFile("q_policy_buggy2.dat", mm);

    std::cout << "\nDone.\n";
    return 0;
}
