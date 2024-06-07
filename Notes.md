## Thoughts on decision making
- my goal is to make a very very simple albeit inefficient engine. I don't think i'll make all the correct decisions the first time around anyways. 
- Currently i plan to use the 8 by 8 board representation to make it extremely simple for me to think, later it can be moved to more efficient bitboard implementations. 
- move generation will be done using the mailbox method (because that seems simplest):  
- represent this as a 1 d array instead of 2d array 
- i am doing a square centric representation of the board. 
## Move generator must:
- take a chess position as input and return the list of all the legal moves as output. 
  - first generate all psuedo legal moves
  - then check if move is legal

