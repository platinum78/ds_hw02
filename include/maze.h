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
    char* map;
} Maze;


Maze* MazeInit(void);
void MazeRead(FILE*, Maze*);
void MazeTrim(Maze*);
char MazePoint(Maze*, int, int);


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
    pMaze->map = (char*)malloc(sizeof(char) * 64);  // Initially allocate array of 64 characters
}


void MazeRead(FILE* input, Maze* maze)
{
    // Set buffer for function operation
    int idx = 0;
    int width = 0, height = 0;
    char cBuf;
    fseek(input, 0, SEEK_SET);

    // First loop iteration: read only the first line and find # of elements in a row
    while (1)
    {
        fread(&cBuf, 1, 1, input);

        // Double the map if it is full
        if (maze->map_len == maze->map_capacity)
        {
            (maze->map_capacity) *= 2;
            maze->map = (char*)realloc(maze->map, sizeof(char) * (maze->map_capacity));
            printf("Map doubled: current capacity %d \n", maze->map_capacity);
        }

        if (cBuf == '0' || cBuf == 'W' || cBuf == 'w' || cBuf == '*' || 
            cBuf == 'E' || cBuf == 'e' || cBuf == 'S' || cBuf == 's')
        {
            (maze->map)[idx++] = cBuf;
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
            maze->map = (char*)realloc(maze->map, sizeof(char) * (maze->map_capacity));
            printf("Map doubled: current capacity %d \n", maze->map_capacity);
        }

        if (cBuf == '0' || cBuf == 'W' || cBuf == 'w' || cBuf == '*' || 
            cBuf == 'C' || cBuf == 'c' || cBuf == 'S' || cBuf == 's')
        {
            (maze->map)[idx++] = cBuf;
            (maze->map_len)++;
        }
        else if (cBuf == '\n')
        {
            // printf("\n");
        }
    }
    // printf("\n");

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
            cBuf = MazePoint(maze, row, col);
            if (cBuf == 'S' || cBuf == 's')
                maze->start = (Point){ .row = row, .col = col };
            else if (cBuf == 'E' || cBuf == 'e')
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
        char* cpNewMap = (char*)malloc(sizeof(char) * nMazeLen);

        // Copy the values from previous map
        int idx = 0;
        for (idx = 0; idx < nMazeLen; idx++)
            cpNewMap[idx] = (maze->map)[idx];

        // Now maze->map is obsolete. Replace it with new map.
        free(maze->map);
        maze->map = cpNewMap;
    }
}


char MazePoint(Maze* maze, int row, int col)
{
    return (maze->map)[row * (maze->width) + col];
}


void MazeMarkDist(Maze* maze, int row, int col, int dist)
{
    (maze->map)[row * (maze->width) + col] = '0' + dist;
}


#endif