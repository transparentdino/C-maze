# C++ Maze Solver Lab

This lab provides a framework for students to implement maze-solving algorithms in C++. The lab includes:

1. A maze generator (`maze.cpp`) that creates random mazes and saves them to files
2. A template for students to implement their own maze-solving algorithms (`solver_template.cpp`)
3. A solution implementation for instructors (`solver_solution.cpp`)

## Instructions for Instructors

1. First, compile and run the maze generator to create maze files:
   ```
   g++ maze.cpp -o maze_generator
   ./maze_generator
   ```

2. Have students use the `solver_template.cpp` as their starting point. They should implement the `solveMaze` function.

3. You can use `solver_solution.cpp` to verify solutions or for demonstration purposes.

## Instructions for Students

1. Copy `solver_template.cpp` to `mysolver.cpp` to begin your implementation.

2. Read the comments in the code to understand what you need to implement.

3. Complete the `solveMaze` function to find a path from the start to the end of the maze.

4. Compile and run your solver:
   ```
   g++ mysolver.cpp -o maze_solver
   ./maze_solver
   ```

5. When prompted, enter the filename of a maze (e.g., "maze_10x10.txt").

6. Test your solver with different start and end points.

7. The program will show you:
   - The time it took to solve the maze
   - The length of the solution path
   - A visual representation of the maze with the solution path

## Requirements

Students should:

1. Implement a working maze-solving algorithm (e.g., BFS, DFS, A*, etc.)
2. Handle walls in the maze correctly
3. Return the path from start to end if one exists
4. Return an empty path if no solution exists

## Evaluation Criteria

Solutions will be evaluated based on:

1. Correctness (finds a valid path when one exists)
2. Efficiency (time to solve)
3. Code quality and readability
4. Proper error handling

## Maze File Format

Each maze file contains:
- The dimensions of the maze (rows and columns)
- The wall configuration for each cell (top, right, bottom, left)

## Implementation Notes

- The maze is represented as a grid of cells, each with 4 walls (top, right, bottom, left)
- A wall value of `true` means the wall exists, `false` means there's a passage
- Students need to check if there's a wall between cells before moving
- Movement is only allowed in four directions: up, right, down, left

Good luck and happy maze solving! 