#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include <stdio.h>


////////////////////////////////////////////////////////////
// Parameters definitions
////////////////////////////////////////////////////////////

#define START -1
#define EXIT -2
#define WALL -3
#define WARP -4

#define PATH_HEAD 0
#define PATH_TAIL 1

#define TRUE 1
#define FALSE 0

#define EAST    0b1000
#define NORTH   0b0100
#define WEST    0b0010
#define SOUTH   0b0001


////////////////////////////////////////////////////////////
// Structurues definitions
////////////////////////////////////////////////////////////

typedef struct Point
{
    int row;
    int col;
} Point;

typedef struct LinkedPoint_
{
    Point point;
    struct LinkedPoint_* previous;
    struct LinkedPoint_* next;
} LinkedPoint;

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

typedef struct Path_
{
    int len;
    LinkedPoint* head;
    LinkedPoint* tail;
} Path;

typedef struct PointQueue
{
    int len;
    LinkedPoint* head;
    LinkedPoint* tail;
} PointQueue;


////////////////////////////////////////////////////////////
// Methods definitions
////////////////////////////////////////////////////////////

// Point
Point* PointInit(int row, int col);
void PointSet(Point* point, int row, int col);
void PointMove(Point* point, int drow, int dcol);
int PointCmp(Point p1, Point p2);

// Maze
Maze* MazeInit(void);
void MazeRead(FILE*, Maze*);
void MazeTrim(Maze*);
Point MazeStart(Maze*);
Point MazeExit(Maze*);
int MazePointVal(Maze*, Point);
void MazeMarkDist(Maze* maze, Point point, int dist);
Maze* MazeClone(Maze* maze);
void MazeCopy(Maze* maze_src, Maze* maze_dst);
void MazeDelete(Maze* maze);

// LinkedPoint
LinkedPoint* LinkedPointInit(void);

// Path
Path* PathInit(void);
void PathAddPasspoint(Path* path, Point point, int pos);
void PathFlip(Path* path);
Path* PathMerge(Path* path1, Path* path2);
void PathDelete(Path* path);

// PointQueue
PointQueue* PointQueueInit();
void PointQueuePush(PointQueue* queue, Point point);
Point PointQueuePop(PointQueue* queue);
int PointQueueLen(PointQueue*);

#endif