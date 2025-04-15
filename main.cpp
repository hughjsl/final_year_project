#include <iostream>
#include <string>
#include <chrono>

#include "tic_tac_toe.h"
#include "minimax.h"
#include "qlearning.h"
#include "opponents.h"

void trainQAgent(QLearningAgent& agent, Minimax& minimaxPlayer, int episodes) {
    using namespace std::chrono; 
    auto start = high_resolution_clock::now(); 

    for (int ep = 0; ep < episodes; ep++) {
        TicTacToe env;
        while (!env.isGameOver()) {
            std::string stateStr = agent.encodeBoard(env.getBoard());
            int action = agent.chooseAction(env);
            if (action < 0) break;

            int ax, ay;
            QLearningAgent::fromActionIndex(action, ax, ay);
            env.makeMove(ax, ay, 1);

            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0; 
                else if (winner == 2) r = -1.0; 
                agent.updateQ(stateStr, action, "", r, true);
                break;
            }

            Move oppMove = minimaxPlayer.getBestMove(env, 2);
            env.makeMove(oppMove.x, oppMove.y, 2);

            std::string nextState = agent.encodeBoard(env.getBoard());
            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, nextState, r, true);
            } else {
                agent.updateQ(stateStr, action, nextState, 0.0, false);
            }
        }

        if ((ep + 1) % 1000 == 0) {
            auto now = high_resolution_clock::now();
            auto elapsedDuration = std::chrono::duration_cast<std::chrono::duration<double>>(now - start);
            double seconds = elapsedDuration.count();
            double estimatedTotalTime = seconds / (ep + 1) * episodes;
            double timeRemaining = estimatedTotalTime - seconds;

            std::cout << "Episode " << ep + 1 << "/" << episodes << " completed. ";
            std::cout << "Elapsed time: " << seconds
                      << "s, Estimated remaining time: "
                      << timeRemaining/60 << "min\n";
        }
    }

    std::cout << "Finished training " << episodes << " episodes.\n";
}

void trainQAgentVsRandom(QLearningAgent& agent, int episodes) {
    using namespace std::chrono; 
    auto start = high_resolution_clock::now(); 

    for (int ep = 0; ep < episodes; ep++) {
        TicTacToe env;
        while (!env.isGameOver()) {
            std::string stateStr = agent.encodeBoard(env.getBoard());
            int action = agent.chooseAction(env);
            if (action < 0) break;

            int ax, ay;
            QLearningAgent::fromActionIndex(action, ax, ay);
            env.makeMove(ax, ay, 1);

            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, "", r, true);
                break;
            }

            Move oppMove = getRandomMove(env, 2);
            env.makeMove(oppMove.x, oppMove.y, 2);

            std::string nextState = agent.encodeBoard(env.getBoard());
            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, nextState, r, true);
            } else {
                agent.updateQ(stateStr, action, nextState, 0.0, false);
            }
        }

        if ((ep + 1) % 1000 == 0) {
            auto now = high_resolution_clock::now();
            auto elapsedDuration = std::chrono::duration_cast<std::chrono::duration<double>>(now - start);
            double seconds = elapsedDuration.count();
            double estimatedTotalTime = seconds / (ep + 1) * episodes;
            double timeRemaining = estimatedTotalTime - seconds;

            std::cout << "Episode " << ep + 1 << "/" << episodes << " completed. ";
            std::cout << "Elapsed time: " << seconds
                      << "s, Estimated remaining time: "
                      << timeRemaining/60 << "min\n";
        }
    }

    std::cout << "Finished training " << episodes << " episodes.\n";
}

void trainQAgentVsBuggy(QLearningAgent& agent, int episodes) {
    using namespace std::chrono; 
    auto start = high_resolution_clock::now(); 
    static Minimax buggyMm; 

    for (int ep = 0; ep < episodes; ep++) {
        TicTacToe env;
        while (!env.isGameOver()) {
            std::string stateStr = agent.encodeBoard(env.getBoard());
            int action = agent.chooseAction(env);
            if (action < 0) break;

            int ax, ay;
            QLearningAgent::fromActionIndex(action, ax, ay);
            env.makeMove(ax, ay, 1);

            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, "", r, true);
                break;
            }

            Move oppMove = getBuggyMinimaxMove(env, buggyMm, 2);
            env.makeMove(oppMove.x, oppMove.y, 2);

            std::string nextState = agent.encodeBoard(env.getBoard());
            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, nextState, r, true);
            } else {
                agent.updateQ(stateStr, action, nextState, 0.0, false);
            }
        }

        if ((ep + 1) % 1000 == 0) {
            auto now = high_resolution_clock::now();
            auto elapsedDuration = std::chrono::duration_cast<std::chrono::duration<double>>(now - start);
            double seconds = elapsedDuration.count();
            double estimatedTotalTime = seconds / (ep + 1) * episodes;
            double timeRemaining = estimatedTotalTime - seconds;

            std::cout << "Episode " << ep + 1 << "/" << episodes << " completed. ";
            std::cout << "Elapsed time: " << seconds
                      << "s, Estimated remaining time: "
                      << timeRemaining/60 << "min\n";
        }
    }

    std::cout << "Finished training " << episodes << " episodes.\n";
}

void trainQAgentVsBuggy2(QLearningAgent& agent, int episodes) {
    using namespace std::chrono; 
    auto start = high_resolution_clock::now(); 
    static Minimax mmForBuggy2;

    for (int ep = 0; ep < episodes; ep++) {
        TicTacToe env;
        while (!env.isGameOver()) {
            std::string stateStr = agent.encodeBoard(env.getBoard());
            int action = agent.chooseAction(env);
            if (action < 0) break;

            int ax, ay;
            QLearningAgent::fromActionIndex(action, ax, ay);
            env.makeMove(ax, ay, 1);

            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, "", r, true);
                break;
            }

            Move oppMove = getBuggyMinimaxMove2(env, mmForBuggy2, 2);
            env.makeMove(oppMove.x, oppMove.y, 2);

            std::string nextState = agent.encodeBoard(env.getBoard());
            if (env.isGameOver()) {
                int winner = env.checkWin();
                double r = 0.0;
                if (winner == 1) r = 1.0;
                else if (winner == 2) r = -1.0;
                agent.updateQ(stateStr, action, nextState, r, true);
            } else {
                agent.updateQ(stateStr, action, nextState, 0.0, false);
            }
        }

        if ((ep + 1) % 1000 == 0) {
            auto now = high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - start).count();
            double estTotal = elapsed / (ep + 1) * episodes;
            double timeRem = estTotal - elapsed;
            std::cout << "Episode " << ep + 1 << "/" << episodes << " completed. "
                      << "Elapsed time: " << elapsed
                      << "s, Estimated remaining time: "
                      << timeRem/60 << "min\n";
        }
    }

    std::cout << "Finished training " << episodes << " episodes.\n";
}


Move qLearningGetBestMove(TicTacToe& game, QLearningAgent& agent, int player) {
    int action = agent.chooseAction(game);
    if (action < 0) {
        return { -1, -1 };
    }
    int x, y;
    QLearningAgent::fromActionIndex(action, x, y);
    return { x, y };
}

int main() {
    std::cout << "Tic-Tac-Toe\n";
    std::cout << "Select an option:\n";
    std::cout << "   0 => Normal game setup\n";
    std::cout << "   1 => Train Q-learning agent vs Minimax  (output: q_policy.dat)\n";
    std::cout << "   2 => Train Q-learning agent vs Random   (output: q_policy_random.dat)\n";
    std::cout << "   3 => Train Q-learning agent vs BuggyMinimax (output: q_policy_buggy.dat)\n";
    std::cout << "   4 => Train Q-learning agent vs BuggyMinimax2 (output: q_policy_buggy2.dat)\n";
    int choice;
    std::cin >> choice;

    if (!std::cin.good()) {
        std::cerr << "Invalid selection.\n";
        return 1;
    }

    if (choice == 1) {
        std::cout << "Training Q-learning agent vs. Minimax...\n";
        QLearningAgent agent(0.1, 1.0, 0.2); // alpha=0.1, gamma=1.0, epsilon=0.2
        Minimax minimaxPlayer;

        int episodes;
        std::cout << "How many training episodes?: ";
        std::cin >> episodes;

        trainQAgent(agent, minimaxPlayer, episodes);

        agent.savePolicy("q_policy.dat");
        std::cout << "Training complete. Policy saved to q_policy.dat.\n";
        return 0;
    }
    else if (choice == 2) {
        // Train QLearning model vs. Random
        std::cout << "Training Q-learning agent vs. Random...\n";
        QLearningAgent agent(0.1, 1.0, 0.2);

        int episodes;
        std::cout << "How many training episodes?: ";
        std::cin >> episodes;

        trainQAgentVsRandom(agent, episodes);

        agent.savePolicy("q_policy_random.dat");
        std::cout << "Training complete. Policy saved to q_policy_random.dat.\n";
        return 0;
    }
    else if (choice == 3) {
        // Train QLearning model vs. Buggy Minimax
        std::cout << "Training Q-learning agent vs. Buggy Minimax...\n";
        QLearningAgent agent(0.1, 1.0, 0.2);

        int episodes;
        std::cout << "How many training episodes?: ";
        std::cin >> episodes;

        trainQAgentVsBuggy(agent, episodes);

        agent.savePolicy("q_policy_buggy.dat");
        std::cout << "Training complete. Policy saved to q_policy_buggy.dat.\n";
        return 0;
    }
    else if (choice == 4) {
        // Train QLearning model vs. Buggy2
        std::cout << "Training Q-learning agent vs. Buggy Minimax2...\n";
        QLearningAgent agent(0.1, 1.0, 0.2);

        int episodes;
        std::cout << "How many training episodes?: ";
        std::cin >> episodes;

        trainQAgentVsBuggy2(agent, episodes);

        agent.savePolicy("q_policy_buggy2.dat");
        std::cout << "Training complete. Policy saved to q_policy_buggy2.dat.\n";
        return 0;
    }

    std::cout << "Choose Player 1 type:\n"
              << "   0 => human\n"
              << "   1 => minimax\n"
              << "   2 => qlearning\n"
              << "   3 => random\n"
              << "   4 => buggyMinimax\n"
              << "   5 => buggyMinimax2\n";
    int p1Type;
    std::cin >> p1Type;

    std::cout << "Choose Player 2 type:\n"
              << "   0 => human\n"
              << "   1 => minimax\n"
              << "   2 => qlearning\n"
              << "   3 => random\n"
              << "   4 => buggyMinimax\n"
              << "   5 => buggyMinimax2\n";
    int p2Type;
    std::cin >> p2Type;

    QLearningAgent qAgent(0.1, 1.0, 0.0); 
    if (p1Type == 2 || p2Type == 2) {
        qAgent.loadPolicy("q_policy.dat"); 
    }

    TicTacToe game;
    Minimax minimaxPlayer;
    int currentPlayer = 1;
    game.printBoard();

    while (true) {
        if (game.isGameOver()) {
            int winner = game.checkWin();
            if (winner) {
                std::cout << "Player " << winner << " wins\n";
            } else {
                std::cout << "Draw\n";
            }
            break;
        }

        int pType = (currentPlayer == 1) ? p1Type : p2Type;
        Move moveChosen{-1, -1};

        switch (pType) {
            case 0: {
                bool validMove = false;
                while (!validMove) {
                    int x, y;
                    std::cout << "Player " << currentPlayer << " enter (x,y): ";
                    std::cin >> x >> y;
                    if (!std::cin.good()) {
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                        std::cout << "Invalid input\n";
                        continue;
                    }
                    if (game.isValidMove(x, y)) {
                        game.makeMove(x, y, currentPlayer);
                        validMove = true;
                        moveChosen = {x, y};
                    } else {
                        std::cout << "Invalid move. Try again.\n";
                    }
                }
                break;
            }
            case 1: {
                moveChosen = minimaxPlayer.getBestMove(game, currentPlayer);
                game.makeMove(moveChosen.x, moveChosen.y, currentPlayer);
                std::cout << "Minimax player " << currentPlayer
                          << " placed at (" << moveChosen.x << "," << moveChosen.y << ")\n";
                break;
            }
            case 2: {
                moveChosen = qLearningGetBestMove(game, qAgent, currentPlayer);
                if (moveChosen.x < 0) {
                    std::cout << "Q-learning player has no valid move\n";
                } else {
                    game.makeMove(moveChosen.x, moveChosen.y, currentPlayer);
                    std::cout << "Q-learning player " << currentPlayer
                              << " placed at (" << moveChosen.x << "," << moveChosen.y << ")\n";
                }
                break;
            }
            case 3: {
                moveChosen = getRandomMove(game, currentPlayer);
                if (moveChosen.x < 0) {
                    std::cout << "Random player has no valid move\n";
                } else {
                    game.makeMove(moveChosen.x, moveChosen.y, currentPlayer);
                    std::cout << "Random player " << currentPlayer
                              << " placed at (" << moveChosen.x << "," << moveChosen.y << ")\n";
                }
                break;
            }
            case 4: {
                moveChosen = getBuggyMinimaxMove(game, minimaxPlayer, currentPlayer);
                if (moveChosen.x < 0) {
                    std::cout << "BuggyMinimax found no valid move\n";
                } else {
                    game.makeMove(moveChosen.x, moveChosen.y, currentPlayer);
                    std::cout << "BuggyMinimax player " << currentPlayer
                              << " placed at (" << moveChosen.x << "," << moveChosen.y << ")\n";
                }
                break;
            }
            case 5: {
                moveChosen = getBuggyMinimaxMove2(game, minimaxPlayer, currentPlayer);
                if (moveChosen.x < 0) {
                    std::cout << "BuggyMinimax2 found no valid move\n";
                } else {
                    game.makeMove(moveChosen.x, moveChosen.y, currentPlayer);
                    std::cout << "BuggyMinimax2 player " << currentPlayer
                              << " placed at (" << moveChosen.x << "," << moveChosen.y << ")\n";
                }
                break;
            }
            default:
                std::cerr << "Invalid player type!\n";
                return 1;
        }

        game.printBoard();
        currentPlayer = 3 - currentPlayer;
    }

    return 0;
}
