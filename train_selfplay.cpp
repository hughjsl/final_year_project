#include <iostream>
#include <string>
#include "tic_tac_toe.h"
#include "qlearning.h"


void trainSelfPlay(QLearningAgent& agent1,
                   QLearningAgent& agent2,
                   int episodes)
{
    for (int ep = 0; ep < episodes; ++ep)
    {
        TicTacToe env;
        int currentPlayer = 1; 

        while (!env.isGameOver())
        {

            QLearningAgent& currentAgent = (currentPlayer == 1) ? agent1 : agent2;

            std::string stateStr = currentAgent.encodeBoard(env.getBoard());
            int action = currentAgent.chooseAction(env);
            if (action < 0) {
                break; 
            }

            int x, y;
            QLearningAgent::fromActionIndex(action, x, y);

            env.makeMove(x, y, currentPlayer);

            if (env.isGameOver())
            {
                int winner = env.checkWin();
                double reward = 0.0;
                if (winner == currentPlayer) {
                    reward = 1.0;  
                } else if (winner != 0) {
                    reward = -1.0;  
                }

                currentAgent.updateQ(stateStr, action, /*nextState=*/"", reward, /*terminal=*/true);
                break;
            }
            else
            {
                std::string nextState = currentAgent.encodeBoard(env.getBoard());
                currentAgent.updateQ(stateStr, action, nextState, 0.0, /*terminal=*/false);
            }

            currentPlayer = 3 - currentPlayer; 
        }
    }
    std::cout << "Finished self-play training of " << episodes << " episodes.\n";
}


void playMatches(QLearningAgent& agent1,
                 QLearningAgent& agent2,
                 int numGames)
{

    agent1.setEpsilon(0.0);
    agent2.setEpsilon(0.0);

    int winsP1 = 0;
    int winsP2 = 0;
    int draws  = 0;

    for (int g = 0; g < numGames; ++g)
    {
        TicTacToe env;
        int currentPlayer = 1;

        while (!env.isGameOver())
        {
            QLearningAgent& currentAgent = (currentPlayer == 1) ? agent1 : agent2;

            int action = currentAgent.chooseAction(env);
            if (action < 0) {
                break;
            }

            int x, y;
            QLearningAgent::fromActionIndex(action, x, y);
            env.makeMove(x, y, currentPlayer);

            if (env.isGameOver())
            {
                int winner = env.checkWin();
                if (winner == 1) ++winsP1;
                else if (winner == 2) ++winsP2;
                else                  ++draws;  
                break;
            }

            currentPlayer = 3 - currentPlayer;
        }
    }

    std::cout << "After " << numGames << " matches:\n"
              << "  Player1 wins: " << winsP1 << "\n"
              << "  Player2 wins: " << winsP2 << "\n"
              << "  Draws:         " << draws  << "\n";
}

int main()
{
    std::cout << "Self-Play Q-Learning Demo\n\n";

    int episodes = 0;
    std::cout << "Enter the number of self-play training episodes: ";
    std::cin >> episodes;
    if (!std::cin.good() || episodes < 0) {
        std::cerr << "Invalid number.\n";
        return 1;
    }

    QLearningAgent agent1(0.1, 1.0, 0.2);
    QLearningAgent agent2(0.1, 1.0, 0.2);

    if (episodes > 0) {

        trainSelfPlay(agent1, agent2, episodes);

        agent1.savePolicy("player1_policy.dat");
        agent2.savePolicy("player2_policy.dat");
    }
    else {
        std::cout << "Loading existing policies...\n";
        agent1.loadPolicy("player1_policy.dat");
        agent2.loadPolicy("player2_policy.dat");
    }

    std::cout << "\nHow many matches to run with these policies? ";
    int matches = 0;
    std::cin >> matches;
    if (!std::cin.good() || matches < 0) {
        std::cerr << "Invalid number.\n";
        return 1;
    }

    if (matches > 0) {
        playMatches(agent1, agent2, matches);
    }

    std::cout << "Done.\n";
    return 0;
}
