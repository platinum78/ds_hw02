#ifndef __UTILS_H__
#define __UTILS_H__

#define PATH_FORWARD 1
#define PATH_BACKWARD 2

#include <stdio.h>
#include <stdlib.h>
#include "./maze.h"


void PrintMazeMap(Maze* maze)
{
    int i, j;
    int num;

    for (i = 0; i < maze->height; i++)
    {
        for (j = 0; j < maze->width; j++)
        {
            num = MazePointVal(maze, i, j);

            if (num == -3)
                printf("*       ");
            else if (num == -2)
                printf("e(%03d)   ", num);
            else if (num == -1)
                printf("s       ");
            else
                printf("%-8d", num);
        }
        printf("\n\n");
    }
}

void PrintPath(Path* path, int direction)
{
    int idx;
    int nPathLen = path->len;
    printf("nPathLen: %d \n", nPathLen);
    
    if (direction == PATH_FORWARD)
    {
        LinkedPoint* pLPoint = path->head;

        for (idx = 0; idx < nPathLen; idx++)
        {
            printf("(%d,%d) ", (pLPoint->point).row, (pLPoint->point).col);
            pLPoint = pLPoint->next;
        }
    }
    else if (direction == PATH_BACKWARD)
    {
        LinkedPoint* pLPoint = path->tail;

        for (idx = 0; idx < nPathLen; idx++)
        {
            printf("(%d,%d) ", (pLPoint->point).row, (pLPoint->point).col);
            pLPoint = pLPoint->previous;
        }
    }
    printf("\n");
}


void WritePath(FILE* output, Path* path, int direction)
{
    int idx;
    int nPathLen = path->len;
    printf("nPathLen: %d \n", nPathLen);
    fprintf(output, "%d \n", path->len - 1);
    if (direction == PATH_FORWARD)
    {
        LinkedPoint* pLPoint = path->head;

        for (idx = 0; idx < nPathLen; idx++)
        {
            fprintf(output, "(%d,%d)", (pLPoint->point).row, (pLPoint->point).col);
            if (idx < nPathLen - 1)
                fprintf(output, ", ");
            pLPoint = pLPoint->next;
        }
    }
    else if (direction == PATH_BACKWARD)
    {
        LinkedPoint* pLPoint = path->tail;

        for (idx = 0; idx < nPathLen; idx++)
        {
            fprintf(output, "(%d,%d)", (pLPoint->point).row, (pLPoint->point).col);
            if (idx < nPathLen - 1)
                fprintf(output, ", ");
            pLPoint = pLPoint->previous;
        }
    }
    printf("\n");
}


void WriteNull(FILE* output)
{
    fprintf(output, "NULL \n");
}

#endif