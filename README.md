# Chess Engine!
This is an attempt to make a Chess Engine that is UCI complient eventually, as well as a basic GUI that allows you to test and play the game. 
## Goals for Version 1:
- [ ] Make a basic gui that represents all the pieces in SFML
- [ ] basic class structure for internal and external representation of board
- [ ] make sure all moves are implemented correctly
- [ ] Test if all moves are implemented correctly using the test cases given in the wiki. 
## Task List for V1:
- [ ] Make a test folder 
- [ ] Test multiple fen strings
## Goals for Version 2:
- [ ] implement more complex algorithms, not entirely sure, basic minimax or alpha beta pruning and move on from there. 

### To build:
```cmake --build ./build```
### To run: 
```build/bin/main```

### To debug:
- ```cd build```
- ```cmake -DCMAKE_BUILD_TYPE=Debug ..```
### To run in debug mode: (in dir outside of build)
- ```gdb --args build/bin/main 2```

### To add more executables:
- still trying to figure out

## Major Resources used:
- https://sites.google.com/site/tscpchess/home?authuser=0
- https://www.chess.com/terms/fen-chess
- https://www.chessprogramming.org/Main_Page
## Heavily inspired by:
- Tom Kerrigen's Simple Chess program
