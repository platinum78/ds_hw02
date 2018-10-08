#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#define TRUE 1
#define FALSE 0

#define BEGIN_START -1
#define BEGIN_EXIT -2

#define DESTINATION_EXIT -2
#define DESTINATION_WARP -4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./maze.h"
#include "./path.h"


typedef struct PointQueue
{
    int len;
    LinkedPoint* head;
    LinkedPoint* tail;
} PointQueue;


PointQueue* PointQueueInit();
void PointQueuePush(PointQueue* queue, Point point);
Point PointQueueDelete(PointQueue* queue);


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
        printf("%p \n", queue->tail);
        (queue->tail)->next = pLPoint;
        queue->tail = pLPoint;
    }
    
    (queue->len)++;
}


Point PointQueueDelete(PointQueue* queue)
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


Path* WarplessPath(Maze* maze, int start, int dest);
Path* WarpablePath(Maze* maze);
int CheckMobility(Maze* maze, Point point);
int BackTrack(Maze* maze, Point point, int dist);
int BackTrack_Vanilla(Maze* maze, Point point, int dist);


Path* WarplessPath(Maze* maze, int start, int dest)
{
    // Initialize instances that would help the function work.
    Path* pPath = PathInit();
    PointQueue* pQueue = PointQueueInit();
    Point PointOld, PointNew;
    Point PointStart, PointDest;  // Default destination is the exit point. Might change if wanted.
    
    // Set buffers for function operation
    int bDestFound = FALSE;
    int nDist = 0;
    int nMobility, nDirection;  // Buffer to store mobility and direction information
    int nQLenPrev, nQLenCurr;  // Buffers to store queue length
    int idx1, idx2, idx3, idx4;  // Counters for loops.

    // Trigger by pushing starting point of the maze
    if (start == BEGIN_START)
        PointStart = maze->start;
    else if (start == BEGIN_EXIT)
        PointStart = maze->exit;
    PointQueuePush(pQueue, PointStart);

    // Iterate until destination is found
    idx2 = 0;
    while (bDestFound == FALSE && idx2 < 50)
    {
        idx2++;
        nDist++;
        printf("\nIteration %d \n", idx2);
        nQLenPrev = pQueue->len;  // Retrieve queue length.
        printf("Queue size: %d \n", nQLenPrev);

        // Get point info while deleting from queue, and check mobility.
        for (idx1 = 0; idx1 < nQLenPrev; idx1++)
        {
            // Delete from queue and get the point
            PointOld = PointQueueDelete(pQueue);
            printf("%d, %d deleted from queue \n", PointOld.row, PointOld.col);
            
            // Check mobility of the obtained point.
            nMobility = CheckMobility(maze, PointOld);

            // Check mobilities to each diretions
            if (nMobility & 0b1000)  // Can move to right
            {
                printf("Right \n");
                // Set new point
                PointNew = (Point){ .row = PointOld.row, .col = PointOld.col + 1 };

                // Check if the new point is the destination
                if (MazePointVal(maze, PointNew.row, PointNew.col) == dest)
                {
                    printf("bDestFound @ right \n");
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                    PointDest = PointNew;
                    bDestFound = TRUE;
                }
                else
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    printf("%d, %d added to queue \n", PointNew.row, PointNew.col);
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                }
            }

            if (nMobility & 0b0100)  // Can move to up
            {
                printf("Up \n");
                PointNew = (Point){ .row = PointOld.row - 1, .col = PointOld.col };

                // Check if the new point is the destination
                if (MazePointVal(maze, PointNew.row, PointNew.col) == dest)
                {
                    printf("bDestFound @ up \n");
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                    PointDest = PointNew;
                    bDestFound = TRUE;
                }
                else
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    printf("%d, %d added to queue \n", PointNew.row, PointNew.col);
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                }
            }

            if (nMobility & 0b0010)  // Can move to left
            {
                printf("Left \n");
                PointNew = (Point){ .row = PointOld.row, .col = PointOld.col - 1 };

                // Check if the new point is the destination
                if (MazePointVal(maze, PointNew.row, PointNew.col) == dest)
                {
                    printf("bDestFound @ left \n");
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                    PointDest = PointNew;
                    bDestFound = TRUE;
                }
                else
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    printf("%d, %d added to queue \n", PointNew.row, PointNew.col);
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                }
            }

            if (nMobility & 0b0001)  // Can move to down
            {
                printf("Down \n");
                PointNew = (Point){ .row = PointOld.row + 1, .col = PointOld.col };

                // Check if the new point is the destination
                if (MazePointVal(maze, PointNew.row, PointNew.col) == dest)
                {
                    printf("bDestFound @ down \n");
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                    PointDest = PointNew;
                    bDestFound = TRUE;
                }
                else
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    printf("%d, %d added to queue \n", PointNew.row, PointNew.col);
                    MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
                }
            }
            if (bDestFound == TRUE)
                printf("Found Destination! \n");
        }

        
    }
    
    // Now found the movement map. Now backtrace to the origin.
    // First, copy the destination index
    PointNew = PointDest;  // Use PointNew as a buffer of Point type
    nDist = MazePointVal(maze, PointNew.row, PointNew.col);
    PathAddPasspoint(pPath, PointNew, PATH_HEAD);
    idx2 = 0;
    while (PointCmp(PointNew, PointStart) == 0 && idx2 < 50)
    {
        idx2++;
        // Retrieve direction information
        nDirection = BackTrack_Vanilla(maze, PointNew, nDist);

        // Move PointNew according to the value of nDirection
        switch(nDirection)
        {
        case 0b1000:
            (PointNew.col)++;
            break;
        case 0b0100:
            (PointNew.row)--;
            break;
        case 0b0010:
            (PointNew.col)--;
            break;
        case 0b0001:
            (PointNew.row)++;
            break;
        }
        PathAddPasspoint(pPath, PointNew, PATH_HEAD);
        nDist--;
    }

    return pPath;
}


Path* WarpablePath(Maze* maze)
{
    /* 
     * To find a warpable path, it would be the most feasible to find a nearest
     * warping point from the starting point, and another nearest warping point
     * from the exit point.
     * Therefore, this function will first find the nearest warping point from
     * the starting point, and then another nearest warping point from the exit
     * point.
     */
    Path* pPath = PathInit();
}


int CheckMobility(Maze* maze, Point point)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nMobility = 0;
    int row, col;

    // Check if rightward movement is available.
    row = point.row; col = point.col + 1;
    if (col < maze->width)
        if (MazePointVal(maze, row, col) == 0 || MazePointVal(maze, row, col) == -4 || MazePointVal(maze, row, col) == -2)
            nMobility += 0b1000;

    // Check if upward movement is available.
    row = point.row - 1; col = point.col;
    if (row >= 0)
        if (MazePointVal(maze, row, col) == 0 || MazePointVal(maze, row, col) == -4 || MazePointVal(maze, row, col) == -2)
            nMobility += 0b0100;

    // Check if leftward movement is available.
    row = point.row; col = point.col - 1;
    if (col >= 0)
        if (MazePointVal(maze, row, col) == 0 || MazePointVal(maze, row, col) == -4 || MazePointVal(maze, row, col) == -2)
            nMobility += 0b0010;
    
    // Check if downward movement is available.
    row = point.row + 1; col = point.col;
    if (row < maze->height)
        if (MazePointVal(maze, row, col) == 0 || MazePointVal(maze, row, col) == -4 || MazePointVal(maze, row, col) == -2)
            nMobility += 0b0001;
    
    // printf("%d \n", nMobility);
    return nMobility;
}


int BackTrack(Maze* maze, Point point, int dist)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nDirection = 0;
    int row, col;
    int nNext;
    if (dist == 1)
        nNext = -1;
    else
        nNext = dist - 1;

    // Check if rightward movement is available.
    row = point.row; col = point.col + 1;
    if (col < maze->width)
        if (MazePointVal(maze, row, col) == nNext)
            nDirection += 0b1000;

    // Check if upward movement is available.
    row = point.row - 1; col = point.col;
    if (row >= 0)
        if (MazePointVal(maze, row, col) == nNext)
            nDirection += 0b0100;

    // Check if leftward movement is available.
    row = point.row; col = point.col - 1;
    if (col >= 0)
        if (MazePointVal(maze, row, col) == nNext)
            nDirection += 0b0010;
    
    // Check if downward movement is available.
    row = point.row + 1; col = point.col;
    if (row < maze->height)
        if (MazePointVal(maze, row, col) == nNext)
            nDirection += 0b0001;
    
    // printf("%d \n", nDirection);
    return nDirection;
}


int BackTrack_Vanilla(Maze* maze, Point point, int dist)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nDirection = 0;
    int row, col;
    int nNext;
    if (dist == 1)
        nNext = -1;
    else
        nNext = dist - 1;

    // Check if rightward movement is available.
    row = point.row; col = point.col + 1;
    if (col < maze->width)
        if (MazePointVal(maze, row, col) == nNext)
            return 0b1000;

    // Check if upward movement is available.
    row = point.row - 1; col = point.col;
    if (row >= 0)
        if (MazePointVal(maze, row, col) == nNext)
            return 0b0100;

    // Check if leftward movement is available.
    row = point.row; col = point.col - 1;
    if (col >= 0)
        if (MazePointVal(maze, row, col) == nNext)
            return 0b0010;
    
    // Check if downward movement is available.
    row = point.row + 1; col = point.col;
    if (row < maze->height)
        if (MazePointVal(maze, row, col) == nNext)
            return 0b0001;
}


#endif