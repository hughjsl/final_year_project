@echo off
echo Building analyze_policy...
g++ -o analyze_policy analyze_policy.cpp qlearning.cpp minimax.cpp tic_tac_toe.cpp

echo Building tic_tac_toe...
g++ -o tic_tac_toe main.cpp tic_tac_toe.cpp minimax.cpp qlearning.cpp opponents.cpp

echo Building train_selfplay...
g++ -o train_selfplay tic_tac_toe.cpp qlearning.cpp minimax.cpp train_selfplay.cpp

echo Building matchup...
g++ -o matchup matchup.cpp tic_tac_toe.cpp minimax.cpp qlearning.cpp opponents.cpp

echo All builds completed
pause