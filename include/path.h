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
    LinkedPoint* pLPNext, *pLPSwap;
    pLPNext = path->head;
    int idx;
    for (idx = 0; idx < nPathLen; idx++)
    {
        pLPSwap = pLPNext->next;
        pLPNext->next = pLPNext->previous;
        pLPNext->previous = pLPSwap;
        pLPNext = pLPNext->previous;
    }
}


#endif