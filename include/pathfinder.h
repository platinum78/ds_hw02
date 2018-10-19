#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <stdio.h>
#include "./datatypes.h"

////////////////////////////////////////////////////////////
// Methods Definitions
////////////////////////////////////////////////////////////

Path* WarplessPath(Maze* maze, int start, int dest);
Path* WarpablePath(Maze* maze);
int CheckMobility(Maze* maze, Point point, int start);
int BackTrack(Maze* maze, Point point, int dist);
int BackTrack_Vanilla(Maze* maze, Point point, int dist, int dest);

#endif