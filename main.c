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
    

    Path* pWarplessPath = WarplessPath(maze, BEGIN_START, DESTINATION_EXIT);

    if (pWarplessPath == NULL)
    {
        WriteNull(output);
        return 0;
    }

    Path* pWarpablePath = WarpablePath(maze);
    
    PrintPath(pWarplessPath, PATH_FORWARD);
    PrintPath(pWarpablePath, PATH_FORWARD);
    
    if (pWarpablePath->len < pWarplessPath->len)
        WritePath(output, pWarpablePath, PATH_FORWARD);
    else
        WritePath(output, pWarplessPath, PATH_FORWARD);

    return 0;
}