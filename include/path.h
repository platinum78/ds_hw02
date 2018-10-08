#ifndef __PATH_H__
#define __PATH_H__

#include <stdio.h>
#include <stdlib.h>
#include "./maze.h"


typedef struct LinkedPoint
{
    Point point;
    LinkedPoint* next;
} LinkedPoint;


typedef struct Path
{
    int len;
    LinkedPoint* path;
} Path;


Path* PathInit()
{
    Path* pPath = (Path*)malloc(sizeof(Path));
    pPath->len = 0;
    pPath->path = NULL;
}


#endif