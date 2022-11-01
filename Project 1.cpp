#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

vector<vector<int>> goalState = {{1, 2, 3},  
                                 {4, 5, 6},
                                 {7, 8, 0}}; 

vector<vector<int>> failed = {{0, 0, 0},  
                              {0, 0, 0},
                              {0, 0, 0}};

struct Node
{
    vector<vector<int>> puzzle;
    int PositionOfZeroRow;
    int PositionOfZeroColumn;
    vector<string> solutionPath;
    int heuristic;
    
};

struct Heuristic
{
    bool operator()(const Node &a, const Node &b)
    {
        return ((a.heuristic + a.solutionPath.size()) > (b.heuristic+ b.solutionPath.size()));
    }
};

int getHeuristic(int whichHeur, vector<vector<int>> temp)
{
    int heur = 0;

    if (whichHeur == 1)//uniform cost sets h(n) to 0
    {
        heur = 0;
    }

    if (whichHeur == 2)
    {
       for(int i = 0; i < temp.size(); i++){
            for(int j = 0; j < temp.size(); j++){
                if (temp[i][j] != goalState[i][j])
                { 
                    if (temp[i][j] != 0)
                    {
                        heur++;
                    }
                }
           }
        }
    }

    return heur;
}
Node Get0Position(vector<vector<int>> child)
{
    Node temp;
    temp.puzzle = child;
    
    for(int i = 0; i < temp.puzzle.size(); i++){
        for(int j = 0; j < temp.puzzle.size(); j++){
            if(temp.puzzle[i][j] == 0){// finds the position of the "0" 
                temp.PositionOfZeroRow = i;
                temp.PositionOfZeroColumn = j;
            }
        }
    }
    
    return temp;
}

Node search(vector<vector<int>> problem, int whichHeuristic)
{   
    priority_queue<Node, vector<Node>, Heuristic> test;
    set<vector<vector<int>>> trackDupes;
    int dupeSize;
    Node currstate;
    Node temp;
    Node failstate;
    failstate.puzzle = failed;
    int maxQueueSize = test.size();
    int numNodesExpanded = 0;
    int tempNum = 0;

    temp = (Get0Position(problem));
    temp.heuristic = getHeuristic(whichHeuristic, problem);
    test.push(temp);
    trackDupes.insert(problem);

    while (!test.empty())
    {
        tempNum = test.size();
        maxQueueSize = max(maxQueueSize, tempNum);

        currstate = test.top();
        trackDupes.insert(currstate.puzzle);

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

            cout << endl << "Solution depth was " << currstate.solutionPath.size() << endl;
            cout << "Number of nodes expanded: " << numNodesExpanded-1 << endl;
            cout << "Max queue size: " << maxQueueSize << endl;

            return currstate;
        }

        for(int i = 0; i < currstate.puzzle.size(); i++)
        {
            cout << endl;
            for(int j = 0; j < currstate.puzzle.size(); j++)
            {
                cout << currstate.puzzle[i][j] << " ";
            }
        }

        cout << endl;

        test.pop();

        if (currstate.PositionOfZeroColumn != 0) //checks if can swap left
        {   
            temp = currstate;

            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1]; //sets the number on the left to the 0 position
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1] = 0; //sets 0 to the position of the swapped number
            temp.PositionOfZeroColumn--;

            dupeSize = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (dupeSize != trackDupes.size())
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

    cout << endl << "No Solution" << endl;

    return failstate;
}


int main(){

    Node end;
    int heuristic = 0;
    int input;

    vector<vector<int>> example = {{1, 2, 3},  
                                   {5, 0, 6},
                                   {4, 7, 8}}; 
    cout << "Problem:";

    for(int i = 0; i < example.size(); i++){
        cout<<endl;
        for(int j = 0; j < example[i].size(); j++){
            cout << example[i][j] << " ";
        }
    }

    cout <<endl;

    cout << "Enter the number of the algorithm you wish to search with: " << endl;

    cout << "1. Uniform Cost Search\n" << "2. Misplaced Tile Heuristic\n" << "3. Manhattan Distance Heuristic\n\n";

    cin >> input;

    heuristic = input;

    end = search(example, heuristic);

    /*
    if (end.puzzle != failed)
    {
        for(int i = 0; i < end.puzzle.size(); i++){
            cout << endl;
            for(int j = 0; j < end.puzzle.size(); j++){
              cout << end.puzzle[i][j] << " ";
           }
        }

        cout<< endl << "Solution Path:" << endl;

        for (int i = 0; i < end.solutionPath.size(); i++)
        {
            cout << end.solutionPath[i]<< " ";
        }

        cout<< endl;

        cout << "Depth: " << end.solutionPath.size()<<endl;

        cout << "NumMisplaced Tiles: " <<end.NumMisplacedTiles<<endl;

    }

    */

    if (end.puzzle == failed)
    {
        cout << "No Solution" << endl;
    }
    /*
    else{
        cout << "Solution Path: ";
        for (int i = 0; i < end.solutionPath.size(); i++)
        {
            cout << end.solutionPath[i]<< " ";
        }
    }
        */
    return 0;
}

