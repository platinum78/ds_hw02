#include <stdio.h>
#include "./include/maze.h"
#include "./include/path.h"
#include "./include/pathfinder.h"
#include "./include/utils.h"

int main(void)
{
    FILE* input = fopen("./io/hw2_input.txt", "r");
    FILE* output = fopen("./io/hw2_output.txt", "w");
    printf("File opened. \n");

    // Read the maze and make a clone
    Maze* maze = MazeInit();
    MazeRead(input, maze);
    Maze* maze_clone = MazeClone(maze);
    

    Path* pWarplessPath = WarplessPath(maze, BEGIN_START, DESTINATION_EXIT);
    
    PrintMazeMap(maze);
    PrintPath(pWarplessPath, PATH_FORWARD);
    PrintPath(pWarplessPath, PATH_BACKWARD);


    return 0;
}