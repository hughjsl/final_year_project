#include <iostream>
#include <string>
#include <unordered_set>
#include <chrono>      
#include <random>    
#include <fstream>     
#include <ctime>      
#include "tic_tac_toe.h"
#include "minimax.h"
#include "qlearning.h"
#include "opponents.h"  


class TeeBuf : public std::streambuf
{
public:

    TeeBuf(std::streambuf* sb1, std::streambuf* sb2)
        : m_sb1(sb1), m_sb2(sb2)
    {
    }

protected:

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        auto put1 = m_sb1->sputn(s, n);
        auto put2 = m_sb2->sputn(s, n);
        return (put1 < put2) ? put1 : put2;
    }

    virtual int overflow(int c = EOF) override {
        if (c == EOF) {
            return !EOF;
        }
        int r1 = m_sb1->sputc(static_cast<char>(c));
        int r2 = m_sb2->sputc(static_cast<char>(c));
        return (r1 == EOF || r2 == EOF) ? EOF : c;
    }

private:
    std::streambuf* m_sb1;
    std::streambuf* m_sb2;
};


std::string encodeBoard(const Board& board) {

    std::string result;
    result.reserve(9);
    for (int y = 2; y >= 0; --y) {
        for (int x = 0; x < 3; ++x) {
            result.push_back(char('0' + board[y][x]));
        }
    }
    return result;
}

Move chooseMoveMinimax(TicTacToe& game, Minimax& mm, int player) {
    return mm.getBestMove(game, player);
}


Move chooseMoveQ(TicTacToe& game, QLearningAgent& agent, int player) {
    agent.setEpsilon(0.0);
    int action = agent.chooseAction(game);
    if (action < 0) {
        return {-1, -1};
    }
    int x, y;
    QLearningAgent::fromActionIndex(action, x, y);
    return {x, y};
}

int main() {

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    unsigned randNum = (std::rand() % 90000000) + 10000000; 
    std::string outFilename = "results/result_" + std::to_string(randNum) + ".txt";

    std::ofstream outFile(outFilename);
    if (!outFile) {
        std::cerr << "Warning: Could not open output file " << outFilename << " for writing!\n"
                  << "Continuing without file logging...\n";
    } else {
        static TeeBuf teeBuf(std::cout.rdbuf(), outFile.rdbuf());
        std::cout.rdbuf(&teeBuf); 
    }


    std::cout << "===== Tic-Tac-Toe Matchup =====\n\n"
              << "Logging to file: " << outFilename << "\n\n";

    std::cout << "Enter Player1 type:\n"
              << "   \"minimax\" => standard Minimax\n"
              << "   \"random\"  => random opponent\n"
              << "   \"buggy\"   => buggy Minimax\n"
              << "   \"buggy2\"  => buggy Minimax #2\n"
              << "   or provide a Q policy filename (e.g. \"q_policy.dat\")\n"
              << "Player1: ";
    std::string p1Choice;
    std::cin >> p1Choice;

    std::cout << "\nEnter Player2 type:\n"
              << "   \"minimax\" => standard Minimax\n"
              << "   \"random\"  => random opponent\n"
              << "   \"buggy\"   => buggy Minimax\n"
              << "   \"buggy2\"  => buggy Minimax #2\n"
              << "   or provide a Q policy filename (e.g. \"q_policy.dat\")\n"
              << "Player2: ";
    std::string p2Choice;
    std::cin >> p2Choice;

    std::cout << "\nHow many games to play?: ";
    int numGames;
    std::cin >> numGames;
    if (!std::cin.good() || numGames <= 0) {
        std::cerr << "Invalid number of games.\n";
        return 1;
    }

    static Minimax mmP1;       
    static Minimax mmP2;         
    static QLearningAgent qP1; 
    static QLearningAgent qP2; 

    Move (*p1MoveFn)(TicTacToe&, int) = nullptr;
    Move (*p2MoveFn)(TicTacToe&, int) = nullptr;

    if (p1Choice == "minimax") {
        p1MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm; 
            return chooseMoveMinimax(game, mm, player);
        };
    }
    else if (p1Choice == "random") {
        p1MoveFn = [](TicTacToe& game, int player) {
            return getRandomMove(game, player);
        };
    }
    else if (p1Choice == "buggy") {
        p1MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm;
            return getBuggyMinimaxMove(game, mm, player);
        };
    }
    else if (p1Choice == "buggy2") {
        p1MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm;
            return getBuggyMinimaxMove2(game, mm, player);
        };
    }
    else {
        qP1.loadPolicy(p1Choice);
        p1MoveFn = [](TicTacToe& game, int player) {
            static QLearningAgent& agent = qP1;
            return chooseMoveQ(game, agent, player);
        };
    }

    if (p2Choice == "minimax") {
        p2MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm;
            return chooseMoveMinimax(game, mm, player);
        };
    }
    else if (p2Choice == "random") {
        p2MoveFn = [](TicTacToe& game, int player) {
            return getRandomMove(game, player);
        };
    }
    else if (p2Choice == "buggy") {
        p2MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm;
            return getBuggyMinimaxMove(game, mm, player);
        };
    }
    else if (p2Choice == "buggy2") {
        p2MoveFn = [](TicTacToe& game, int player) {
            static Minimax mm;
            return getBuggyMinimaxMove2(game, mm, player);
        };
    }
    else {
        qP2.loadPolicy(p2Choice);
        p2MoveFn = [](TicTacToe& game, int player) {
            static QLearningAgent& agent = qP2;
            return chooseMoveQ(game, agent, player);
        };
    }

    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    int lastPercent = -1;

    std::unordered_set<std::string> finalStates;

    int winsP1 = 0;
    int winsP2 = 0;
    int draws  = 0;

    for (int g = 0; g < numGames; ++g) {
        int percent = int((g+1)*100.0 / numGames);
        if (percent > lastPercent) {
            lastPercent = percent;
            auto now = high_resolution_clock::now();
            double elapsedSec = duration_cast<duration<double>>(now - start).count();

            double fractionDone = double(g+1)/double(numGames);
            double estTotalTime = (fractionDone > 0.0)
                                  ? elapsedSec / fractionDone
                                  : 0.0;
            double remainSec = estTotalTime - elapsedSec;

            std::cout << "Match " << (g+1) << "/" << numGames << " ("
                      << percent << "%) completed. Elapsed: "
                      << elapsedSec/60 << "m, Remaining: ~"
                      << remainSec/60 << "m\n";
        }

        TicTacToe game;
        int currentPlayer = 1; 

        while (!game.isGameOver()) {
            Move m;
            if (currentPlayer == 1) {
                m = p1MoveFn(game, 1);
            } else {
                m = p2MoveFn(game, 2);
            }

            if (m.x < 0 || m.y < 0) {
                break;
            }
            game.makeMove(m.x, m.y, currentPlayer);

            if (game.isGameOver()) {
                int winner = game.checkWin();
                if (winner == 1) {
                    winsP1++;
                } else if (winner == 2) {
                    winsP2++;
                } else {
                    draws++;
                }
                break;
            }
            currentPlayer = 3 - currentPlayer;
        }

        if (!game.isGameOver()) {
            if (game.isFull() && game.checkWin() == 0) {
                draws++;
            }
        }

        finalStates.insert(encodeBoard(game.getBoard()));
    }


    std::cout << "\nResults after " << numGames << " games:\n";
    std::cout << "  Player1 wins: " << winsP1 << "\n";
    std::cout << "  Draws:        " << draws  << "\n";
    std::cout << "  Player2 wins: " << winsP2 << "\n";


    std::cout << "\nNumber of unique ending board states: " 
              << finalStates.size() << "\n";

    auto end = high_resolution_clock::now();
    double totalSec = duration_cast<duration<double>>(end - start).count();
    std::cout << "\nDone. Total time: " << totalSec/60 << "m\n";

    return 0;
}
