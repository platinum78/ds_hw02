#include <stdio.h>
#include <time.h>
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
    

    Path* pWarplessPath = WarplessPath(maze, START, EXIT);
    Path* pWarpablePath = WarpablePath(maze);

    WritePath(output, pWarplessPath, pWarpablePath);
    PathDelete(pWarpablePath); PathDelete(pWarplessPath);
    MazeDelete(maze);
    fclose(input); fclose(output);
    clock_t toc = clock();

    printf("%f milliseconds elapsed! \n", (double)(toc-tic)/CLOCKS_PER_SEC * 1000);

    return 0;
}