#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <vector>

// 0 = empty, 1 = player1, 2 = player2
typedef std::vector<std::vector<int>> Board;

class TicTacToe {
public:
    TicTacToe();

    // prints the current board state
    //   bottom left is (0, 0)
    //   uses '1' and '2' to indicate player pieces, '.' for empty
    void printBoard() const;

    // place player piee at given co-ords if valid
    bool makeMove(int x, int y, int player);

    // resets position to 0, used in minimax
    void undoMove(int x, int y);

    // checks co-ords are valid and space is not occupied
    bool isValidMove(int x, int y) const;

    // returns winning player or 0
    int checkWin() const;

    // returns true if board is full, else 0
    bool isFull() const;

    // returns true if game is over, else 0
    bool isGameOver() const;

    const Board& getBoard() const { return board; }

private:
    Board board;
};

#endif
