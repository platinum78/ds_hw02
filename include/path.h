#ifndef __PATH_H__
#define __PATH_H__

#define PATH_HEAD 0
#define PATH_TAIL 1

#include <stdio.h>
#include <stdlib.h>
#include "./maze.h"


typedef struct LinkedPoint_
{
    Point point;
    struct LinkedPoint_* previous;
    struct LinkedPoint_* next;
} LinkedPoint;


typedef struct Path_
{
    int len;
    LinkedPoint* head;
    LinkedPoint* tail;
} Path;


Path* PathInit()
{
    Path* pPath = (Path*)malloc(sizeof(Path));
    pPath->len = 0;
    pPath->head = NULL;
    pPath->tail = NULL;
}


void PathAddPasspoint(Path* path, Point point, int pos)
{
    // Allocate LinkedPoint instance and set the point.
    LinkedPoint* pLPoint = (LinkedPoint*)malloc(sizeof(LinkedPoint));
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


Path* PathMerge(Path* path1, Path* path2)
{
    // Get total length of new path
    int nPathLen = path1->len + path2->len;
    printf("Length of new path: %d \n", path1->len);

    // Create a new Path instance
    Path* pPath = (Path*)malloc(sizeof(Path));

    // Copy first object of path 1
    LinkedPoint* pLPNext = path1->head;
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


#endif