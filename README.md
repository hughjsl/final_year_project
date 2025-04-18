Build instructions - Windows

Install a recent version of MinGW / GCC and include g++ on PATH
Open a terminal in the src directory
Run build.bat. This will build all .exe files involved in this project

tic_tac_toe.exe - human interactive game environment, allows for play between multiple different player type

train_selfplay.exe - trains two self-play agents to play against each other, produces policies playr1_policy.dat and player2_policy.dat

analyze_policy.exe - performs analysis of all .dat files in the src directory, returns proportion of states where policy is optimal according to minimax

matchup.exe - for batch simulation between any two agents, input a .dat file or specify a hard-coded opponent
