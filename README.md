8 Puzzle Solver
====
The 8 puzzle is a 3x3 matrix with characters 1-8 and a blank tile. The goal of the 8 puzzle is to slide the tiles of characters so they display the following:
```
	1 | 2 | 3
	4 | 5 | 6
	7 | 8 | *
```
With * representing the blank tile. I have implemented an 8 puzzle solver in the C++ file driver.cpp with three methods. The first method used to solve the 8 puzzle is the uniform cost search where the cost is equal for all moves. Uniform cost search is a variation of breadth first search for all the possible expansions of one state to the next. The second method implemented was A* with misplaced tile heuristics. This method was much more efficient that uniform cost because cost is equal to cost of expansion + the number of misplaced tiles. The final method implemented was A* with Manhattan distance heuristics. This algorithm has us calculate cost by taking the cost to expand puzzles to the point + distance of each single tile from its goal position or h(n) + g(n). This should yield the most efficient solution in terms of both time and space complexity. All of these methods should be able to yield the same solution however A* Manhattan should be the most efficient of the 3 algorithms.

To Run
====
```
	$ make
	$ ./driver
```
Enter the puzzle characters, separating them with spaces from L to R and top to bottom then press enter:
```
Enter the puzzle to solve('0' - '8' with 0 representing blank tile):
1 2 3 4 5 6 7 8 0
//the resulting table for that input would be 
		1 2 3
		4 5 6
		7 8 *
```
Then type 1 to run Uniform Cost, 2 for A* misplaced, and 3 for A* manhattan
