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

    // Read the maze
    Maze* maze = MazeInit();
    MazeRead(input, maze);

    Path* pWarpablePath = WarpablePath(maze);
    Path* pWarplessPath = WarplessPath(maze);


    return 0;
}