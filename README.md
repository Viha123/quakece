# Chess Engine!
This is an attempt to make a Chess Engine that is UCI complient eventually, as well as a basic GUI that allows you to test and play the game. 
## How to build and run?
### To build:
- Ensure you have CMAKE and a g++ installed.
- Then run:
    -  ```./build.sh```
### To run and play against AI: 
```build/bin/main 1```
### To run and play against another player:
```build/bin/main 2```
### To debug:
- ```cd build```
- ```cmake -DCMAKE_BUILD_TYPE=Debug ..```
### To run in debug mode: (in dir outside of build)
- ```gdb --args build/bin/main 2```

### To run valgrind: 
- first compile then run with:
- ```valgrind --leak-check=full -v build/bin/main p```
### To profile: 
- cd build
- ```cmake -DENABLE_PROFILING=ON ..```
- run the profiler command:
```gprof build/bin/main gmon.out```
- turn the text output into graph form (assume gprof2dot and graphviz are installed):
```gprof build/bin/main gmon.out | gprof2dot -s -w | dot -Tpng -o output.png```

## Current Features:
- GUI player 1 vs player 2 and GUI player 1 vs Computer
- Mailbox Representation for the Board
- Complete set of perft tests 
- Perft 6 in around 14 seconds. 
- Basic Material and Mobility Evaluation
- Negamax and Alpha Beta Pruning Search. 
- Move ordering based on captures first that massively reduce search space of the Alpha beta pruning
- Piece Square Tables for the evaluation function
- Quiescence search
- Collect principal variation moves (for the non quiescence search)
- Basic Timer on the GUI. 
## Features To Add:
- Refactor code to better follow best pracitces
- Improve Endgame Evaluation. 
- UCI compatibility
- Transposition Tables ?
- Iterative Deapening and add timer logic
- MAYBE (bitboards and magic bitboards but that might require a huge code haul)

## Goals for Version 1:
- [x] Make a basic gui that represents all the pieces in SFML
- [x] basic class structure for internal and external representation of board
- [x] make sure all moves are implemented correctly
- [x] Test if all moves are implemented correctly using the test cases given in the wiki. 

## Task List for V1:
- [x] Make a test folder 
- [x] Test multiple fen strings

## Task List for V1.5:
- [x] Randomly choose moves allow a vs computer mode
- [x] Perform time tests for depth results and go over code to reduce speed because there are a lot of low hanging performance improvements that can be made to this. 

## Goals for Version 2:
- [x] implement more complex algorithms basic minimax or alpha beta pruning and move on from there. 
- [x] Basic Move Ordering (MVV-LVA) Most Valuable Victim - Least Valuable Aggressor
- [ ] Order based on promotions and checks
- [ ] Opening books (choose from a database of openings)
- [x] Piece Square Lists
- [x] Improve Endgame Evaluation.
- [x] Quiescence search
- [ ] Transposition tables
- [ ] Iterative deeping. Add timer management
- [ ] Improve GUI
- [x] checkmate acknolwedgement and Timer in GUI. 

## Resources used:
- https://sites.google.com/site/tscpchess/home?authuser=0
- https://www.chess.com/terms/fen-chess
- https://www.chessprogramming.org/Main_Page
## Inspired by:
- Tom Kerrigen's Simple Chess program
