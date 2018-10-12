#include <stdio.h>
#include <time.h>
#include "./include/maze.h"
#include "./include/path.h"
#include "./include/pathfinder.h"
#include "./include/utils.h"

int main(void)
{
    clock_t tic = clock();
    FILE* input = fopen("./io/hw2_input.txt", "r");
    FILE* output = fopen("./io/hw2_output.txt", "w");
    printf("File opened. \n");

    // Read the maze and make a clone
    Maze* maze = MazeInit();
    MazeRead(input, maze);
    

    Path* pWarplessPath = WarplessPath(maze, BEGIN_START, DESTINATION_EXIT);
    Path* pWarpablePath = WarpablePath(maze);

    if (pWarplessPath != NULL && pWarpablePath != NULL)
    {
        // Print each path to console
        printf("Warpless Path: ");
        PrintPath(pWarplessPath, PATH_FORWARD);
        printf("Warpable Path: ");
        PrintPath(pWarpablePath, PATH_FORWARD);

        if (pWarpablePath->len < pWarplessPath->len)
            WritePath(output, pWarpablePath, PATH_FORWARD);
        else
            WritePath(output, pWarplessPath, PATH_FORWARD);
    }
    else if (pWarplessPath != NULL && pWarpablePath == NULL)
    {
        // Print each path to console
        printf("Warpless Path: ");
        PrintPath(pWarplessPath, PATH_FORWARD);
        printf("Warpable Path: none");

        WritePath(output, pWarplessPath, PATH_FORWARD);
    }
    else if (pWarplessPath == NULL && pWarpablePath != NULL)
    {
        // Print each path to console
        printf("Warpless Path: none");
        printf("Warpable Path: ");
        PrintPath(pWarpablePath, PATH_FORWARD);

        // Write path to output
        WritePath(output, pWarplessPath, PATH_FORWARD);
    }
    else
    {
        WriteNull(output);
        return 0;
    }
    clock_t toc = clock();

    printf("%f seconds elapsed! \n", (double)(toc-tic)/CLOCKS_PER_SEC * 1000);

    return 0;
}