#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// Directions: 0 = top, 1 = right, 2 = bottom, 3 = left
struct Cell {
    bool walls[4] = {true, true, true, true}; // top, right, bottom, left
    bool visited = false;
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

    // Generate maze using recursive backtracking (iterative with stack)
    void generateMaze() {
        // Initialize random seed
        srand(time(nullptr));
        
        // Reset all cells
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j].visited = false;
                for (int k = 0; k < 4; k++) {
                    grid[i][j].walls[k] = true;
                }
            }
        }
        
        // Using stack for DFS
        stack<pair<int,int>> st;
        grid[0][0].visited = true;
        st.push({0,0});
        
        int dr[4] = {-1, 0, 1, 0};
        int dc[4] = {0, 1, 0, -1};

        while(!st.empty()) {
            auto [r, c] = st.top();
            vector<tuple<int,int,int>> neighbors; // tuple: (nr, nc, direction)
            
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (inBounds(nr, nc) && !grid[nr][nc].visited) {
                    neighbors.push_back(make_tuple(nr, nc, d));
                }
            }
            
            if(neighbors.empty()) {
                st.pop();
            } else {
                auto [nr, nc, d] = neighbors[rand() % neighbors.size()];
                // Remove wall between current cell and neighbor
                grid[r][c].walls[d] = false;
                int opposite = (d + 2) % 4;
                grid[nr][nc].walls[opposite] = false;
                grid[nr][nc].visited = true;
                st.push({nr, nc});
            }
        }
    }

    // Save maze to file so students can load it
    void saveMaze(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Could not open file for writing." << endl;
            return;
        }

        outFile << rows << " " << cols << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                for (int k = 0; k < 4; k++) {
                    outFile << grid[i][j].walls[k] << " ";
                }
            }
        }
        outFile.close();
        cout << "Maze saved to " << filename << endl;
    }

    // Load maze from file
    bool loadMaze(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error: Could not open file for reading." << endl;
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

    // Display the maze in ASCII art. If a solution path is provided, mark it.
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

    // Display maze without solution path (for initial visualization)
    void displayMaze() const {
        vector<pair<int,int>> emptyPath;
        displayMaze(emptyPath, {-1, -1}, {-1, -1});
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "==== Maze Generator and Solver Framework ====" << endl;
    cout << "This program will generate mazes for students to solve." << endl;
    cout << "Students should implement their own solving algorithm in solver.cpp" << endl;
    cout << endl;

    char genNewMaze = 'y';

    while (genNewMaze == 'y' || genNewMaze == 'Y') {
        int r, c;
        cout << "Enter number of rows and columns for the maze (e.g., 5 5): ";
        cin >> r >> c;
        if (r <= 0 || c <= 0) {
            cout << "Invalid dimensions. Please enter positive numbers." << endl;
            continue;
        }
        
        Maze m(r, c);
        
        // Time the maze generation
        auto genStart = chrono::high_resolution_clock::now();
        m.generateMaze();
        auto genEnd = chrono::high_resolution_clock::now();
        auto genDuration = chrono::duration_cast<chrono::milliseconds>(genEnd - genStart);
        cout << "Maze generated in " << fixed << setprecision(2) << genDuration.count() << " milliseconds." << endl;
        
        // Save the maze to file for students to load
        string mazeFile = "maze_" + to_string(r) + "x" + to_string(c) + ".txt";
        m.saveMaze(mazeFile);
        
        // Display the unsolved maze
        cout << "Generated maze:" << endl;
        m.displayMaze();
        
        cout << "Do you want to generate a new maze? (y/n): ";
        cin >> genNewMaze;
    }
    
    cout << "Goodbye! Students can now run solver.cpp with the generated maze files." << endl;
    return 0;
} 