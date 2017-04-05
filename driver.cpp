#include <iostream>
#include <queue>
#include <vector>
#include <unistd.h>
#include <utility>
using namespace std;

// Global Vars
const int x = 3, y = 3;
int heuristic = 0; // 0 as Default, if 1 use Manhattan for cost
int nodes_expanded = 0, depth = 0, maxQ = 0;

struct myPuzzle{
   char goal[3][3];
   char puzzle[3][3];
   int cost = 0;
   int node_depth = 0;
   int blank_x;
   int blank_y;
   myPuzzle * parent = NULL;
   
   myPuzzle();
   myPuzzle(const struct myPuzzle & src);
   bool isDone();
   bool operator!=(const myPuzzle& p) const;
   bool operator==(const myPuzzle& p) const;
   
   void printPuzzle();
   int misTiles();
   int manhattan();
   void fillTable();
};

// Helper Functions
bool alreadySolved(struct myPuzzle p, vector <myPuzzle *> & v); // Checks for redundant Nodes
int mv_left(struct myPuzzle & p);   
int mv_right(struct myPuzzle & p);
int mv_up(struct myPuzzle & p);
int mv_down(struct myPuzzle & p);

// Solving Algo's
void solveUniCost(struct myPuzzle p);
void h_misplaced(struct myPuzzle p);
void h_manhattan(struct myPuzzle p);

// ====================== MAIN ======================
int main(){
   char userIn = '1';

   cout << "8 Puzzle Solver\n";
   
   // Puzzle to solve
   myPuzzle p1;
   p1.fillTable(); // Populate
   p1.printPuzzle();
         
   cout << "0 for Uniform Cost, 1 for A* Misplaced Tiles, else A* Manhattan: ";
   cin >> userIn;
   if(userIn == '0'){
   	heuristic = 0;
   	p1.cost = p1.misTiles();
   	cout << "Misplaced Tiles: " << p1.misTiles() << endl << "Blank at index: " 
         << p1.blank_x << ":" << p1.blank_y << endl << endl;
      solveUniCost(p1);
   }
   else if(userIn == '1'){
   	heuristic = 0;
   	p1.cost = p1.misTiles();
   	cout << "Misplaced Tiles: " << p1.misTiles() << endl << "Blank at index: " 
         << p1.blank_x << ":" << p1.blank_y << endl << endl;
      h_misplaced(p1);
   }
   else{
   	heuristic = 1; // Set Heuristic As Manhattan
   	p1.cost = p1.manhattan();
   	cout << "Manhattan Distance: " << p1.manhattan() << endl << "Blank at index: " 
         << p1.blank_x << ":" << p1.blank_y << endl << endl;
   	h_manhattan(p1);
   }
   return 0;
}


// =======================OTHER STUFF===================

//Comparator for priority_queue
struct comp : public  std::binary_function<myPuzzle *, myPuzzle*, bool>{
   bool operator()(const myPuzzle* a, const myPuzzle* b) const{
      return a->cost > b->cost;
   }
};

void h_manhattan(struct myPuzzle p){   
	priority_queue <myPuzzle *, vector<myPuzzle *>, comp> solveQ; // Queue of puzzles
	vector <myPuzzle *> solved; // Puzzle not to repeat
	
	myPuzzle * root = &p;
	myPuzzle * cur = root;
	// Add original puzzle as puzzles not to repeat
	solved.push_back(root);
	// First puzzle as head
	solveQ.push(root);
	
	// Counters for Depth
	depth = 0;
	
	//SOLVE
	while(solveQ.size() > 0){
	   // Take current queue
		cur = solveQ.top();
	
		//Check max nodes in queue
		if(maxQ < solveQ.size()) maxQ = solveQ.size();
		
		// Check if queue's head is the solution
		if (cur->isDone()){
		   // If goal == puzzle, break out of loop
		   cout << "\nA* Manhattan Distance - DONE!\n";
			cur->printPuzzle();
			while(cur->parent != NULL){
			   //Increase depth
		      depth++;
			   cur = cur->parent;
			   
			   cout << endl;
			  
			   cur->printPuzzle();
			}
			break;
		}
		nodes_expanded++;
		// Expand
      myPuzzle* left = new myPuzzle(*cur);
      myPuzzle* right = new myPuzzle(*cur);
      myPuzzle* down = new myPuzzle(*cur);
      myPuzzle* up = new myPuzzle(*cur);
      
      if( mv_left(*left) != -1 && alreadySolved(*left, solved) == false ){
         left->parent = cur;
         if(left != NULL){
	         //==== H(n) + G(n)
	         left->node_depth = left->parent->node_depth + 1;
	         left->cost += left->node_depth;
         }
	      solveQ.push(left);
         solved.push_back(left);
		}
		else delete[] left;
		
		if( mv_right(*right) != -1 && alreadySolved(*right, solved) == false ){
		   right->parent = cur;
		   if(right != NULL){
	         //==== H(n) + G(n)
	         right->node_depth = right->parent->node_depth + 1;
	         right->cost += right->node_depth;
         }
         solveQ.push(right);
		   solved.push_back(right);
		}
		else delete[] right;
		
		if( mv_up(*up) != -1 && alreadySolved(*up, solved) == false ){
		   up->parent = cur;
		   if(up != NULL){
	         //==== H(n) + G(n)
	         up->node_depth = up->parent->node_depth + 1;
	         up->cost += up->node_depth;
         }
         solveQ.push(up);
		   solved.push_back(up);
		}
		else delete[] up;
		
		if( mv_down(*down) != -1 && alreadySolved(*down, solved) == false ){
		   down->parent = cur;
		   if(down != NULL){
	         //==== H(n) + G(n)
	         down->node_depth = down->parent->node_depth + 1;
	         down->cost += down->node_depth;
         }
         solveQ.push(down);
		   solved.push_back(down);
		   
		}
		else delete[] down;
		//Remove top since its not the final goal
		solveQ.pop();
		cout << endl;
	}
	cout << "#Max Depth: " << depth << "   #Nodes Expanded: " << nodes_expanded 
	   << "   #Max Queue Size: " << maxQ << endl;
	   
	// Clean Dangling pointers    // Not Working
	// Delete dangling pointers
	for (std::vector<myPuzzle *>::iterator it = solved.begin(); it != solved.end(); ){
	   myPuzzle * deleteThis = *it;
	   it = solved.erase(it);
	   delete[] deleteThis;
	   ++it;
	}
}

void h_misplaced(struct myPuzzle p){   
	priority_queue <myPuzzle *, vector<myPuzzle *>, comp> solveQ; // Queue of puzzles
	vector <myPuzzle *> solved; // Puzzles not to repeat
	
	myPuzzle * root = &p;
	myPuzzle * cur = root;
	// Add original puzzle as puzzles not to repeat
	solved.push_back(root);
	// First puzzle as head
	solveQ.push(root);
	
	// Counters for Depth
	depth = 0;
	
	//SOLVE
	while(solveQ.size() > 0){
	   // Take current queue
		cur = solveQ.top();
	   
		//Check max nodes in queue
		if(maxQ < solveQ.size()) maxQ = solveQ.size();
		
		// Check if queue's head is the solution
		if (cur->isDone()){
		   // If goal == puzzle, break out of loop
		   cout << "\nA* Misplaced Tiles - DONE!\n";
			cur->printPuzzle();
			while(cur->parent != NULL){
			   //Increase depth
		      depth++;
			   cur = cur->parent;
			   
			   cout << endl;
			   
			   cur->printPuzzle();
			}
			break;
		}
		nodes_expanded++;
		// Expand
      myPuzzle* left = new myPuzzle(*cur);
      myPuzzle* right = new myPuzzle(*cur);
      myPuzzle* down = new myPuzzle(*cur);
      myPuzzle* up = new myPuzzle(*cur);
      
      if( mv_left(*left) != -1 && alreadySolved(*left, solved) == false ){
         left->parent = cur;
         if(left != NULL){
	         //==== H(n) + G(n)
	         left->node_depth = left->parent->node_depth + 1;
	         left->cost += left->node_depth;
         }
	      solveQ.push(left);
         solved.push_back(left);
		}
		else delete[] left;
		
		if( mv_right(*right) != -1 && alreadySolved(*right, solved) == false ){
		   right->parent = cur;
		   if(right != NULL){
	         //==== H(n) + G(n)
	         right->node_depth = right->parent->node_depth + 1;
	         right->cost += right->node_depth;
         }
         solveQ.push(right);
		   solved.push_back(right);
		}
		else delete[] right;
		
		if( mv_up(*up) != -1 && alreadySolved(*up, solved) == false ){
		   up->parent = cur;
		   if(up != NULL){
	         //==== H(n) + G(n)
	         up->node_depth = up->parent->node_depth + 1;
	         up->cost += up->node_depth;
         }
         solveQ.push(up);
		   solved.push_back(up);
		}
		else delete[] up;
		
		if( mv_down(*down) != -1 && alreadySolved(*down, solved) == false ){
		   down->parent = cur;
		   if(down != NULL){
	         //==== H(n) + G(n)
	         down->node_depth = down->parent->node_depth + 1;
	         down->cost += down->node_depth;
         }
         solveQ.push(down);
		   solved.push_back(down);
		   
		}
		else delete[] down;
		//Remove top since its not the final goal
		solveQ.pop();
		cout << endl;
	}
	cout << "#Max Depth: " << depth << "   #Nodes Expanded: " << nodes_expanded 
	   << "   #Max Queue Size: " << maxQ << endl;
	   
	// Delete dangling pointers
	// Not Working
	for (std::vector<myPuzzle *>::iterator it = solved.begin(); it != solved.end(); ){
	   myPuzzle * deleteThis = *it;
	   it = solved.erase(it);
	   delete[] deleteThis;
	   ++it;
	}
}

void solveUniCost(struct myPuzzle p){   
	queue <myPuzzle *> solveQ; // Queue of puzzles
	vector <myPuzzle *> solved; // Puzzled not to repeat
	
	myPuzzle * root = &p;
	myPuzzle * cur = root;
	// Add original puzzle as puzzles not to repeat
	solved.push_back(root);
	// First puzzle as head
	solveQ.push(root);
	
	// Counters for Depth
	depth = 0;
	
	//SOLVE
	while(solveQ.size() > 0){
	   // Take current queue
		cur = solveQ.front();
		
		//Check max nodes in queue
		if(maxQ < solveQ.size()) maxQ = solveQ.size();
		
		// Check if queue's head is the solution
		if (cur->isDone()){
		   // If goal == puzzle, break out of loop
			cout << "\nUniform Cost - DONE!\n";
			cur->printPuzzle();
			while(cur->parent != NULL){
			   //Increase depth
		      depth++;
			   cur = cur->parent;
			   
			   cout << endl;
			   
			   cur->printPuzzle();
			}
			break;
		}
		nodes_expanded++;
		// Expand
      myPuzzle* left = new myPuzzle(*cur);
      myPuzzle* right = new myPuzzle(*cur);
      myPuzzle* down = new myPuzzle(*cur);
      myPuzzle* up = new myPuzzle(*cur);
      
      if( mv_left(*left) != -1 && alreadySolved(*left, solved) == false ){
         left->parent = cur;
         solveQ.push(left);
         solved.push_back(left);
		}
		if( mv_right(*right) != -1 && alreadySolved(*right, solved) == false ){
		   right->parent = cur;
		   solveQ.push(right);
		   solved.push_back(right);
		}
		if( mv_up(*up) != -1 && alreadySolved(*up, solved) == false ){
		   up->parent = cur;
		   solveQ.push(up);
		   solved.push_back(up);
		}
		if( mv_down(*down) != -1 && alreadySolved(*down, solved) == false ){
		   down->parent = cur;
		   solveQ.push(down);
		   solved.push_back(down);
		   
		}
		//Remove top since its not the final goal
		solveQ.pop();
		cout << endl;
	}
	cout << "#Max Depth: " << depth << "   #Nodes Expanded: " << nodes_expanded 
	   << "   #Max Queue Size: " << maxQ << endl;
}

bool alreadySolved(struct myPuzzle p, vector <myPuzzle *> & v){
   for(int i = 0; i < v.size(); i++){
      if(p == *v.at(i)){
         return true;
      }
   }
   return false;
}

bool myPuzzle::operator!=(const myPuzzle& p) const{
   int diff = 0;
   for(int i = 0; i < x ; i++){
      for(int j = 0; j < y; j++){
         if(p.puzzle[i][j] != puzzle[i][j]) return true;
      }
   }
   return false;
}

bool myPuzzle::operator==(const myPuzzle& p) const{
   int diff = 0;
   for(int i = 0; i < x ; i++){
      for(int j = 0; j < y; j++){
         if(p.puzzle[i][j] != puzzle[i][j]) return false;
      }
   }
   return true;
}

bool myPuzzle::isDone(){ // Bool checker if goal == puzzle
	int diff = 0;
	for(int i = 0; i < x ; i++){
	   for(int j = 0; j < y; j++){
	      if(puzzle[i][j] != goal[i][j]) return false; 
	   }
	}
   return true;
}

myPuzzle::myPuzzle(){ // Empty Init
	goal[0][0] = '1';
	goal[0][1] = '2';
	goal[0][2] = '3';
	goal[1][0] = '4';
	goal[1][1] = '5';
	goal[1][2] = '6';
	goal[2][0] = '7';
	goal[2][1] = '8';
	goal[2][2] = '0';
	blank_x = 3;
	blank_y = 3;
	cost = 0;
	node_depth = 0;
}

myPuzzle::myPuzzle(const struct myPuzzle & src){ // Constructor a(b) -> a = b
	goal[0][0] = '1';
	goal[0][1] = '2';
	goal[0][2] = '3';
	goal[1][0] = '4';
	goal[1][1] = '5';
	goal[1][2] = '6';
	goal[2][0] = '7';
	goal[2][1] = '8';
	goal[2][2] = '0';
	puzzle[0][0] = src.puzzle[0][0];
	puzzle[0][1] = src.puzzle[0][1];
	puzzle[0][2] = src.puzzle[0][2];
	puzzle[1][0] = src.puzzle[1][0];
	puzzle[1][1] = src.puzzle[1][1];
	puzzle[1][2] = src.puzzle[1][2];
	puzzle[2][0] = src.puzzle[2][0];
	puzzle[2][1] = src.puzzle[2][1];
	puzzle[2][2] = src.puzzle[2][2];
   cost = src.cost;
	blank_x = src.blank_x;
	blank_y = src.blank_y;
	node_depth = src.node_depth;
}

int myPuzzle::misTiles(){//Count how many tiles is misplaced
   int distance = 0;
   for(int i = 0; i < x; i++){
         for(int j = 0; j < y; j++){
            if( (puzzle[i][j] != goal[i][j]) && (puzzle[i][j] != '0') ){
                distance++;  
            }        
         }
   }
   //cout << "Misplaced Tiles" << endl;
   return distance;
}   

int myPuzzle::manhattan(){
	int distance = 0;
	for(int i = 0; i < x; i++){
		for(int j = 0; j < y; j++){
			if(puzzle[i][j] != '0'){
				// Look for goal
				for(int m = 0; m < x; m++){
					for(int n = 0; n < y; n++){
						// Misplaced, look for distance
						if(goal[m][n] == puzzle[i][j]){
							distance = distance + (abs(i-m) + abs(j-n));
						}
					}
				}
			}
		}
	}
	return distance;
}

void myPuzzle::fillTable(){
      cout << "Enter 1-8, 0 for space:\n";
      for(int i = 0; i < x; i++){
         for(int j = 0; j < y; j++){
            cin >>  puzzle[i][j];
            if (puzzle[i][j]=='0') {
               blank_x = i;
               blank_y = j;
            }
         }
      }
}

void myPuzzle::printPuzzle(){
   for(int i = 0; i < x; i++){
      for(int j = 0; j < y; j++){
         cout << puzzle[i][j] << " ";
      }
      cout << endl;      
   }
}
// Move Functions =============================================================
int mv_left(struct myPuzzle & p){
   if(p.blank_y == 0) return -1;
   else{
      //Swap 0s
      char temp = p.puzzle[p.blank_x][p.blank_y - 1];
      p.puzzle[p.blank_x][p.blank_y - 1] = p.puzzle[p.blank_x][p.blank_y];
      p.puzzle[p.blank_x][p.blank_y] = temp;
      //Update blanks
      p.blank_y = (p.blank_y - 1);
   }
   if(heuristic == 0) p.cost = p.misTiles();
   else p.cost = p.manhattan();
   return p.cost;
}

int mv_right(struct myPuzzle & p){
   if(p.blank_y == 2) return - 1;
   else{
      //Swap 0s
      char temp = p.puzzle[p.blank_x][p.blank_y + 1];
      p.puzzle[p.blank_x][p.blank_y + 1] = p.puzzle[p.blank_x][p.blank_y];
      p.puzzle[p.blank_x][p.blank_y] = temp;
      //Update blanks
      p.blank_y = p.blank_y + 1;
   }
   if(heuristic == 0) p.cost = p.misTiles();
   else p.cost = p.manhattan();
   return p.cost;
}
int mv_up(struct myPuzzle & p){
   if(p.blank_x == 0) return -1;
   else{
      //Swap 0s
      char temp = p.puzzle[p.blank_x-1][p.blank_y];
      p.puzzle[p.blank_x-1][p.blank_y] = p.puzzle[p.blank_x][p.blank_y];
      p.puzzle[p.blank_x][p.blank_y] = temp;
      //Update blanks
      p.blank_x = p.blank_x - 1;
   }
   if(heuristic == 0) p.cost = p.misTiles();
   else p.cost = p.manhattan();
   return p.cost;
}
int mv_down(struct myPuzzle & p){
   if(p.blank_x == 2) return -1;
   else{
      //Swap 0s
      char temp = p.puzzle[p.blank_x + 1][p.blank_y];
      p.puzzle[p.blank_x + 1][p.blank_y] = p.puzzle[p.blank_x][p.blank_y];
      p.puzzle[p.blank_x][p.blank_y] = temp;
      //Update blanks
      p.blank_x = p.blank_x + 1;
   }
   if(heuristic == 0) p.cost = p.misTiles();
   else p.cost = p.manhattan();
   return p.cost;
}
