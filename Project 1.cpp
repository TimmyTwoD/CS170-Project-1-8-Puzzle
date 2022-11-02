#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

//hard-coded template states
vector<vector<int>> goalState = {{1, 2, 3},  
                                 {4, 5, 6},
                                 {7, 8, 0}}; 

vector<vector<int>> impossible = {{1, 4, 3},  
                                  {2, 5, 0},
                                  {6, 7, 8}};

vector<vector<int>> failed = {{0, 0, 0},  
                              {0, 0, 0},
                              {0, 0, 0}};

vector<vector<int>> VeryEasy = {{1, 2, 3},  
                                {4, 5, 6},
                                {7, 0, 8}};

vector<vector<int>> Easy = {{1, 2, 3},  
                            {5, 0, 6},
                            {4, 7, 8}};

vector<vector<int>> Hard = {{1, 6, 7},  
                            {5, 0, 3},
                            {4, 8, 2}};

struct Node
{
    vector<vector<int>> puzzle;
    int PositionOfZeroRow;
    int PositionOfZeroColumn;
    vector<string> solutionPath;
    int heuristic;
    
};

struct Heuristic // comparator class for constructing the priority queue
{
    bool operator()(const Node &a, const Node &b)
    {
        return ((a.heuristic + a.solutionPath.size()) > (b.heuristic+ b.solutionPath.size())); 
    }
};

int getHeuristic(int whichHeur, vector<vector<int>> temp)//this function gets the h(n) value for each puzzle
{
    int heur = 0;

    if (whichHeur == 1)//uniform cost sets h(n) to 0
    {
        heur = 0;
    }

    if (whichHeur == 2)//misplaced tile heuristic
    {
       for(int i = 0; i < temp.size(); i++){
            for(int j = 0; j < temp.size(); j++){
                if (temp[i][j] != goalState[i][j])
                { 
                    if (temp[i][j] != 0)
                    {
                        heur++;//counts number of misplaced tiles in the puzzle
                    }
                }
           }
        }
    }

    if (whichHeur == 3)//manhattan distance heuristc
    {
       for(int i = 0; i < temp.size(); i++){
            for(int j = 0; j < temp.size(); j++){
                if (temp[i][j] != goalState[i][j])
                { 
                    if (temp[i][j] != 0)//gets the distance of each misplaced tile to the goal state and adds it to h(n)
                    {
                        if (temp[i][j] == 1)
                        {
                            heur += i+j;
                        }
                        if (temp[i][j] == 2)
                        {
                            heur += i + abs(j-1);
                        }
                        if (temp[i][j] == 3)
                        {
                            heur += i + abs(j-2);
                        }
                        if (temp[i][j] == 4)
                        {
                            heur += abs(i-1) + j;
                        }
                        if (temp[i][j] == 5)
                        {
                            heur += abs(i-1) + abs(j-1);
                        }
                        if (temp[i][j] == 6)
                        {
                            heur += abs(i-1) + abs(j-2);
                        }
                        if (temp[i][j] == 7)
                        {
                            heur += abs(i-2) + j;
                        }
                        if (temp[i][j] == 8)
                        {
                            heur += abs(i-2) + abs(j-1);
                        }
                    }
                }
           }
        }
    }

    return heur;
}
Node Get0Position(vector<vector<int>> child) // finds the position of the "0" and updates the nodes with it
{// this function is really only needed for the initial state
    Node temp;
    temp.puzzle = child; 
    
    for(int i = 0; i < temp.puzzle.size(); i++){
        for(int j = 0; j < temp.puzzle.size(); j++){
            if(temp.puzzle[i][j] == 0){
                temp.PositionOfZeroRow = i;
                temp.PositionOfZeroColumn = j;
            }
        }
    }
    
    return temp;
}

Node search(vector<vector<int>> problem, int whichHeuristic)//main search function
{   
    clock_t tStart = clock();
    priority_queue<Node, vector<Node>, Heuristic> test;
    set<vector<vector<int>>> trackDupes;
    int dupeSize;
    Node currstate;
    Node temp;
    Node failstate;
    failstate.puzzle = failed; // making a hardcoded fail state just for the function to return something
    int maxQueueSize = test.size();
    int numNodesExpanded = 0;
    int tempNum = 0;

    temp = (Get0Position(problem));/// initializing the first state
    temp.heuristic = getHeuristic(whichHeuristic, problem);
    test.push(temp);
    trackDupes.insert(problem);

    while (!test.empty())
    {
        tempNum = test.size();
        maxQueueSize = max(maxQueueSize, tempNum);

        currstate = test.top();
        trackDupes.insert(currstate.puzzle); 

        test.pop();
        numNodesExpanded++;

        if (currstate.puzzle == goalState)
        {
            
            for(int i = 0; i < currstate.puzzle.size(); i++)
            {
                cout << endl;
                for(int j = 0; j < currstate.puzzle.size(); j++)
                {
                    cout << currstate.puzzle[i][j] << " ";
                }
            }
            cout << "\nGoal state!\n";
            cout << "Solution depth was " << currstate.solutionPath.size() << endl;
            cout << "Number of nodes expanded: " << numNodesExpanded-1 << endl;
            cout << "Max queue size: " << maxQueueSize << endl;
            printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


            return currstate;
        }

        /*
        for(int i = 0; i < currstate.puzzle.size(); i++)
        {
            cout << endl;
            for(int j = 0; j < currstate.puzzle.size(); j++)
            {
                cout << currstate.puzzle[i][j] << " ";
            }
        }
        
        cout << endl;

        cout << "The best state to expand with a g(n) = " << currstate.solutionPath.size() << " and h(n) = " << currstate.heuristic << " is ";
        */

        if (currstate.PositionOfZeroColumn != 0) //checks if can swap left
        {   
            temp = currstate;

            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1]; //sets the number on the left to the 0 position
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1] = 0; //sets 0 to the position of the swapped number
            temp.PositionOfZeroColumn--;

            dupeSize = trackDupes.size(); //checking dupes
            trackDupes.insert(temp.puzzle);
            if (dupeSize != trackDupes.size()) //if not duped, ready to push and update node's attributes
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Left");
                test.push(temp);
            }

        }

        if (currstate.PositionOfZeroColumn < currstate.puzzle.size()-1) //checks if can swap right
        {
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn+1]; //sets the number on the right to the 0 position
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn+1] = 0; //sets 0 to the position of the swapped number
            temp.PositionOfZeroColumn++;

            dupeSize = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (dupeSize != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Right");
                test.push(temp);
            }

        }

        if (currstate.PositionOfZeroRow != 0) //checks if can swapp up
        {
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow-1][temp.PositionOfZeroColumn]; //sets the number above to where 0 position
            temp.puzzle[temp.PositionOfZeroRow-1][temp.PositionOfZeroColumn] = 0; //sets 0 to the position of the swapped number
            temp.PositionOfZeroRow--;

            dupeSize = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (dupeSize != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Up");
                test.push(temp);
            }

        }

        if (currstate.PositionOfZeroRow < currstate.puzzle[currstate.PositionOfZeroRow].size()-1) //checks if can swap down
        {   
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow+1][temp.PositionOfZeroColumn]; //sets the number below to where 0 position
            temp.puzzle[temp.PositionOfZeroRow+1][temp.PositionOfZeroColumn] = 0; //sets 0 to the position of the swapped number
            temp.PositionOfZeroRow++;

            dupeSize = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (dupeSize != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Down");
                test.push(temp);
            }

        }

    }

    return failstate;
}


int main(){

    Node end;
    int heuristic = 0;
    int input;
    int input2;
    int input3;

    vector<vector<int>> example= {{1, 2, 3},  
                                  {5, 0, 6},
                                  {4, 7, 8}}; 
    
    cout << endl << "David Liu's 8-Puzzle Solver\n\n";
    cout <<"========================================================\n\n";
    cout << "Enter \"1\" for a default puzzle or \"2\" to create a custom puzzle\n";
    cin >> input;

    if (input == 1)
    {
        cout << "Choose a default puzzle by entering the number: " << endl;
        cout << "1. Already Solved\n"; //goalstate already
        cout << "2. Very Easy\n"; //depth 1 puzzle
        cout << "3. Easy\n"; // depth 4 puzzle in the project details
        cout << "4. Hard\n"; // depth 16 puzle in the project details
        cout << "5. Literally Impossible\n"; //unsolvable config

        cin >> input;

        if (input == 1)
        {
            example = goalState;
        }
        else if (input == 2)
        {
            example = VeryEasy;
        }
        else if (input == 3)
        {
            example = Easy;
        }
        else if (input == 4)
        {
            example = Hard;
        }
        else if (input == 5)
        {
            example = impossible;
        }

    }
    else if (input == 2)
    {
        cout << "Enter the configuration of your puzzle and enter a \"0\" for the blank\n";
        cout << "Enter 3 numbers at a time separating them with a space\n";
        cout << "First Row:  ";
        cin >> input >> input2 >> input3;

        example[0][0] = input;
        example[0][1] = input2;
        example[0][2] = input3;

        cout << "\nSecond Row: ";
        cin >> input >> input2 >> input3;

        example[1][0] = input;
        example[1][1] = input2;
        example[1][2] = input3;

        cout << "\nThird Row:  ";
        cin >> input >> input2 >> input3;

        example[2][0] = input;
        example[2][1] = input2;
        example[2][2] = input3;
    }

    for(int i = 0; i < example.size(); i++){
        cout<<endl;
        for(int j = 0; j < example[i].size(); j++){
            cout << example[i][j] << " ";
        }
    }

    cout <<endl;

    cout << "\nEnter the number of the algorithm you wish to search with: " << endl;

    cout << "1. Uniform Cost Search\n" << "2. Misplaced Tile Heuristic\n" << "3. Manhattan Distance Heuristic\n\n";

    cin >> input;

    heuristic = input;

    end = search(example, heuristic);

    if (end.puzzle == failed)
    {
        cout << "No Solution" << endl;
    }

    return 0;
}

