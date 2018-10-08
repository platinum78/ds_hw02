#include <stdio.h>
#include "./include/maze.h"
#include "./include/utils.h"

Point test_function(int row, int col)
{
    Point point = (Point){ .row = row, .col = col };
    return point;
}

int main(void)
{
    Point point1;
    Point point2 = { 1, 1 };
    point1 = point2;

    printf("%d \n", PointCmp(point1, point2));
}