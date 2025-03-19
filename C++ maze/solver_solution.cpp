#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// Directions: 0 = top, 1 = right, 2 = bottom, 3 = left
struct Cell {
    bool walls[4] = {true, true, true, true}; // top, right, bottom, left
};

class Maze {
public:
    int rows, cols;
    vector<vector<Cell>> grid;

    Maze(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<Cell>(cols));
    }

    // Helper function to check bounds
    bool inBounds(int r, int c) const {
        return (r >= 0 && r < rows && c >= 0 && c < cols);
    }

    // Load maze from file
    bool loadMaze(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error: Could not open file '" << filename << "' for reading." << endl;
            return false;
        }

        inFile >> rows >> cols;
        grid.resize(rows, vector<Cell>(cols));
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                for (int k = 0; k < 4; k++) {
                    bool wall;
                    inFile >> wall;
                    grid[i][j].walls[k] = wall;
                }
            }
        }
        inFile.close();
        return true;
    }

    // Display the maze in ASCII art with the solution path
    void displayMaze(const vector<pair<int,int>> &path, pair<int,int> start, pair<int,int> end) const {
        // Create a grid to mark solution path for quick lookup
        vector<vector<bool>> inPath(rows, vector<bool>(cols, false));
        for(auto &p : path) {
            inPath[p.first][p.second] = true;
        }
        
        // For each row of cells
        for (int i = 0; i < rows; i++) {
            // Print the top borders of the row
            for (int j = 0; j < cols; j++) {
                cout << "+";
                if(grid[i][j].walls[0])
                    cout << "---";
                else
                    cout << "   ";
            }
            cout << "+\n";
            
            // Print the cell contents and vertical walls
            for (int j = 0; j < cols; j++) {
                // For the first column, always print the left border
                if(j == 0) cout << "|";
                string cellContent = "   ";
                if(i == start.first && j == start.second)
                    cellContent = " S ";
                else if(i == end.first && j == end.second)
                    cellContent = " E ";
                else if(inPath[i][j])
                    cellContent = " * ";
                cout << cellContent;
                if(grid[i][j].walls[1])
                    cout << "|";
                else
                    cout << " ";
            }
            cout << "\n";
        }
        // Print the bottom border of the maze
        for (int j = 0; j < cols; j++) {
            cout << "+";
            if(grid[rows-1][j].walls[2])
                cout << "---";
            else
                cout << "   ";
        }
        cout << "+\n";
    }

    // Display maze without solution path
    void displayMaze() const {
        vector<pair<int,int>> emptyPath;
        displayMaze(emptyPath, {-1, -1}, {-1, -1});
    }
};

/* 
 * SOLUTION: BFS Implementation to solve the maze
 */
vector<pair<int,int>> solveMaze(const Maze& maze, pair<int,int> start, pair<int,int> end) {
    int rows = maze.rows;
    int cols = maze.cols;
    
    // BFS algorithm
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int,int>>> parent(rows, vector<pair<int,int>>(cols, {-1, -1}));
    queue<pair<int,int>> q;
    
    // Directions: 0 = top, 1 = right, 2 = bottom, 3 = left
    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};

    // Start BFS
    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        
        // Check if we reached the end
        if (r == end.first && c == end.second) 
            break;
            
        // Explore all four directions
        for (int d = 0; d < 4; d++) {
            // Check if there's a wall in this direction
            if (maze.grid[r][c].walls[d]) 
                continue;
                
            int nr = r + dr[d];
            int nc = c + dc[d];
            
            // Check bounds and if already visited
            if (!maze.inBounds(nr, nc) || visited[nr][nc]) 
                continue;
                
            // Mark as visited and set parent
            visited[nr][nc] = true;
            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }
    
    // Reconstruct path from end to start
    vector<pair<int,int>> path;
    if (!visited[end.first][end.second]) 
        return path; // No solution
        
    // Create path by traversing parent pointers from end to start
    pair<int,int> current = end;
    while (current != start) {
        path.push_back(current);
        current = parent[current.first][current.second];
    }
    path.push_back(start);
    
    // Reverse to get path from start to end
    reverse(path.begin(), path.end());
    
    return path;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "==== Maze Solver Lab (SOLUTION) ====" << endl;
    cout << "This program loads a maze and solves it using BFS." << endl;
    
    string filename;
    cout << "Enter the maze file to load (e.g., maze_10x10.txt): ";
    cin >> filename;
    
    Maze maze(0, 0); // Will be resized when loading
    if (!maze.loadMaze(filename)) {
        return 1;
    }
    
    cout << "Maze loaded successfully! Dimensions: " << maze.rows << "x" << maze.cols << endl;
    cout << "Current maze:" << endl;
    maze.displayMaze();
    
    char continueSolving = 'y';
    while (continueSolving == 'y' || continueSolving == 'Y') {
        int sr, sc, er, ec;
        cout << "Enter starting cell (row col) [0-indexed, e.g., 0 0]: ";
        cin >> sr >> sc;
        cout << "Enter ending cell (row col) [0-indexed, e.g., " << maze.rows-1 << " " << maze.cols-1 << "]: ";
        cin >> er >> ec;
        
        if (sr < 0 || sr >= maze.rows || sc < 0 || sc >= maze.cols || 
            er < 0 || er >= maze.rows || ec < 0 || ec >= maze.cols) {
            cout << "Invalid start or end positions. Please try again." << endl;
            continue;
        }
        
        // Time the maze solving process
        auto solveStart = chrono::high_resolution_clock::now();
        vector<pair<int,int>> solution = solveMaze(maze, {sr, sc}, {er, ec});
        auto solveEnd = chrono::high_resolution_clock::now();
        auto solveDuration = chrono::duration_cast<chrono::microseconds>(solveEnd - solveStart);
        
        if (solution.empty()) {
            cout << "No solution exists from (" << sr << "," << sc << ") to (" 
                 << er << "," << ec << ")." << endl;
        } else {
            // Display the solving time
            if (solveDuration.count() < 1000) {
                cout << "Maze solved in " << solveDuration.count() << " microseconds!" << endl;
            } else {
                // Convert to milliseconds for readability if time is more than 1ms
                double msTime = solveDuration.count() / 1000.0;
                cout << "Maze solved in " << fixed << setprecision(2) << msTime << " milliseconds!" << endl;
            }
            
            // Display path length information
            cout << "Solution path length: " << solution.size() << " cells" << endl;
            
            maze.displayMaze(solution, {sr, sc}, {er, ec});
        }
        
        cout << "Try solving with different endpoints? (y/n): ";
        cin >> continueSolving;
    }
    
    cout << "Thanks for using the Maze Solver Lab!" << endl;
    return 0;
} 