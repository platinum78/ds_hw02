#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
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
        (queue->tail)->next = pLPoint;
        queue->tail = pLPoint;
    }
    
    (queue->len)++;
}


Point PointQueueDelete(PointQueue* queue)
{
    // Make a Point object to return values from deleted point
    Point newPoint;
    newPoint.row = (queue->head->point).row;
    newPoint.col = (queue->head->point).col;

    // Delete the first element of the queue
    if (queue->len == 0)
    {
        // If the queue is empty, removal operation should raise an error.
        return;
    }
    else if (queue->len == 1)
    {
        // If queue has only one element, removing would leave no element in the queue.
        free(queue->head);
        queue->head = queue->tail = NULL;
    }
    else
    {
        LinkedPoint* pLPoint = queue->head->next;  // A buffer to store the pointer to second element
        free(queue->head);
        queue->head = pLPoint;
    }

    return newPoint;
}


Path* WarplessPath(Maze* maze);
Path* WarpablePath(Maze* maze);
int CheckMobility(Maze* maze, Point point);


Path* WarplessPath(Maze* maze)
{
    // Initialize instances that would help the function work.
    Path* pPath = PathInit();
    PointQueue* pQueue = PointQueueInit();
    Point PointOld, PointNew;
    Point PointDest = maze->exit;  // Default destination is the exit point. Might change if wanted.
    int bDestFound = FALSE;
    int nDist = 0;
    int nMobility;
    int nQLenPrev, nQLenCurr;  // Buffers to store queue length
    int idx1, idx2, idx3, idx4;  // Counters for loops.

    // Trigger by pushing starting point of the maze
    PointQueuePush(pQueue, maze->start);

    // Iterate until destination is found
    while (bDestFound == FALSE)
    {
        nDist++;
        nQLenPrev = pQueue->len;  // Retrieve queue length.
        // Get point info while deleting from queue, and check mobility.
        for (idx1 = 0; idx1 < nQLenPrev; idx1++)
        {
            // Delete from queue and get the point
            PointOld = PointQueueDelete(pQueue);

            // Check mobility of the obtained point.
            nMobility = CheckMobility(maze, PointOld);

            // Check mobilities to each diretions
            if (nMobility & 0b1000)  // Can move to right
            {
                // Set new point
                PointNew = (Point){ .row = PointOld.row, .col = PointOld.col + 1 };

                // Check if the new point is the destination
                if (PointCmp(PointNew, PointDest))
                {
                    bDestFound = TRUE;
                    continue;
                }

                // If not destination, push this point into the queue and mark the distance
                PointQueuePush(pQueue, PointNew);
                MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
            }
            else if (nMobility & 0b0100)  // Can move to up
            {
                PointNew = (Point){ .row = PointOld.row - 1, .col = PointOld.col };

                // Check if the new point is the destination
                if (PointCmp(PointNew, PointDest))
                {
                    bDestFound = TRUE;
                    continue;
                }

                // If not destination, push this point into the queue
                PointQueuePush(pQueue, PointNew);
                MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
            }
            else if (nMobility & 0b0010)  // Can move to left
            {
                PointNew = (Point){ .row = PointOld.row, .col = PointOld.col - 1 };

                // Check if the new point is the destination
                if (PointCmp(PointNew, PointDest))
                {
                    bDestFound = TRUE;
                    continue;
                }

                // If not destination, push this point into the queue
                PointQueuePush(pQueue, PointNew);
                MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
            }
            else if (nMobility & 0b0001)  // Can move to down
            {
                PointNew = (Point){ .row = PointOld.row + 1, .col = PointOld.col };

                // Check if the new point is the destination
                if (PointCmp(PointNew, PointDest))
                {
                    bDestFound = TRUE;
                    continue;
                }

                // If not destination, push this point into the queue
                PointQueuePush(pQueue, PointNew);
                MazeMarkDist(maze, PointNew.row, PointNew.col, nDist);
            }
        }

    }
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

    // Check if rightware movement is available.
    row = point.row; col = point.col + 1;
    if (col < maze->width)
        if (MazePoint(maze, row, col) == '0')
            nMobility += 0b1000;

    // Check if upward movement is available.
    row = point.row - 1; col = point.col;
    if (row >= 0)
        if (MazePoint(maze, row, col) == '0')
            nMobility += 0b0100;

    // Check if leftward movement is available.
    row = point.row; col = point.col - 1;
    if (col >= 0)
        if (MazePoint(maze, row, col) == '0')
            nMobility += 0b0010;
    
    // Check if downward movement is available.
    row = point.row + 1; col = point.col;
    if (row < maze->height)
        if (MazePoint(maze, row, col) == '0')
            nMobility += 0b0001;
    
    return nMobility;
}


#endif