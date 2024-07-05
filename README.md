# Chess Engine!
This is an attempt to make a Chess Engine that is UCI complient eventually, as well as a basic GUI that allows you to test and play the game. 
## Goals for Version 1:
- [x] Make a basic gui that represents all the pieces in SFML
- [x] basic class structure for internal and external representation of board
- [x] make sure all moves are implemented correctly
- [x] Test if all moves are implemented correctly using the test cases given in the wiki. 
## Task List for V1:
- [x] Make a test folder 
- [x] Test multiple fen strings
## Task List for V1.5:
- [ ] Randomly choose moves allow a vs computer mode
- [ ] Perform time tests for depth results and go over code to reduce speed because there are a lot of low hanging performance improvements that can be made to this. 
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
### To add more executables:
- still trying to figure out

## Major Resources used:
- https://sites.google.com/site/tscpchess/home?authuser=0
- https://www.chess.com/terms/fen-chess
- https://www.chessprogramming.org/Main_Page
## Heavily inspired by:
- Tom Kerrigen's Simple Chess program
