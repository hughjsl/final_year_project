#include "qlearning.h"
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <cstdint>   
#include <ctime>
#include <algorithm>
#include <limits>
#include "minimax.h" 

QLearningAgent::QLearningAgent(double alpha_, double gamma_, double epsilon_)
    : alpha(alpha_), gamma(gamma_), epsilon(epsilon_)
{

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}


int QLearningAgent::toActionIndex(int x, int y) {
    return y * 3 + x;
}

void QLearningAgent::fromActionIndex(int action, int &x, int &y) {
    x = action % 3;
    y = action / 3;
}

std::string QLearningAgent::encodeBoard(const Board& board) const {
    std::string result;
    result.reserve(9);
    for (int y = 2; y >= 0; --y) {
        for (int x = 0; x < 3; ++x) {
            result.push_back(char('0' + board[y][x]));
        }
    }
    return result;
}

int QLearningAgent::chooseAction(const TicTacToe& game) {
    std::string stateStr = encodeBoard(game.getBoard());
    auto it = Q.find(stateStr);
    if (it == Q.end()) {
        Q[stateStr] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        it = Q.find(stateStr);
    }
    std::array<double, 9>& qvals = it->second;

    std::vector<int> validMoves;
    for (int action = 0; action < 9; ++action) {
        int x, y;
        fromActionIndex(action, x, y);
        if (game.isValidMove(x, y)) {
            validMoves.push_back(action);
        }
    }
    if (validMoves.empty()) {
        return -1;
    }

    double r = double(std::rand()) / RAND_MAX;
    if (r < epsilon) {
        int idx = std::rand() % validMoves.size();
        return validMoves[idx];
    } else {
        double bestVal = -std::numeric_limits<double>::infinity();
        int bestAction = validMoves[0];
        for (int a : validMoves) {
            if (qvals[a] > bestVal) {
                bestVal = qvals[a];
                bestAction = a;
            }
        }
        return bestAction;
    }
}

void QLearningAgent::updateQ(const std::string& stateStr, int action,
                             const std::string& nextStateStr,
                             double reward, bool terminal)
{
    if (Q.find(stateStr) == Q.end()) {
        Q[stateStr] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    double currentQ = Q[stateStr][action];

    double tdTarget;
    if (terminal) {
        tdTarget = reward;
    } else {
        if (Q.find(nextStateStr) == Q.end()) {
            Q[nextStateStr] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        }
        double bestNext = -std::numeric_limits<double>::infinity();
        for (double qv : Q[nextStateStr]) {
            if (qv > bestNext) {
                bestNext = qv;
            }
        }
        tdTarget = reward + gamma * bestNext;
    }
    double tdError = tdTarget - currentQ;
    Q[stateStr][action] += alpha * tdError;
}

void QLearningAgent::savePolicy(const std::string& filename) const {
    std::ofstream out(filename, std::ios::out | std::ios::binary);
    if (!out) {
        std::cerr << "could not open " << filename << " to write\n";
        return;
    }

    uint64_t size = Q.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (auto const &kv : Q) {
        const std::string& state = kv.first;
        const std::array<double, 9>& qvals = kv.second;

        uint64_t len = state.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));

        out.write(state.data(), len);

        out.write(reinterpret_cast<const char*>(qvals.data()), 9 * sizeof(double));
    }

    out.close();
    std::cout << "saved q-policy to " << filename << std::endl;
}

void QLearningAgent::loadPolicy(const std::string& filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "could not open " << filename << " for reading\n";
        return;
    }

    Q.clear();

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
    std::cout << "loaded q-policy: " << filename << std::endl;
}

void QLearningAgent::clearPolicy() {
    Q.clear();
}
