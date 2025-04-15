#ifndef QLEARNING_H
#define QLEARNING_H

#include <string>
#include <unordered_map>
#include <array>
#include "tic_tac_toe.h"

// each board state stored as string, along with 9 q-values for 9 possible moves
class QLearningAgent {
public:
    QLearningAgent(double alpha=0.1, double gamma=1.0, double epsilon=0.2);

    // epsilon-greedy, returns action or -1
    int chooseAction(const TicTacToe& game);

    // q-learning update
    //   Q(s,a) <- Q(s,a) + alpha [ r + gamma * max_a'( Q(s', a') ) - Q(s,a) ]
    void updateQ(const std::string& stateStr, int action,
                 const std::string& nextStateStr,
                 double reward, bool terminal);

    // convert (x, y) to [0...8] or vice-versa
    static int toActionIndex(int x, int y);
    static void fromActionIndex(int action, int &x, int &y);

    void savePolicy(const std::string& filename) const;

    void loadPolicy(const std::string& filename);

    void clearPolicy();

    void setEpsilon(double e) { epsilon = e; }

    std::string encodeBoard(const Board& board) const;

private:
    std::unordered_map<std::string, std::array<double, 9>> Q;

    // hyperparameters
    double alpha;  
    double gamma;  
    double epsilon; 
};

#endif
