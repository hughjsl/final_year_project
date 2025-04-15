#include "tic_tac_toe.h"
#include <iostream>

TicTacToe::TicTacToe()
    : board(3, std::vector<int>(3, 0))
{
    // initialize 3x3 grid of 0
}

void TicTacToe::printBoard() const {
    std::cout << "\n";
    for (int y = 2; y >= 0; --y) {
        for (int x = 0; x < 3; ++x) {
            char symbol = '.';
            if (board[y][x] == 1) symbol = '1'; 
            else if (board[y][x] == 2) symbol = '2';

            std::cout << " " << symbol;
            if (x < 2) std::cout << " |";
        }
        std::cout << "\n";
        if (y > 0) {
            std::cout << "-----------\n";
        }
    }
    std::cout << "\n";
}

bool TicTacToe::makeMove(int x, int y, int player) {
    // validate co-ords
    if (x < 0 || x > 2 || y < 0 || y > 2) return false;
    if (board[y][x] != 0) return false;

    // enter value
    board[y][x] = player;
    return true;
}

void TicTacToe::undoMove(int x, int y) {
    board[y][x] = 0;
}

bool TicTacToe::isValidMove(int x, int y) const {
    if (x < 0 || x > 2 || y < 0 || y > 2) return false;
    return (board[y][x] == 0);
}

int TicTacToe::checkWin() const {
    // check rows
    for (int y = 0; y < 3; ++y) {
        if (board[y][0] != 0 &&
            board[y][0] == board[y][1] &&
            board[y][1] == board[y][2]) {
            return board[y][0];
        }
    }
    // check cols
    for (int x = 0; x < 3; ++x) {
        if (board[0][x] != 0 &&
            board[0][x] == board[1][x] &&
            board[1][x] == board[2][x]) {
            return board[0][x];
        }
    }
    // check diagonals
    if (board[1][1] != 0) {
        if (board[0][0] == board[1][1] &&
            board[1][1] == board[2][2]) {
            return board[1][1];
        }
        if (board[2][0] == board[1][1] &&
            board[1][1] == board[0][2]) {
            return board[1][1];
        }
    }

    return 0; // no winner
}

bool TicTacToe::isFull() const {
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            if (board[y][x] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool TicTacToe::isGameOver() const {
    // end game if board is full, or no winner
    if (checkWin() != 0) {
        return true;
    }
    if (isFull()) {
        return true;
    }
    return false;
}
