#ifndef __DATA_STRUCTURE_H__
#define __DATA_STRUCTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct Point
{
    int row;
    int col;
} Point;


int PointCmp(Point p1, Point p2)
{
    if (p1.row != p2.row)
        return 0;
    if (p1.col != p2.col)
        return 0;
    return 1;
}


typedef struct Maze
{
    int height;
    int width;
    int map_capacity;
    int map_len;
    Point start;
    Point exit;
    int* map;
} Maze;


Maze* MazeInit(void);
void MazeRead(FILE*, Maze*);
void MazeTrim(Maze*);
int MazePointVal(Maze*, int, int);


Maze* MazeInit(void)
{
    /*
     * Allocate and initialize Maze instance.
     * Initially create an integer array with size of 64.
     */
    
    Maze* pMaze = (Maze*)malloc(sizeof(Maze));
    pMaze->height = 0;
    pMaze->width = 0;
    pMaze->map_capacity = 64;  // Initially the map has length of 64.
    pMaze->map_len = 0;
    pMaze->map = (int*)malloc(sizeof(int) * 64);  // Initially allocate array of 64 characters
    
    return pMaze;
}


void MazeRead(FILE* input, Maze* maze)
{
    // Set buffer for function operation
    int idx = 0;
    int width = 0, height = 0;
    char cBuf;
    int nBuf;
    fseek(input, 0, SEEK_SET);

    // First loop iteration: read only the first line and find # of elements in a row
    while (1)
    {
        fread(&cBuf, 1, 1, input);

        // Double the map if it is full
        if (maze->map_len == maze->map_capacity)
        {
            (maze->map_capacity) *= 2;
            maze->map = (int*)realloc(maze->map, sizeof(int) * (maze->map_capacity));
            printf("Map doubled: current capacity %d \n", maze->map_capacity);
        }


        // Filter input by character
        // Start: -1, End: -2, Wall: -3, Warp: -4
        if (cBuf == '0')
        {
            (maze->map)[idx++] = 0;
            width++; (maze->map_len)++;
        }
        else if (cBuf == '*')
        {
            (maze->map)[idx++] = -3;
            width++; (maze->map_len)++;
        }
        else if (cBuf == 'S' || cBuf == 's')
        {
            (maze->map)[idx++] = cBuf;
            width++; (maze->map_len)++;
        }
        else if (cBuf == 'E' || cBuf == 'e')
        {
            (maze->map)[idx++] = -2;
            width++; (maze->map_len)++;
        }
        else if (cBuf == 'W' || cBuf == 'w')
        {
            (maze->map)[idx++] = -4;
            width++; (maze->map_len)++;
        }
        else if (cBuf == '\n')
        {
            // printf("\n");
            break;
        }
    }

    // Second loop iteration: read until EOF
    while (fread(&cBuf, 1, 1, input) != 0)
    {
        // Double the map if it is full
        if (maze->map_len == maze->map_capacity)
        {
            (maze->map_capacity) *= 2;
            maze->map = (int*)realloc(maze->map, sizeof(int) * (maze->map_capacity));
            printf("Map doubled: current capacity %d \n", maze->map_capacity);
        }

        if (cBuf == '0')
        {
            (maze->map)[idx++] = 0;
            (maze->map_len)++;
        }
        else if (cBuf == '*')
        {
            (maze->map)[idx++] = -3;
            (maze->map_len)++;
        }
        else if (cBuf == 'S' || cBuf == 's')
        {
            (maze->map)[idx++] = -1;
            (maze->map_len)++;
        }
        else if (cBuf == 'E' || cBuf == 'e')
        {
            (maze->map)[idx++] = -2;
            (maze->map_len)++;
        }
        else if (cBuf == 'W' || cBuf == 'w')
        {
            (maze->map)[idx++] = -4;
            (maze->map_len)++;
        }
        else if (cBuf == '\n')
        {
            // printf("\n");
        }
    }

    // Now we got the whole file. Calculate the dimension of the map.
    height = idx / width;
    maze->height = height;
    maze->width = width;

    // Now find start, exit, and warp points of the maze.
    int row, col;
    for (row = 0; row < maze->height; row++)
    {
        for (col = 0; col < maze->width; col++)
        {
            nBuf = MazePointVal(maze, row, col);
            if (nBuf == -1)
                maze->start = (Point){ .row = row, .col = col };
            else if (nBuf == -2)
                maze->exit = (Point){ .row = row, .col = col };
        }
    }

    printf("Maze read complete. \n");
    printf("Width: %d, height: %d \n", maze->width, maze->height);

    MazeTrim(maze);
}


void MazeTrim(Maze* maze)
{
    int nMazeLen = (maze->width) * (maze->height);
    if (nMazeLen != (maze->map_len))
    {
        // Allocate new map with exact length
        int* cpNewMap = (int*)malloc(sizeof(int) * nMazeLen);

        // Copy the values from previous map
        int idx = 0;
        for (idx = 0; idx < nMazeLen; idx++)
            cpNewMap[idx] = (maze->map)[idx];

        // Now maze->map is obsolete. Replace it with new map.
        free(maze->map);
        maze->map_len = nMazeLen;
        maze->map_capacity = nMazeLen;
        maze->map = cpNewMap;
    }
}


int MazePointVal(Maze* maze, int row, int col)
{
    return (maze->map)[row * (maze->width) + col];
}


void MazeMarkDist(Maze* maze, int row, int col, int dist)
{
    (maze->map)[row * (maze->width) + col] = dist;
}


Maze* MazeClone(Maze* maze)
{
    Maze* pMaze = (Maze*)malloc(sizeof(Maze));
    pMaze->map = (int*)malloc(sizeof(int) * (maze->height) * (maze->width));
    
    int idx = 0;
    for (idx = 0; idx < (maze->height) * (maze->width); idx++)
        (pMaze->map)[idx] = (maze->map)[idx];
    pMaze->width = maze->width;
    pMaze->height = maze->height;
    pMaze->start = maze->start;
    pMaze->exit = maze->exit;
    pMaze->map_len = maze->map_len;
    pMaze->map_capacity = maze->map_capacity;

    return pMaze;
}

Maze* MazeCopy(Maze* maze_src, Maze* maze_dst)
{
    free(maze_dst->map);
    maze_dst->map = (int*)malloc(sizeof(int) * (maze_src->height) * (maze_src->width));

    int idx;
    for (idx = 0; idx < (maze_src->height) * (maze_src->width); idx++)
        (maze_dst->map)[idx] = (maze_src->map)[idx];
    maze_dst->width = maze_src->width;
    maze_dst->height = maze_src->height;
    maze_dst->start = maze_src->start;
    maze_dst->exit = maze_src->exit;
    maze_dst->map_len = maze_src->map_len;
    maze_dst->map_capacity = maze_src->map_capacity;
}


#endif