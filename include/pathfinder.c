#ifndef __PATHFINDER_C__
#define __PATHFINDER_C__

#include <stdio.h>
#include <stdlib.h>
#include "./pathfinder.h"

Path* WarplessPath(Maze* maze, int start, int dest)
{
    // Initialize instances that would help the function work.
    // printf("WarplessPath \n");
    Path* pPath = PathInit();
    PointQueue* pQueue = PointQueueInit();
    Point PointOld, PointNew;
    Point PointStart, PointDest;  // Default destination is the exit point. Might change if wanted.
    Maze* maze_clone = MazeClone(maze);
    
    // Set buffers for function operation
    int bDestFound = FALSE;
    int nDist = 0;
    int nMobility, nDirection;  // Buffer to store mobility and direction information
    int nQLenPrev;  // Buffers to store queue length
    int idx1, idx2;  // Counters for loops.

    // Trigger by pushing starting point of the maze
    if (start == START)
        PointStart = MazeStart(maze_clone);
    else if (start == EXIT)
        PointStart = MazeExit(maze_clone);
    PointQueuePush(pQueue, PointStart);

    idx2 = 0;
    // Iterate until destination is found
    while (!bDestFound)
    {
        idx2++; nDist++;
        nQLenPrev = PointQueueLen(pQueue);  // Retrieve queue length.

        // Stop working if queue is empty: no path!
        if (nQLenPrev == 0)
        {
            printf("No path found! \n");
            return NULL;
        }

        // Get point info while deleting from queue, and check mobility.
        for (idx1 = 0; idx1 < nQLenPrev; idx1++)
        {
            // Pop previous point and check mobility of that point
            PointOld = PointQueuePop(pQueue);
            nMobility = CheckMobility(maze_clone, PointOld, start);

            // Check mobilities to each diretions
            if (nMobility & EAST && !bDestFound)  // Can move to right
            {
                // Set new point
                PointNew = PointOld; PointMove(&PointNew, 0, 1);
                // Check if the new point is the destination
                if (MazePointVal(maze_clone, PointNew) == dest)
                    bDestFound = TRUE;
                
                if (!bDestFound)
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    MazeMarkDist(maze_clone, PointNew, nDist);
                }
            }

            if (nMobility & NORTH && !bDestFound)  // Can move to up
            {
                // Set new point
                PointNew = PointOld; PointMove(&PointNew, -1, 0);

                // Check if the new point is the destination
                if (MazePointVal(maze_clone, PointNew) == dest)
                    bDestFound = TRUE;
                
                if (!bDestFound)
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    MazeMarkDist(maze_clone, PointNew, nDist);
                }
            }

            if (nMobility & WEST && !bDestFound)  // Can move to left
            {
                // Set new point
                PointNew = PointOld; PointMove(&PointNew, 0, -1);

                // Check if the new point is the destination
                if (MazePointVal(maze_clone, PointNew) == dest)
                    bDestFound = TRUE;
                
                if (!bDestFound)
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    MazeMarkDist(maze_clone, PointNew, nDist);
                }
            }

            if (nMobility & SOUTH && !bDestFound)  // Can move to down
            {
                // Set new point
                PointNew = PointOld; PointMove(&PointNew, 1, 0);

                // Check if the new point is the destination
                if (MazePointVal(maze_clone, PointNew) == dest)
                    bDestFound = TRUE;
                
                if (!bDestFound)
                {
                    // If not destination, push this point into the queue and mark the distance
                    PointQueuePush(pQueue, PointNew);
                    MazeMarkDist(maze_clone, PointNew, nDist);
                }
            }
            
            if (bDestFound)
            {
                printf("Destination Found at %d, %d! \n", PointNew.row, PointNew.col);
                MazeMarkDist(maze_clone, PointNew, nDist);
                PointDest = PointNew;
                break;
            }
        }
    }
    
    // Now found the movement map. Now backtrack to the origin.
    // First, copy the destination index
    PointNew = PointDest;  // Use PointNew as a Point-type buffer
    nDist = MazePointVal(maze_clone, PointNew);
    PathAddPasspoint(pPath, PointNew, PATH_HEAD);
    idx2 = 0;
    while (PointCmp(PointNew, PointStart) == 0 && idx2 < 5000)
    {
        idx2++;
        // Retrieve direction information
        nDirection = BackTrack_Vanilla(maze_clone, PointNew, nDist, start);

        // Move PointNew according to the value of nDirection
        switch(nDirection)
        {
        case EAST:
            PointMove(&PointNew, 0, 1);
            break;
        case NORTH:
            PointMove(&PointNew, -1, 0);
            break;
        case WEST:
            PointMove(&PointNew, 0, -1);
            break;
        case SOUTH:
            PointMove(&PointNew, 1, 0);
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
    Path* pPath1 = PathInit();
    Path* pPath2 = PathInit();
    Maze* maze_backup = MazeClone(maze);
    
    pPath1 = WarplessPath(maze, START, WARP);
    if (pPath1 == NULL)
        return NULL;
    
    MazeCopy(maze_backup, maze);
    pPath2 = WarplessPath(maze, EXIT, WARP);
    if (pPath2 == NULL)
        return NULL;
    PathFlip(pPath2);

    // printf("Checkpoint Passed! \n");
    Path* pPathMerged = PathMerge(pPath1, pPath2);
    
    PathDelete(pPath1); PathDelete(pPath2); pPath1 = NULL; pPath2 = NULL;
    MazeDelete(maze_backup); maze_backup = NULL;

    return pPathMerged;
}

int CheckMobility(Maze* maze, Point point, int start)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nMobility = 0;
    Point PointBuf;

    if (start == START)
    {    
        // Check if rightward movement is available.
        PointBuf = point; PointMove(&PointBuf, 0, 1);
        if (PointBuf.col < maze->width)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == EXIT)
                nMobility += EAST;

        // Check if upward movement is available.
        PointBuf = point; PointMove(&PointBuf, -1, 0);
        if (PointBuf.row >= 0)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == EXIT)
                nMobility += NORTH;

        // Check if leftward movement is available.
        PointBuf = point; PointMove(&PointBuf, 0, -1);
        if (PointBuf.col >= 0)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == EXIT)
                nMobility += WEST;
        
        // Check if downward movement is available.
        PointBuf = point; PointMove(&PointBuf, 1, 0);
        if (PointBuf.row < maze->height)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == -4 || MazePointVal(maze, PointBuf) == -2)
                nMobility += SOUTH;
    }
    else if (start == EXIT)
    {
        // Check if rightward movement is available.
        PointBuf = point; PointMove(&PointBuf, 0, 1);
        if (PointBuf.col < maze->width)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == START)
                nMobility += EAST;

        // Check if upward movement is available.
        PointBuf = point; PointMove(&PointBuf, -1, 0);
        if (PointBuf.row >= 0)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == START)
                nMobility += NORTH;

        // Check if leftward movement is available.
        PointBuf = point; PointMove(&PointBuf, 0, -1);
        if (PointBuf.col >= 0)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == START)
                nMobility += WEST;
        
        // Check if downward movement is available.
        PointBuf = point; PointMove(&PointBuf, 1, 0);
        if (PointBuf.row < maze->height)
            if (MazePointVal(maze, PointBuf) == 0 || MazePointVal(maze, PointBuf) == WARP || MazePointVal(maze, PointBuf) == START)
                nMobility += SOUTH;
    }
    return nMobility;
}

int BackTrack(Maze* maze, Point point, int dist)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nDirection = 0;
    Point PointBuf;
    int nNext;
    if (dist == 1)
        nNext = -1;
    else
        nNext = dist - 1;

    // Check if rightward movement is available.
    PointBuf = point; PointMove(&PointBuf, 0, 1);
    if (PointBuf.col < maze->width)
        if (MazePointVal(maze, PointBuf) == nNext)
            nDirection += EAST;

    // Check if upward movement is available.
    PointBuf = point; PointMove(&PointBuf, -1, 0);
    if (PointBuf.row >= 0)
        if (MazePointVal(maze, PointBuf) == nNext)
            nDirection += NORTH;

    // Check if leftward movement is available.
    PointBuf = point; PointMove(&PointBuf, 0, -1);
    if (PointBuf.col >= 0)
        if (MazePointVal(maze, PointBuf) == nNext)
            nDirection += WEST;
    
    // Check if downward movement is available.
    PointBuf = point; PointMove(&PointBuf, 1, 0);
    if (PointBuf.row < maze->height)
        if (MazePointVal(maze, PointBuf) == nNext)
            nDirection += SOUTH;

    return nDirection;
}


int BackTrack_Vanilla(Maze* maze, Point point, int dist, int start)
{
    // Check if movable to E, N, W, S, and return an integer in binary info.
    int nDirection = 0;
    Point PointBuf;
    int nNext;
    if (dist == 1)
        nNext = start;
    else
        nNext = dist - 1;

    // Check if rightward movement is available.
    PointBuf = point; PointMove(&PointBuf, 0, 1);
    if (PointBuf.col < maze->width)
        if (MazePointVal(maze, PointBuf) == nNext)
            return EAST;

    // Check if upward movement is available.
    PointBuf = point; PointMove(&PointBuf, -1, 0);
    if (PointBuf.row >= 0)
        if (MazePointVal(maze, PointBuf) == nNext)
            return NORTH;

    // Check if leftward movement is available.
    PointBuf = point; PointMove(&PointBuf, 0, -1);
    if (PointBuf.col >= 0)
        if (MazePointVal(maze, PointBuf) == nNext)
            return WEST;
    
    // Check if downward movement is available.
    PointBuf = point; PointMove(&PointBuf, 1, 0);
    if (PointBuf.row < maze->height)
        if (MazePointVal(maze, PointBuf) == nNext)
            return SOUTH;
}

#endif