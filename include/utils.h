#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include "./maze.h"


void PrintMazeMap(Maze* maze)
{
    int i, j;
    for (i = 0; i < maze->height; i++)
    {
        for (j = 0; j < maze->width; j++)
            printf("%c ", (maze->map)[i * (maze->width) + j]);
        printf("\n");
    }
}


#endif