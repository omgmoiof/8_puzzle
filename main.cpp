#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;




class Node
{
    protected:
        int** puzzle;
        int f_value;
        int h_value;
        int g_value;
    
    public:
        Node();
        Node(int**, int, int);
        void defaultPuzzle();
        void userGeneratedPuzzle();
        void displayPuzzle();
        // int misplacedTiles();
        // int manhattanDistance();
        bool isGoal();
        int getFValue();
        int getHValue();
        int getGValue();
        void setFValue(int);
        int** getPuzzle();
        int** moveUpward(int, int);
        int** moveDownward(int, int);
        int** moveLeft(int, int);
        int** moveRight(int, int);
    
};

struct CompareFValue
{
    bool operator()(Node* n1, Node* n2)
    {
        return n1->getFValue() > n2->getFValue();
    }
};


Node::Node() //constructs a Node with no parameters (only the initial state)
{
    puzzle = new int*[3];
    for (int i = 0; i < 3; i++)
    {
        puzzle[i] = new int[3];
    }
    g_value = 0;
} 

Node::Node(int** p, int g, int h) //constructs a Node with array and int parameters (game state, g and h values)
{
    puzzle = p;
    g_value = g;
    h_value = h;
    f_value = g_value + h_value;
}

int** Node::getPuzzle() //returns the game state
{
    return puzzle;
}

void Node::defaultPuzzle() //sets the game state to the predetermined state below
{
    
    puzzle[0][0] = 1;                      //| 1 3 2 |
    puzzle[0][1] = 3;                      //| 4 8 0 |
    puzzle[0][2] = 2;                      //| 7 6 5 |
    puzzle[1][0] = 4;
    puzzle[1][1] = 8;
    puzzle[1][2] = 0;
    puzzle[2][0] = 7;
    puzzle[2][1] = 6;
    puzzle[2][2] = 5;
    
}

void Node::userGeneratedPuzzle() //takes in user input to populate the array
{
    int x1, x2, x3;
    cout << "\tEnter your puzzle, use zero to represent the blank" << endl;
    cout << "\tEnter the first row, use space or tabs between numbers: ";
    cin >> x1; cin >> x2; cin >> x3;
    puzzle[0][0] = x1; puzzle[0][1] = x2; puzzle[0][2] = x3; 
    
    cout << "\tEnter the second row, use space or tabs between numbers: ";
    cin >> x1; cin >> x2; cin >> x3;
    puzzle[1][0] = x1; puzzle[1][1] = x2; puzzle[1][2] = x3;
    
    cout << "\tEnter the third row, use space or tabs between numbers: ";
    cin >> x1; cin >> x2; cin >> x3;
    puzzle[2][0] = x1; puzzle[2][1] = x2; puzzle[2][2] = x3;
    
}

int chooseAlgorithm() //prompts the user to select which algorithm to use
{
    int i = 0;
    cout << "\tEnter your choice of algorithm" << endl;
    cout << "\t\t1. Uniform Cost Search" << endl;
    cout << "\t\t2. A* with the Misplaced Tile heuristic" << endl;
    cout << "\t\t3. A* with the Manhattan Distance heuristic" << endl;
    
    cin >> i;
    cout << endl;
    
    if (i != 1 && i != 2 && i != 3)
    {
        cout << "Error, invalid input" << endl;
        i = chooseAlgorithm();
        
    }
    return i;
    
    
}

void Node::displayPuzzle() //outputs the values in the array
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) cout << puzzle[i][j] << " ";
        cout << endl;
    }
    
    
}

int misplacedTiles(int** puzzle) //returns the number of misplaced tiles in the game state
{
    int misplacedTiles = 0;
    if (puzzle[0][0] != 1) misplacedTiles++;
    if (puzzle[0][1] != 2) misplacedTiles++;
    if (puzzle[0][2] != 3) misplacedTiles++;
    if (puzzle[1][0] != 4) misplacedTiles++;
    if (puzzle[1][1] != 5) misplacedTiles++;
    if (puzzle[1][2] != 6) misplacedTiles++;
    if (puzzle[2][0] != 7) misplacedTiles++;
    if (puzzle[2][1] != 8) misplacedTiles++;
    
    return misplacedTiles;
}

int manhattanDistance(int** puzzle) //returns the manhattan distance of the game state
{
    int counter = 0;
    int value = 0;
    int xdist = 0;
    int ydist = 0;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (puzzle[i][j] != 0)
            {
                value = puzzle[i][j];
                xdist = abs(i - ((value-1)/3)); //(value-1)/3 gives expected x coordinate for a particular value
                ydist = abs(j - ((value-1)%3)); //(value-1)%3 gives expected y coordinate for a particular value
            
                // cout << value << " is displaced " << ydist << " horizontally and " << xdist << " vertically" << endl;
            
                counter += xdist + ydist;
                
            }
            
        }
    }
    
    return counter;
}

bool Node::isGoal() //returns whether or not the puzzle is completed
{
    return misplacedTiles(puzzle) == 0;
}

Node* UniformCostSearch(Node* initialNode) //performs the Uniform counter Search
{
  int counter = 0;
  priority_queue<Node*, vector<Node*>, CompareFValue> queue; //priorty queue that enqueues nodes by their F Value
  queue.push(initialNode);
  Node* temp;
  int x = 0;
  int y = 0;
  
  cout << "Expanding state:" << endl;
  initialNode->displayPuzzle();
  
  while(true)
  {
      if (queue.empty()) //checks if queue is empty
      {
          cout << "No solution" << endl;
          return NULL;
      }
      
      temp = queue.top();
      queue.pop();
      if (temp->isGoal()) //checks if current state is the goal state
      {
          cout << "Solution found!!!" << endl;
          temp->displayPuzzle();
          cout << "The search algorithm expanded " << counter << " nodes." << endl;
          cout << "The depth of the goal node is " << temp->getGValue() << "." << endl;
          return temp;
          
      }
      
      //expands the current node
      
      cout << "The best state to expand with g(n) = " << temp->getGValue() << " and h(n) = " << temp->getHValue() << endl;
      temp->displayPuzzle();
      
      int** p = temp->getPuzzle();
      for (int i = 0; i < 3; i++)
      {
          for (int j = 0; j < 3; j++)
          {
             if (p[i][j] == 0)
             {
                 x = j;
                 y = i;
             }
          }
       }
       
       if ( y != 0 ) queue.push(new Node(temp->moveUpward(y,x), temp->getGValue() + 1, 0));
       if ( y != 2 ) queue.push(new Node(temp->moveDownward(y,x), temp->getGValue() + 1, 0));
       if ( x != 0 ) queue.push(new Node(temp->moveLeft(y,x), temp->getGValue() + 1, 0));
       if ( x != 2 ) queue.push(new Node(temp->moveRight(y,x), temp->getGValue() + 1, 0));
       
       counter++;
  }
    
    
}

Node* MisplacedTileSearch(Node* initialNode) //performs the A* search based on the Misplaced Tile heuristic
{
  int counter = 0;
  priority_queue<Node*, vector<Node*>, CompareFValue> queue; //priorty queue that enqueues nodes by their F Value
  initialNode->setFValue(misplacedTiles(initialNode->getPuzzle()));
  queue.push(initialNode);
  Node* temp;
  int x = 0;
  int y = 0;
  
  cout << "Expanding state:" << endl;
  initialNode->displayPuzzle();
  
  while(true)
  {
      if (queue.empty() == true) //checks if queue is empty
      {
          cout << "No solution" << endl;
          return NULL;
      }
      
      temp = queue.top();
      queue.pop();
      if (temp->isGoal()) //checks if current state is the goal state
      {
          cout << "Solution found!!!" << endl;
          temp->displayPuzzle();
          cout << "The search algorithm expanded " << counter << " nodes." << endl;
          cout << "The depth of the goal node is " << temp->getGValue() << "." << endl;
          return temp;
          
      }
      
      //expands the current node
      
      cout << "The best state to expand with g(n) = " << temp->getGValue() << " and h(n) = " << temp->getHValue() << endl;
      temp->displayPuzzle();

      
      int** p = temp->getPuzzle();
      for (int i = 0; i < 3; i++)
      {
          for (int j = 0; j < 3; j++)
          {
             if (p[i][j] == 0)
             {
                 x = j;
                 y = i;
             }
          }
       }
       
       
       if ( y != 0 ) queue.push(new Node(temp->moveUpward(y,x), temp->getGValue() + 1, misplacedTiles(temp->moveUpward(y,x))));
       if ( y != 2 ) queue.push(new Node(temp->moveDownward(y,x), temp->getGValue() + 1, misplacedTiles(temp->moveDownward(y,x))));
       if ( x != 0 ) queue.push(new Node(temp->moveLeft(y,x), temp->getGValue() + 1, misplacedTiles(temp->moveLeft(y,x))));
       if ( x != 2 ) queue.push(new Node(temp->moveRight(y,x), temp->getGValue() + 1, misplacedTiles(temp->moveRight(y,x))));
       
       counter++;
  }
    
}

Node* ManhattanDistanceSearch(Node* initialNode) //performs the A* search based on the Manhattan Distance heuristic
{
  int counter = 0;
  int testCounter = 0;
  priority_queue<Node*, vector<Node*>, CompareFValue> queue; //priorty queue that enqueues nodes by their F Value
  initialNode->setFValue(manhattanDistance(initialNode->getPuzzle()));
  queue.push(initialNode);
  Node* temp;
  Node* upNode, downNode, leftNode, rightNode;
  int x = 0;
  int y = 0;
  
  cout << "Expanding state:" << endl;
  initialNode->displayPuzzle();
  
  while(true)
  {
      if (queue.empty()) //checks if queue is empty
      {
          cout << "No solution" << endl;
          return NULL;
      }
      
      temp = queue.top();
      queue.pop();
      if (temp->isGoal()) //checks if current state is the goal state
      {
          cout << "Solution found!!!" << endl;
          temp->displayPuzzle();
          cout << "The search algorithm expanded " << counter << " nodes." << endl;
          cout << "The depth of the goal node is " << temp->getGValue() << "." << endl;
          return temp;
          
      }
      
      
      //expands the current node
      cout << "The best state to expand with g(n) = " << temp->getGValue() << " and h(n) = " << temp->getHValue() << endl;
      temp->displayPuzzle();

      
      int** p = temp->getPuzzle();
      for (int i = 0; i < 3; i++)
      {
          for (int j = 0; j < 3; j++)
          {
             if (p[i][j] == 0)
             {
                 x = j;
                 y = i;
             }
          }
       }
       
      if ( y != 0 )
      {
          upNode = new Node(temp->moveUpward(y,x), temp->getGValue() + 1, manhattanDistance(temp->moveUpward(y,x)));
          queue.push(upNode);
          
      }
    //   if ( y != 2 )
    //   {
    //       downNode = new Node(temp->moveDownward(y,x), temp->getGValue() + 1, manhattanDistance(temp->moveDownward(y,x)));
    //       queue.push(downNode);
          
    //   }
    //   if ( x != 0 )
    //   {
    //       leftNode = new Node(temp->moveLeft(y,x), temp->getGValue() + 1, manhattanDistance(temp->moveLeft(y,x)));
    //       queue.push(leftNode);
    //   }
    //   if ( x != 2 ) 
    //   {
    //       rightNode = new Node(temp->moveRight(y,x), temp->getGValue() + 1, manhattanDistance(temp->moveRight(y,x)));
    //       queue.push(rightNode);
    //   }
       counter++;
       while (!queue.empty())
       {
           queue.top()->displayPuzzle();
           queue.pop();
           cout << testCounter++ << endl;
           
       }
    }
}

int Node::getFValue() //returns the f value
{
    return f_value;
}

int Node::getHValue() //returns the h value
{
    return h_value;
}

int Node::getGValue() //returns the g value
{
    return g_value;
}

void Node::setFValue(int f) //sets the f value
{
    f_value = f;
}

int** Node::moveUpward(int i, int j) //returns the game state in which the blank space is moved upward
{
    int** temp = puzzle;
    swap(temp[i][j],temp[i-1][j]);
    return temp;
}

int** Node::moveDownward(int i, int j) //returns the game state in which the blank space is moved downward
{
    int** temp = puzzle;
    swap(temp[i][j], temp[i+1][j]);
    return temp;
}

int** Node::moveLeft(int i, int j) //returns the game state in which the blank space is moved left
{
    int** temp = puzzle;
    swap(temp[i][j],temp[i][j-1]);
    return temp;
}

int** Node::moveRight(int i, int j) //returns the game state in which the blank space is moved right
{
    int** temp = puzzle;
    swap(temp[i][j],temp[i][j+1]);
    return temp;
}


int main()
{
    int response = 0;
    int choice = 0;
    Node* initialState = new Node();
    
    cout << "Welcome to Phillip's Miraculous 8-Puzzle Solver." << endl;
    
    while (response != 1 && response != 2)
    {
        cout << "Type \"1\" to use the default puzzle, or \"2\" to enter your own puzzle." << endl;
    
        cin >> response;
        cout << endl;
    
        if (response == 1) initialState->defaultPuzzle();
        else if (response == 2) initialState->userGeneratedPuzzle();
        else 
        {
        cout << "Error, invalid input" << endl;
        
        }
    
    }
    
    
    choice = chooseAlgorithm();
    
    initialState->displayPuzzle();
    Node* goal;
    if (choice == 1) goal = UniformCostSearch(initialState);
    if (choice == 2) goal = MisplacedTileSearch(initialState);
    if (choice == 3) goal = ManhattanDistanceSearch(initialState);
    
    if (goal == NULL) cout << "No solution" << endl;
    
    
    // Node* a = new Node();
    // a->userGeneratedPuzzle();
    
    // Node* temp = new Node(a->moveDownward(0,0),0,0);
    // temp->displayPuzzle();
    
    // int** b;
    // priority_queue<Node*, vector<Node*>, CompareFValue> queue;
    // queue.push(new Node(b, 100, 100));
    // queue.push(new Node(b, 0, 0));
    // queue.push(new Node(b, 7, 2));
    
    // temp = queue.top();
    // queue.pop();
    // cout << temp->getFValue();
    // temp = queue.top();
    // queue.pop();
    // cout << temp->getFValue();
    // temp = queue.top();
    // queue.pop();
    // cout << temp->getFValue();
    
    
}


