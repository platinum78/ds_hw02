#ifndef __UTILS_H__
#define __UTILS_H__

#define PATH_FORWARD 1
#define PATH_BACKWARD 2

#include <stdio.h>
#include <stdlib.h>
#include "./datatypes.h"
#include "./pathfinder.h"


void PrintMazeMap(Maze* maze);
void PrintPath(Path* path, int direction);
void WritePath(FILE* output, Path* pWarplessPath, Path* pWarpablePath);
void WritePathWorker(FILE* output, Path* path, int direction);
void WriteNull(FILE* output);


void PrintMazeMap(Maze* maze)
{
    int i, j;
    int num;
    Point PointBuf;

    for (i = 0; i < maze->height; i++)
    {
        for (j = 0; j < maze->width; j++)
        {
            PointSet(&PointBuf, i, j);
            num = MazePointVal(maze, PointBuf);

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


void WritePath(FILE* output, Path* pWarplessPath, Path* pWarpablePath)
{
    if (pWarplessPath != NULL && pWarpablePath != NULL)
    {
        // Print each path to console
        printf("Warpless Path: ");
        PrintPath(pWarplessPath, PATH_FORWARD);
        printf("Warpable Path: ");
        PrintPath(pWarpablePath, PATH_FORWARD);

        if (pWarpablePath->len < pWarplessPath->len)
            WritePathWorker(output, pWarpablePath, PATH_FORWARD);
        else
            WritePathWorker(output, pWarplessPath, PATH_FORWARD);
    }
    else if (pWarplessPath != NULL && pWarpablePath == NULL)
    {
        // Print each path to console
        printf("Warpless Path: ");
        PrintPath(pWarplessPath, PATH_FORWARD);
        printf("Warpable Path: none \n");

        WritePathWorker(output, pWarplessPath, PATH_FORWARD);
    }
    else if (pWarplessPath == NULL && pWarpablePath != NULL)
    {
        // Print each path to console
        printf("Warpless Path: none");
        printf("Warpable Path: ");
        PrintPath(pWarpablePath, PATH_FORWARD);

        // Write path to output
        WritePathWorker(output, pWarplessPath, PATH_FORWARD);
    }
    else
    {
        WriteNull(output);
    }
}


void WritePathWorker(FILE* output, Path* path, int direction)
{
    int idx;
    int nPathLen = path->len;
    // printf("nPathLen: %d \n", nPathLen);
    fprintf(output, "%d \n", path->len - 1);
    if (direction == PATH_FORWARD)
    {
        LinkedPoint* pLPoint = path->head;

        while (pLPoint->next != NULL)
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

        while (pLPoint->previous != NULL)
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