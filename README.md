# Programming Assignment #2 - Maze Escape

Data Structure SWE2015-41, School of Software, SKKU <br>
Susung Park (2014311254), School of Mechanical Engineering, SKKU

<br>

## 1. Just tell me how to use it!

Using the program is simple. Just following the instruction below.

1. Open the assignment directory from the console.
2. Compile the source via gcc by typing `gcc -o main.exe ./include/datatypes.c ./include/pathfinder.c ./main.c`
3. Put your input file in `{ASSIGNMENT_DIR}/io`. Input file should have the name `hw2_input.txt`.
4. Run the program with no option.
5. The output file will emerge in the same diredtory with the input file.

This program is tested on Windows 10 (build 1803).

<br>

## 2. Objectives

In this assignment, the followings are considered.

1. Finding accurate paths
2. Modulization through usage of functions (i.e. simple `main` function)
3. Simplification of the problem

In this CoC, the program finds the shortest path.

<br>

## 3. Operations

The data types and operations are arranged in the `datatypes.h` header.
Please refer to this file to see which operations are implemented.

<br>

## 4. Problem Analysis

### 1. Algorithms

The identical algorithm with that from class is applied for plain path-finding.

### 2. Warping?

The real problem is that there are some *'warping zones'* in this assignment.
This makes the problem seem more difficult, but actually it's just a trick,
and does not make the problem significantly more difficult.

Since it costs no energy to move between warping points,
how far the two warping points are does not matter at all.
Therefore, what we should consider is to minimize the energy cost while moving
from the starting point to the warping zone, and then from another warping zone to the exit point.
The problem indeed reduced into finding two shortest routes!

One possible confusion is the 'underground tunnel' case; which there is no warpless path but warpable path exists.
This happens when starting point and exit point are isolated by walls, but each side have at least one warping zone.
However, since finding the shortest route from the starting/exit point to the warping zone would indicate if there is
any warping zone on that side, this situation does not need any separate classification procedure.

Therefore, the provided methods will cover all cases.
After that, we can flip the path, and then merge it with the path from starting point to warping zone.

In summary, the operating procedure of the program is as follows.

1. For warpless paths, run route-finding algorithm learned in class.
2. For warpable paths,
   * Find a forward path from the starting point to the closest warping zone.
   * Find a backward path from the exit point to the closest warping zone.
   * Flip the backward path to make a forward path.
   * Merge two paths into one.
3. Compare the warpless and warpable paths, and write one with shorter distance.

<br>

## 4. Performance Analysis

This algorithm differs its number of iterations significantly case-by-case.
Therefore, it's not easy to find the general Theta(g(n)) of the algorithm.
However, assuming the worst case, which is the case that there is no wall and the start and exit points are diagonally opposite, we can find the asymptotic expression.
Since the algorithm would seek for all points in the maze map, time complexity will be **O(mn)** when the size of map is m x n.

In case of warpable paths, it is likely that the number of operations in path finding would significantly decrease.
Nevertheless, when there is only one or no warping zone, the amount of path-finding calculation will be equal to that of worst case in warpless paths.
However, these exceptions are not handled, for the following reasons.

* Algorithm is not heavy enough to make the user feel uncomfortable.
* Not only the number of warping zone but also the relative location to the walls is important. This might require *semantic* analysis on morphology of maze map.

<br>

## 5. Benchmarks

This program is written and tested on my workstation.
The specs of the workstation are: Windows 10 Pro (build 1803), AMD Ryzen 2700X (~4GHz), 32G DDR4 RAM

Using the default input, approximately 3-5ms is required.
Using a maze input with 550 x 100, approximately 50ms is required.
Using a maze input with 10000 x 200, approximately 540ms is required.