#ifndef __DATATYPES_C__
#define __DATATYPES_C__

#include <stdio.h>
#include <stdlib.h>
#include "./datatypes.h"

////////////////////////////////////////////////////////////
// Point
////////////////////////////////////////////////////////////

Point* PointInit(int row, int col)
{
    Point* pPoint = (Point*)malloc(sizeof(Point));
    pPoint->row = row;
    pPoint->col = col;
    return pPoint;
}

void PointSet(Point* point, int row, int col)
{
    point->row = row; point->col = col;
}

void PointMove(Point* point, int drow, int dcol)
{
    point->row += drow;
    point->col += dcol;
}

int PointCmp(Point p1, Point p2)
{
    if (p1.row != p2.row)
        return 0;
    if (p1.col != p2.col)
        return 0;
    return 1;
}


////////////////////////////////////////////////////////////
// Maze
////////////////////////////////////////////////////////////

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
    Point PointBuf;
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
            (maze->map)[idx++] = -1;
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
            PointSet(&PointBuf, row, col);
            nBuf = MazePointVal(maze, PointBuf);
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

Point MazeStart(Maze* maze)
{
    return maze->start;
}

Point MazeExit(Maze* maze)
{
    return maze->exit;
}

int MazePointVal(Maze* maze, Point point)
{
    return (maze->map)[point.row * (maze->width) + point.col];
}

void MazeMarkDist(Maze* maze, Point point, int dist)
{
    (maze->map)[point.row * (maze->width) + point.col] = dist;
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

void MazeCopy(Maze* maze_src, Maze* maze_dst)
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

void MazeDelete(Maze* maze)
{
    free(maze->map);
    maze->map = NULL;
    free(maze);
}


////////////////////////////////////////////////////////////
// LinkedPoint
////////////////////////////////////////////////////////////

LinkedPoint* LinkedPointInit()
{
    LinkedPoint* pLPoint = (LinkedPoint*)malloc(sizeof(LinkedPoint));
    pLPoint->previous = NULL;
    pLPoint->next = NULL;
    return pLPoint;
}


////////////////////////////////////////////////////////////
// Path
////////////////////////////////////////////////////////////

Path* PathInit()
{
    Path* pPath = (Path*)malloc(sizeof(Path));
    pPath->len = 0;
    pPath->head = NULL;
    pPath->tail = NULL;

    return pPath;
}

void PathAddPasspoint(Path* path, Point point, int pos)
{
    // Allocate LinkedPoint instance and set the point.
    LinkedPoint* pLPoint = LinkedPointInit();
    pLPoint->point = point;

    // Put it either at the head or tail.
    if (pos == PATH_TAIL)
    {
        if (path->len == 0)
        {
            path->head = pLPoint;
            path->tail = pLPoint;
        }
        else
        {
            pLPoint->previous = path->tail;
            (path->tail)->next = pLPoint;
            path->tail = pLPoint;
        }
    }
    else if (pos == PATH_HEAD)
    {
        if (path->len == 0)
        {
            path->head = pLPoint;
            path->tail = pLPoint;
        }
        else
        {
            pLPoint->next = path->head;
            (path->head)->previous = pLPoint;
            path->head = pLPoint;
        }
    }

    // Increase path length by one.
    (path->len)++;
}

void PathFlip(Path* path)
{
    int nPathLen = path->len;
    LinkedPoint* pLPHere, *pLPNext, *pLPSwap;
    pLPNext = path->head;
    int idx;
    for (idx = 0; idx < nPathLen; idx++)
    {
        pLPHere = pLPNext;
        pLPNext = pLPNext->next;
        
        pLPSwap = pLPHere->next;
        pLPHere->next = pLPHere->previous;
        pLPHere->previous = pLPSwap;
    }
    pLPSwap = path->head;
    path->head = path->tail;
    path->tail = pLPSwap;
}

// Merge two paths into a new one
Path* PathMerge(Path* path1, Path* path2)
{
    // Get total length of new path
    int nPathLen = path1->len + path2->len;
    printf("Length of new path: %d \n", nPathLen);

    // Create a new Path instance
    Path* pPath = PathInit();

    // Copy first object of path 1
    LinkedPoint* pLPNext = path1->head;
    printf("Checkpoint Passed! \n");
    PathAddPasspoint(pPath, pLPNext->point, PATH_TAIL);
    pLPNext = pLPNext->next;
    // Continue copying objects from path 1
    int idx = 0;
    for (idx = 1; idx < path1->len; idx++)
    {
        PathAddPasspoint(pPath, pLPNext->point, PATH_TAIL);
        pLPNext = pLPNext->next;
    }

    pLPNext = path2->head;
    for (idx = 0; idx < path2->len; idx++)
    {
        PathAddPasspoint(pPath, pLPNext->point, PATH_TAIL);
        pLPNext = pLPNext->next;
    }

    return pPath;
}

// Delete path from heap
void PathDelete(Path* path)
{
    if (path != NULL)
    {
        LinkedPoint* PointBuf = path->head;
        LinkedPoint* PointNext;
        int idx = 0;

        while (PointBuf->next != NULL)
        {
            idx++;
            PointNext = PointBuf->next;
            free(PointBuf);
            PointBuf = PointNext;
        }
        
        free(path);
    }
}


////////////////////////////////////////////////////////////
// PointQueue
////////////////////////////////////////////////////////////

PointQueue* PointQueueInit()
{
    PointQueue* pQueue = (PointQueue*)malloc(sizeof(PointQueue));
    pQueue->len = 0;
    pQueue->head = NULL;
    pQueue->tail = NULL;

    return pQueue;
}

void PointQueuePush(PointQueue* queue, Point point)
{
    // Allocate a LinkedPoint instance
    LinkedPoint* pLPoint = (LinkedPoint*)malloc(sizeof(LinkedPoint));

    // Copy values into new instance.
    (pLPoint->point).row = point.row;
    (pLPoint->point).col = point.col;

    if (queue->len == 0)
    {
        // Initialize if queue was empty before.
        queue->head = pLPoint;
        queue->tail = pLPoint;
    }
    else
    {
        // Simply add the new LinkedPoint onto the edge of queue.
        (queue->tail)->next = pLPoint;
        queue->tail = pLPoint;
    }
    
    (queue->len)++;
}

Point PointQueuePop(PointQueue* queue)
{
    // Make a Point object to return values from deleted point
    Point newPoint;

    // Delete the first element of the queue
    if (queue->len == 0)
    {
        // If the queue is empty, removal operation should raise an error.
        newPoint = (Point){ -1, -1 };
        return newPoint;
    }
    else if (queue->len == 1)
    {
        // If queue has only one element, removing would leave no element in the queue.
        newPoint.row = (queue->head->point).row;
        newPoint.col = (queue->head->point).col;
        free(queue->head);
        queue->head = queue->tail = NULL;
        (queue->len)--;
    }
    else
    {
        newPoint.row = (queue->head->point).row;
        newPoint.col = (queue->head->point).col;
        LinkedPoint* pLPoint = queue->head->next;  // A buffer to store the pointer to second element
        free(queue->head);
        queue->head = pLPoint;
        (queue->len)--;
    }

    return newPoint;
}

// Returns the length of PointQueue
int PointQueueLen(PointQueue* queue)
{
    return queue->len;
}

#endif